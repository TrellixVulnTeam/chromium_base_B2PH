// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mojo/core/channel.h"

#include <stdint.h>
#include <windows.h>

#include <algorithm>
#include <limits>
#include <memory>

#include "base/bind.h"
#include "base/containers/queue.h"
#include "base/location.h"
#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop_current.h"
#include "base/message_loop/message_pump_for_io.h"
#include "base/process/process_handle.h"
#include "base/synchronization/lock.h"
#include "base/task_runner.h"
#include "base/win/scoped_handle.h"
#include "base/win/win_util.h"

namespace mojo {
namespace core {

namespace {

class ChannelWin : public Channel,
                   public base::MessageLoopCurrent::DestructionObserver,
                   public base::MessagePumpForIO::IOHandler {
 public:
  ChannelWin(Delegate* delegate,
             ConnectionParams connection_params,
             HandlePolicy handle_policy,
             scoped_refptr<base::TaskRunner> io_task_runner)
      : Channel(delegate, handle_policy),
        self_(this),
        io_task_runner_(io_task_runner) {
    if (connection_params.server_endpoint().is_valid()) {
      handle_ = connection_params.TakeServerEndpoint()
                    .TakePlatformHandle()
                    .TakeHandle();
      needs_connection_ = true;
    } else {
      handle_ =
          connection_params.TakeEndpoint().TakePlatformHandle().TakeHandle();
    }

    CHECK(handle_.IsValid());
  }

  void Start() override {
    io_task_runner_->PostTask(
        FROM_HERE, base::BindOnce(&ChannelWin::StartOnIOThread, this));
  }

  void ShutDownImpl() override {
    // Always shut down asynchronously when called through the public interface.
    io_task_runner_->PostTask(
        FROM_HERE, base::BindOnce(&ChannelWin::ShutDownOnIOThread, this));
  }

  void Write(MessagePtr message) override {
    if (remote_process().is_valid()) {
      // If we know the remote process handle, we transfer all outgoing handles
      // to the process now rewriting them in the message.
      std::vector<PlatformHandleInTransit> handles = message->TakeHandles();
      for (auto& handle : handles) {
        if (handle.handle().is_valid())
          handle.TransferToProcess(remote_process().Clone());
      }
      message->SetHandles(std::move(handles));
    }

    bool write_error = false;
    {
      base::AutoLock lock(write_lock_);
      if (reject_writes_)
        return;

      bool write_now = !delay_writes_ && outgoing_messages_.empty();
      outgoing_messages_.emplace_back(std::move(message));
      if (write_now && !WriteNoLock(outgoing_messages_.front().get()))
        reject_writes_ = write_error = true;
    }
    if (write_error) {
      // Do not synchronously invoke OnWriteError(). Write() may have been
      // called by the delegate and we don't want to re-enter it.
      io_task_runner_->PostTask(FROM_HERE,
                                base::BindOnce(&ChannelWin::OnWriteError, this,
                                               Error::kDisconnected));
    }
  }

  void LeakHandle() override {
    DCHECK(io_task_runner_->RunsTasksInCurrentSequence());
    leak_handle_ = true;
  }

  bool GetReadPlatformHandles(const void* payload,
                              size_t payload_size,
                              size_t num_handles,
                              const void* extra_header,
                              size_t extra_header_size,
                              std::vector<PlatformHandle>* handles,
                              bool* deferred) override {
    DCHECK(extra_header);
    if (num_handles > std::numeric_limits<uint16_t>::max())
      return false;
    using HandleEntry = Channel::Message::HandleEntry;
    size_t handles_size = sizeof(HandleEntry) * num_handles;
    if (handles_size > extra_header_size)
      return false;
    handles->reserve(num_handles);
    const HandleEntry* extra_header_handles =
        reinterpret_cast<const HandleEntry*>(extra_header);
    for (size_t i = 0; i < num_handles; i++) {
      HANDLE handle_value =
          base::win::Uint32ToHandle(extra_header_handles[i].handle);
      if (PlatformHandleInTransit::IsPseudoHandle(handle_value))
        return false;
      if (remote_process().is_valid()) {
        // If we know the remote process's handle, we assume it doesn't know
        // ours; that means any handle values still belong to that process, and
        // we need to transfer them to this process.
        handle_value = PlatformHandleInTransit::TakeIncomingRemoteHandle(
                           handle_value, remote_process().get())
                           .ReleaseHandle();
      }
      handles->emplace_back(base::win::ScopedHandle(std::move(handle_value)));
    }
    return true;
  }

 private:
  // May run on any thread.
  ~ChannelWin() override {}

  void StartOnIOThread() {
    base::MessageLoopCurrent::Get()->AddDestructionObserver(this);
    base::MessageLoopCurrentForIO::Get()->RegisterIOHandler(handle_.Get(),
                                                            this);

    if (needs_connection_) {
      BOOL ok = ::ConnectNamedPipe(handle_.Get(), &connect_context_.overlapped);
      if (ok) {
        PLOG(ERROR) << "Unexpected success while waiting for pipe connection";
        OnError(Error::kConnectionFailed);
        return;
      }

      const DWORD err = GetLastError();
      switch (err) {
        case ERROR_PIPE_CONNECTED:
          break;
        case ERROR_IO_PENDING:
          is_connect_pending_ = true;
          AddRef();
          return;
        case ERROR_NO_DATA:
        default:
          OnError(Error::kConnectionFailed);
          return;
      }
    }

    // Now that we have registered our IOHandler, we can start writing.
    {
      base::AutoLock lock(write_lock_);
      if (delay_writes_) {
        delay_writes_ = false;
        WriteNextNoLock();
      }
    }

    // Keep this alive in case we synchronously run shutdown, via OnError(),
    // as a result of a ReadFile() failure on the channel.
    scoped_refptr<ChannelWin> keep_alive(this);
    ReadMore(0);
  }

  void ShutDownOnIOThread() {
    base::MessageLoopCurrent::Get()->RemoveDestructionObserver(this);

    // TODO(https://crbug.com/583525): This function is expected to be called
    // once, and |handle_| should be valid at this point.
    CHECK(handle_.IsValid());
    CancelIo(handle_.Get());
    if (leak_handle_)
      ignore_result(handle_.Take());
    else
      handle_.Close();

    // Allow |this| to be destroyed as soon as no IO is pending.
    self_ = nullptr;
  }

  // base::MessageLoopCurrent::DestructionObserver:
  void WillDestroyCurrentMessageLoop() override {
    DCHECK(io_task_runner_->RunsTasksInCurrentSequence());
    if (self_)
      ShutDownOnIOThread();
  }

  // base::MessageLoop::IOHandler:
  void OnIOCompleted(base::MessagePumpForIO::IOContext* context,
                     DWORD bytes_transfered,
                     DWORD error) override {
    if (error != ERROR_SUCCESS) {
      if (context == &write_context_) {
        {
          base::AutoLock lock(write_lock_);
          reject_writes_ = true;
        }
        OnWriteError(Error::kDisconnected);
      } else {
        OnError(Error::kDisconnected);
      }
    } else if (context == &connect_context_) {
      DCHECK(is_connect_pending_);
      is_connect_pending_ = false;
      ReadMore(0);

      base::AutoLock lock(write_lock_);
      if (delay_writes_) {
        delay_writes_ = false;
        WriteNextNoLock();
      }
    } else if (context == &read_context_) {
      OnReadDone(static_cast<size_t>(bytes_transfered));
    } else {
      CHECK(context == &write_context_);
      OnWriteDone(static_cast<size_t>(bytes_transfered));
    }
    Release();
  }

  void OnReadDone(size_t bytes_read) {
    DCHECK(is_read_pending_);
    is_read_pending_ = false;

    if (bytes_read > 0) {
      size_t next_read_size = 0;
      if (OnReadComplete(bytes_read, &next_read_size)) {
        ReadMore(next_read_size);
      } else {
        OnError(Error::kReceivedMalformedData);
      }
    } else if (bytes_read == 0) {
      OnError(Error::kDisconnected);
    }
  }

  void OnWriteDone(size_t bytes_written) {
    if (bytes_written == 0)
      return;

    bool write_error = false;
    {
      base::AutoLock lock(write_lock_);

      DCHECK(is_write_pending_);
      is_write_pending_ = false;
      DCHECK(!outgoing_messages_.empty());

      Channel::MessagePtr message = std::move(outgoing_messages_.front());
      outgoing_messages_.pop_front();

      // Overlapped WriteFile() to a pipe should always fully complete.
      if (message->data_num_bytes() != bytes_written)
        reject_writes_ = write_error = true;
      else if (!WriteNextNoLock())
        reject_writes_ = write_error = true;
    }
    if (write_error)
      OnWriteError(Error::kDisconnected);
  }

  void ReadMore(size_t next_read_size_hint) {
    DCHECK(!is_read_pending_);

    size_t buffer_capacity = next_read_size_hint;
    char* buffer = GetReadBuffer(&buffer_capacity);
    DCHECK_GT(buffer_capacity, 0u);

    BOOL ok =
        ::ReadFile(handle_.Get(), buffer, static_cast<DWORD>(buffer_capacity),
                   NULL, &read_context_.overlapped);
    if (ok || GetLastError() == ERROR_IO_PENDING) {
      is_read_pending_ = true;
      AddRef();
    } else {
      OnError(Error::kDisconnected);
    }
  }

  bool WriteNoLock(Channel::Message* message) {
    // We can release all the handles immediately now that we're attempting an
    // actual write to the remote process.
    //
    // If the HANDLE values are locally owned, that means we're sending them
    // to a broker who will duplicate-and-close them. If the broker never
    // receives that message (and thus we effectively leak these handles),
    // either it died (and our total dysfunction is imminent) or we died; in
    // either case the handle leak doesn't matter.
    //
    // If the handles have already been transferred and are therefore remotely
    // owned, the only way they won't eventually be managed by the remote
    // process is if the remote process dies before receiving this message. At
    // that point, again, potential handle leaks don't matter.
    std::vector<PlatformHandleInTransit> handles = message->TakeHandles();
    for (auto& handle : handles)
      handle.CompleteTransit();

    BOOL ok = WriteFile(handle_.Get(), message->data(),
                        static_cast<DWORD>(message->data_num_bytes()), NULL,
                        &write_context_.overlapped);
    if (ok || GetLastError() == ERROR_IO_PENDING) {
      is_write_pending_ = true;
      AddRef();
      return true;
    }
    return false;
  }

  bool WriteNextNoLock() {
    if (outgoing_messages_.empty())
      return true;
    return WriteNoLock(outgoing_messages_.front().get());
  }

  void OnWriteError(Error error) {
    DCHECK(io_task_runner_->RunsTasksInCurrentSequence());
    DCHECK(reject_writes_);

    if (error == Error::kDisconnected) {
      // If we can't write because the pipe is disconnected then continue
      // reading to fetch any in-flight messages, relying on end-of-stream to
      // signal the actual disconnection.
      if (is_read_pending_ || is_connect_pending_)
        return;
    }

    OnError(error);
  }

  // Keeps the Channel alive at least until explicit shutdown on the IO thread.
  scoped_refptr<Channel> self_;

  // The pipe handle this Channel uses for communication.
  base::win::ScopedHandle handle_;

  // Indicates whether |handle_| must wait for a connection.
  bool needs_connection_ = false;

  const scoped_refptr<base::TaskRunner> io_task_runner_;

  base::MessagePumpForIO::IOContext connect_context_;
  base::MessagePumpForIO::IOContext read_context_;
  bool is_connect_pending_ = false;
  bool is_read_pending_ = false;

  // Protects all fields potentially accessed on multiple threads via Write().
  base::Lock write_lock_;
  base::MessagePumpForIO::IOContext write_context_;
  base::circular_deque<Channel::MessagePtr> outgoing_messages_;
  bool delay_writes_ = true;
  bool reject_writes_ = false;
  bool is_write_pending_ = false;

  bool leak_handle_ = false;

  DISALLOW_COPY_AND_ASSIGN(ChannelWin);
};

}  // namespace

// static
scoped_refptr<Channel> Channel::Create(
    Delegate* delegate,
    ConnectionParams connection_params,
    HandlePolicy handle_policy,
    scoped_refptr<base::TaskRunner> io_task_runner) {
  return new ChannelWin(delegate, std::move(connection_params), handle_policy,
                        io_task_runner);
}

}  // namespace core
}  // namespace mojo

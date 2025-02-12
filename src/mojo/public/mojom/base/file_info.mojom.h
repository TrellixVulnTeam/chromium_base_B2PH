// mojo/public/mojom/base/file_info.mojom.h is auto generated by mojom_bindings_generator.py, do not edit

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_MOJOM_BASE_FILE_INFO_MOJOM_H_
#define MOJO_PUBLIC_MOJOM_BASE_FILE_INFO_MOJOM_H_

#include <stdint.h>

#include <limits>
#include <type_traits>
#include <utility>

#include "base/callback.h"
#include "base/macros.h"
#include "base/optional.h"

#include "mojo/public/cpp/bindings/mojo_buildflags.h"
#if BUILDFLAG(MOJO_TRACE_ENABLED)
#include "base/trace_event/trace_event.h"
#endif
#include "mojo/public/cpp/bindings/clone_traits.h"
#include "mojo/public/cpp/bindings/equals_traits.h"
#include "mojo/public/cpp/bindings/lib/serialization.h"
#include "mojo/public/cpp/bindings/struct_ptr.h"
#include "mojo/public/cpp/bindings/struct_traits.h"
#include "mojo/public/cpp/bindings/union_traits.h"
#include "mojo/public/mojom/base/file_info.mojom-shared.h"
#include "mojo/public/mojom/base/file_info.mojom-forward.h"
#include "mojo/public/mojom/base/time.mojom.h"
#include <string>
#include <vector>




#include "base/files/file.h"
#include "base/component_export.h"




namespace mojo_base {
namespace mojom {








class COMPONENT_EXPORT(MOJO_BASE_MOJOM) FileInfo {
 public:
  template <typename T>
  using EnableIfSame = std::enable_if_t<std::is_same<FileInfo, T>::value>;
  using DataView = FileInfoDataView;
  using Data_ = internal::FileInfo_Data;

  template <typename... Args>
  static FileInfoPtr New(Args&&... args) {
    return FileInfoPtr(
        base::in_place, std::forward<Args>(args)...);
  }

  template <typename U>
  static FileInfoPtr From(const U& u) {
    return mojo::TypeConverter<FileInfoPtr, U>::Convert(u);
  }

  template <typename U>
  U To() const {
    return mojo::TypeConverter<U, FileInfo>::Convert(*this);
  }


  FileInfo();

  FileInfo(
      int64_t size,
      bool is_directory,
      bool is_symbolic_link,
      ::base::Time last_modified,
      ::base::Time last_accessed,
      ::base::Time creation_time);

  ~FileInfo();

  // Clone() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Clone() or copy
  // constructor/assignment are available for members.
  template <typename StructPtrType = FileInfoPtr>
  FileInfoPtr Clone() const;

  // Equals() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Equals() or == operator
  // are available for members.
  template <typename T, FileInfo::EnableIfSame<T>* = nullptr>
  bool Equals(const T& other) const;

  template <typename UserType>
  static std::vector<uint8_t> Serialize(UserType* input) {
    return mojo::internal::SerializeImpl<
        FileInfo::DataView, std::vector<uint8_t>>(input);
  }

  template <typename UserType>
  static mojo::Message SerializeAsMessage(UserType* input) {
    return mojo::internal::SerializeAsMessageImpl<
        FileInfo::DataView>(input);
  }

  // The returned Message is serialized only if the message is moved
  // cross-process or cross-language. Otherwise if the message is Deserialized
  // as the same UserType |input| will just be moved to |output| in
  // DeserializeFromMessage.
  template <typename UserType>
  static mojo::Message WrapAsMessage(UserType input) {
    return mojo::Message(std::make_unique<
        internal::FileInfo_UnserializedMessageContext<
            UserType, FileInfo::DataView>>(0, 0, std::move(input)));
  }

  template <typename UserType>
  static bool Deserialize(const void* data,
                          size_t data_num_bytes,
                          UserType* output) {
    return mojo::internal::DeserializeImpl<FileInfo::DataView>(
        data, data_num_bytes, std::vector<mojo::ScopedHandle>(), output, Validate);
  }

  template <typename UserType>
  static bool Deserialize(const std::vector<uint8_t>& input,
                          UserType* output) {
    return FileInfo::Deserialize(
        input.size() == 0 ? nullptr : &input.front(), input.size(), output);
  }

  template <typename UserType>
  static bool DeserializeFromMessage(mojo::Message input,
                                     UserType* output) {
    auto context = input.TakeUnserializedContext<
        internal::FileInfo_UnserializedMessageContext<
            UserType, FileInfo::DataView>>();
    if (context) {
      *output = std::move(context->TakeData());
      return true;
    }
    input.SerializeIfNecessary();
    return mojo::internal::DeserializeImpl<FileInfo::DataView>(
        input.payload(), input.payload_num_bytes(),
        std::move(*input.mutable_handles()), output, Validate);
  }

  
  int64_t size;
  
  bool is_directory;
  
  bool is_symbolic_link;
  
  ::base::Time last_modified;
  
  ::base::Time last_accessed;
  
  ::base::Time creation_time;

 private:
  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);
};

// The comparison operators are templates, so they are only instantiated if they
// are used. Thus, the bindings generator does not need to know whether
// comparison operators are available for members.
template <typename T, FileInfo::EnableIfSame<T>* = nullptr>
bool operator<(const T& lhs, const T& rhs);

template <typename T, FileInfo::EnableIfSame<T>* = nullptr>
bool operator<=(const T& lhs, const T& rhs) {
  return !(rhs < lhs);
}

template <typename T, FileInfo::EnableIfSame<T>* = nullptr>
bool operator>(const T& lhs, const T& rhs) {
  return rhs < lhs;
}

template <typename T, FileInfo::EnableIfSame<T>* = nullptr>
bool operator>=(const T& lhs, const T& rhs) {
  return !(lhs < rhs);
}

template <typename StructPtrType>
FileInfoPtr FileInfo::Clone() const {
  return New(
      mojo::Clone(size),
      mojo::Clone(is_directory),
      mojo::Clone(is_symbolic_link),
      mojo::Clone(last_modified),
      mojo::Clone(last_accessed),
      mojo::Clone(creation_time)
  );
}

template <typename T, FileInfo::EnableIfSame<T>*>
bool FileInfo::Equals(const T& other_struct) const {
  if (!mojo::Equals(this->size, other_struct.size))
    return false;
  if (!mojo::Equals(this->is_directory, other_struct.is_directory))
    return false;
  if (!mojo::Equals(this->is_symbolic_link, other_struct.is_symbolic_link))
    return false;
  if (!mojo::Equals(this->last_modified, other_struct.last_modified))
    return false;
  if (!mojo::Equals(this->last_accessed, other_struct.last_accessed))
    return false;
  if (!mojo::Equals(this->creation_time, other_struct.creation_time))
    return false;
  return true;
}

template <typename T, FileInfo::EnableIfSame<T>*>
bool operator<(const T& lhs, const T& rhs) {
  if (lhs.size < rhs.size)
    return true;
  if (rhs.size < lhs.size)
    return false;
  if (lhs.is_directory < rhs.is_directory)
    return true;
  if (rhs.is_directory < lhs.is_directory)
    return false;
  if (lhs.is_symbolic_link < rhs.is_symbolic_link)
    return true;
  if (rhs.is_symbolic_link < lhs.is_symbolic_link)
    return false;
  if (lhs.last_modified < rhs.last_modified)
    return true;
  if (rhs.last_modified < lhs.last_modified)
    return false;
  if (lhs.last_accessed < rhs.last_accessed)
    return true;
  if (rhs.last_accessed < lhs.last_accessed)
    return false;
  if (lhs.creation_time < rhs.creation_time)
    return true;
  if (rhs.creation_time < lhs.creation_time)
    return false;
  return false;
}


}  // namespace mojom
}  // namespace mojo_base

namespace mojo {


template <>
struct COMPONENT_EXPORT(MOJO_BASE_MOJOM) StructTraits<::mojo_base::mojom::FileInfo::DataView,
                                         ::mojo_base::mojom::FileInfoPtr> {
  static bool IsNull(const ::mojo_base::mojom::FileInfoPtr& input) { return !input; }
  static void SetToNull(::mojo_base::mojom::FileInfoPtr* output) { output->reset(); }

  static decltype(::mojo_base::mojom::FileInfo::size) size(
      const ::mojo_base::mojom::FileInfoPtr& input) {
    return input->size;
  }

  static decltype(::mojo_base::mojom::FileInfo::is_directory) is_directory(
      const ::mojo_base::mojom::FileInfoPtr& input) {
    return input->is_directory;
  }

  static decltype(::mojo_base::mojom::FileInfo::is_symbolic_link) is_symbolic_link(
      const ::mojo_base::mojom::FileInfoPtr& input) {
    return input->is_symbolic_link;
  }

  static const decltype(::mojo_base::mojom::FileInfo::last_modified)& last_modified(
      const ::mojo_base::mojom::FileInfoPtr& input) {
    return input->last_modified;
  }

  static const decltype(::mojo_base::mojom::FileInfo::last_accessed)& last_accessed(
      const ::mojo_base::mojom::FileInfoPtr& input) {
    return input->last_accessed;
  }

  static const decltype(::mojo_base::mojom::FileInfo::creation_time)& creation_time(
      const ::mojo_base::mojom::FileInfoPtr& input) {
    return input->creation_time;
  }

  static bool Read(::mojo_base::mojom::FileInfo::DataView input, ::mojo_base::mojom::FileInfoPtr* output);
};

}  // namespace mojo

#endif  // MOJO_PUBLIC_MOJOM_BASE_FILE_INFO_MOJOM_H_
// WindowsProject1.cpp : 定义应用程序的入口点。
//

#include "WindowsProject1.h"

#include <ipc/ipc_channel.h>
#include <ipc/ipc_listener.h>
#include <ipc/ipc_channel_mojo.h>
#include <base/memory/read_only_shared_memory_region.h>
#include <base/memory/ref_counted_memory.h>
#include <base/memory/scoped_refptr.h>
#include <base/message_loop/message_loop.h>
#include <base/task/single_thread_task_executor.h>
#include <base/threading/thread.h>
#include <base/threading/thread_task_runner_handle.h>
#include <base/timer/timer.h>
#include <base/power_monitor/power_monitor.h>
#include <base/power_monitor/power_monitor_device_source.h>
#include <base/logging.h>
#include <base/command_line.h>
#include <base/message_loop/message_loop_current.h>
#include <mojo/core/embedder/embedder.h>
#include <url/url_util.h>
#include "framework.h"
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                      // 当前实例
WCHAR szTitle[MAX_LOADSTRING];        // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];  // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);


void SendString(IPC::Sender* sender, const std::string& str) {
  IPC::Message* message = new IPC::Message(0, 2, IPC::Message::PRIORITY_NORMAL);
  message->WriteString(str);
  sender->Send(message);
}

void SendValue(IPC::Sender* sender, int32_t value) {
  IPC::Message* message = new IPC::Message(0, 2, IPC::Message::PRIORITY_NORMAL);
  message->WriteInt(value);
  sender->Send(message);
}


class ListenerThatExpectsOK : public IPC::Listener {
 public:
  explicit ListenerThatExpectsOK(base::OnceClosure quit_closure)
      : received_ok_(false), quit_closure_(std::move(quit_closure)) {}

  ~ListenerThatExpectsOK() override = default;

  bool OnMessageReceived(const IPC::Message& message) override {
    base::PickleIterator iter(message);
    std::string should_be_ok;
    iter.ReadString(&should_be_ok);
    received_ok_ = true;
    std::move(quit_closure_).Run();
    return true;
  }

  void OnChannelError() override {
    // The connection should be healthy while the listener is waiting
    // message.  An error can occur after that because the peer
    // process dies.
    CHECK(received_ok_);
  }

  static void SendOK(IPC::Sender* sender) { SendString(sender, "OK"); }

 private:
  bool received_ok_;
  base::OnceClosure quit_closure_;
};


class TestListenerBase : public IPC::Listener {
 public:
  explicit TestListenerBase(base::OnceClosure quit_closure)
      : quit_closure_(std::move(quit_closure)) {}

  ~TestListenerBase() override = default;
  void OnChannelError() override { RunQuitClosure(); }

  void set_sender(IPC::Sender* sender) { sender_ = sender; }
  IPC::Sender* sender() const { return sender_; }
  void RunQuitClosure() {
    if (quit_closure_)
      std::move(quit_closure_).Run();
  }

 private:
  IPC::Sender* sender_ = nullptr;
  base::OnceClosure quit_closure_;
};


class PowerMonitorTestObserver : public base::PowerObserver {
 public:
  // PowerObserver callbacks.
  void OnPowerStateChange(bool on_battery_power) override {
    LOG(INFO) << "OnPowerStateChange";
  }
  void OnSuspend() override { 
    LOG(INFO) << "OnSuspend";
  }
  void OnResume() override { LOG(INFO) << "OnResume"; }
};  // namespace base


class WinMsgFilter : public base::MessagePumpForUI::Observer {

public:
  virtual void WillDispatchMSG(const MSG& msg) override {
   
    if (msg.message == WM_CLOSE) {
     LOG(INFO) << "WillDispatchMSG: WM_CLOSE" ;
    }
  }

  virtual void DidDispatchMSG(const MSG& msg) override {
    
  }
};


int APIENTRY
wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
         _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: 在此处放置代码。

  // 初始化全局字符串
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);
  

  const char* length_cases[] = {
      // One with everything in it.
      "http://user:pass@host:99/foo?bar#baz",
      // One with nothing in it.
      "",
      // Working backwards, let's start taking off stuff from the full one.
      "http://user:pass@host:99/foo?bar#",
      "http://user:pass@host:99/foo?bar",
      "http://user:pass@host:99/foo?",
      "http://user:pass@host:99/foo",
      "http://user:pass@host:99/",
      "http://user:pass@host:99",
      "http://user:pass@host:",
      "http://user:pass@host",
      "http://host",
      "http://user@",
      "http:",
  };
  for (size_t i = 0; i < base::size(length_cases); i++) {
    int true_length = static_cast<int>(strlen(length_cases[i]));

    url::Parsed parsed;
    url::ParseStandardURL(length_cases[i], true_length, &parsed);

    
  }

  const char kStr1[] = "http://www.com/";
  bool b = url::FindAndCompareScheme(kStr1, static_cast<int>(strlen(kStr1)),
                                   "http", NULL);
  const char kHTTPScheme[] = "http";

  url::SchemeType scheme_type;
  b = url::GetStandardSchemeType(
      kHTTPScheme, url::Component(0, strlen(kHTTPScheme)), &scheme_type);

  // 执行应用程序初始化:
  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }

  LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));



  base::CommandLine::Init(0, nullptr);
  
  logging::LoggingSettings settings;
  settings.logging_dest =
      logging::LOG_TO_SYSTEM_DEBUG_LOG | logging::LOG_TO_STDERR;
  logging::InitLogging(settings);
  logging::SetLogItems(true, true, true, true);

  base::SingleThreadTaskExecutor task_executor(base::MessagePumpType::UI);
  base::RunLoop run_loop;
 
  base::PlatformThread::SetName("maintest");
  /*base::MessageLoopCurrentForUI::Get()->AddMessagePumpObserver(
      new WinMsgFilter);*/


  base::PowerMonitor::Initialize(
      std::make_unique<base::PowerMonitorDeviceSource>());
  base::PowerMonitor::AddObserver(new PowerMonitorTestObserver);

  mojo::core::Init();


  std::unique_ptr<IPC::Channel> channel = IPC::ChannelMojo::Create(
      mojo::ScopedMessagePipeHandle(), IPC::Channel::MODE_SERVER,
                               nullptr, base::ThreadTaskRunnerHandle::Get(),
                               base::ThreadTaskRunnerHandle::Get(), nullptr);

  run_loop.Run();

  return 0;
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance;  // 将实例句柄存储在全局变量中

  HWND hWnd =
      CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                    0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  if (!hWnd) {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
    case WM_COMMAND: {
      int wmId = LOWORD(wParam);
      // 分析菜单选择:
      switch (wmId) {
        case IDM_ABOUT:
          DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
          break;
        case IDM_EXIT:
          DestroyWindow(hWnd);
          break;
        default:
          return DefWindowProc(hWnd, message, wParam, lParam);
      }
    } break;
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);
      // TODO: 在此处添加使用 hdc 的任何绘图代码...
      EndPaint(hWnd, &ps);
    } break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
    case WM_INITDIALOG:
      return (INT_PTR)TRUE;

    case WM_COMMAND:
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
      }
      break;
  }
  return (INT_PTR)FALSE;
}

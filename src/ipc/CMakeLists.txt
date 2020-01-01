project (ipc)
ADD_DEFINITIONS(-DNOMINMAX)
if (WIN32)  
ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN -DUNICODE -D_UNICODE)
endif(WIN32)

set (CMAKE_CXX_STANDARD 14)

if(MSVC)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MDd")
endif()

INCLUDE_DIRECTORIES (. ..)

FILE (GLOB source_files  "ipc_message_utils.cc"
    "ipc_message_utils.h"
    "ipc_mojo_param_traits.cc"
    "ipc_mojo_param_traits.h"
    "param_traits_log_macros.h"
    "param_traits_macros.h"
    "param_traits_read_macros.h"
    "param_traits_write_macros.h"
    "struct_constructor_macros.h"
	 "ipc_channel.h"
      "ipc_channel_common.cc"
      "ipc_channel_factory.cc"
      "ipc_channel_factory.h"
      "ipc_channel_handle.h"
      "ipc_channel_mojo.cc"
      "ipc_channel_mojo.h"
      "ipc_channel_proxy.cc"
      "ipc_channel_proxy.h"
      "ipc_channel_reader.cc"
      "ipc_channel_reader.h"
      "ipc_listener.h"
      "ipc_logging.cc"
      "ipc_logging.h"
      "ipc_message_macros.h"
      "ipc_message_pipe_reader.cc"
      "ipc_message_pipe_reader.h"
      "ipc_message_start.h"
      "ipc_message_templates.h"
      "ipc_message_templates_impl.h"
      "ipc_mojo_bootstrap.cc"
      "ipc_mojo_bootstrap.h"
      "ipc_sender.h"
      "ipc_sync_channel.cc"
      "ipc_sync_channel.h"
      "ipc_sync_message_filter.cc"
      "ipc_sync_message_filter.h"
      "message_filter.cc"
      "message_filter.h"
      "message_filter_router.cc"
      "message_filter_router.h"
      "message_router.cc"
      "message_router.h"
	  ipc_channel.cc
	  ipc_message_protobuf_utils.h)



FILE (GLOB message_support  "handle_attachment_win.cc"
    "handle_attachment_win.h"
    "handle_win.cc"
    "handle_win.h"
    "ipc_message.cc"
    "ipc_message.h"
    "ipc_message_attachment.cc"
    "ipc_message_attachment.h"
    "ipc_message_attachment_set.cc"
    "ipc_message_attachment_set.h"
    "ipc_message_support_export.h"
    "ipc_mojo_handle_attachment.cc"
    "ipc_mojo_handle_attachment.h"
    "ipc_mojo_message_helper.cc"
    "ipc_mojo_message_helper.h"
    "ipc_platform_file.cc"
    "ipc_platform_file.h"
    "ipc_sync_message.cc"
    "ipc_sync_message.h"
    "mach_port_attachment_mac.cc"
    "mach_port_attachment_mac.h"
    "mach_port_mac.cc"
    "mach_port_mac.h")
SOURCE_GROUP("message_support" FILES ${message_support})

FILE (GLOB native_handle_type_converters "native_handle_type_converters.cc"
    "native_handle_type_converters.h")
SOURCE_GROUP("native_handle_type_converters" FILES ${native_handle_type_converters})


if (WIN32)  
  

elseif (IOS)


elseif (OSX)


elseif (ANDROID)

endif(WIN32)
 
ADD_LIBRARY (ipc ${source_files} ${message_support} ${native_handle_type_converters})

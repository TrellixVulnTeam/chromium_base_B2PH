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


FILE (GLOB header_files *.h)
FILE (GLOB source_files *.cc)
#移除单元测试
file(GLOB source_to_remove *unittest*)
list(REMOVE_ITEM source_files ${source_to_remove})
list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/ipc_cpu_perftest.cc")
list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/run_all_perftests.cc")
list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/ipc_send_fds_test.cc")
list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/ipc_security_test_util.cc")


if (WIN32)  
    list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/handle_attachment_fuchsia.cc")
	list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/handle_attachment_fuchsia.h")
	list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/mach_port_attachment_mac.h")
	list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/mach_port_attachment_mac.cc")
	list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/mach_port_mac.h")
	list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/mach_port_mac.cc")
	list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/ipc_platform_file_attachment_posix.cc")
	

elseif (IOS)


elseif (OSX)


elseif (ANDROID)

endif(WIN32)
 
ADD_LIBRARY (ipc ${header_files} ${source_files})

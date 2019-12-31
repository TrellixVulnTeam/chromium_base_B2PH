project (net)
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

#base 
FILE (GLOB base base/*.h base/*.cc)
SOURCE_GROUP("base" FILES ${base})
#移除单元测试
file(GLOB base_to_remove base/*unittest*)
list(REMOVE_ITEM base ${base_to_remove})
list(REMOVE_ITEM base "${CMAKE_CURRENT_SOURCE_DIR}/base/mime_sniffer_perftest.cc")
list(REMOVE_ITEM base "${CMAKE_CURRENT_SOURCE_DIR}/base/interval_set_test.cc")
list(REMOVE_ITEM base "${CMAKE_CURRENT_SOURCE_DIR}/base/interval_test.cc")
list(REMOVE_ITEM base "${CMAKE_CURRENT_SOURCE_DIR}/base/load_timing_info_test_util.cc")
list(REMOVE_ITEM base "${CMAKE_CURRENT_SOURCE_DIR}/base/load_timing_info_test_util.h")
list(REMOVE_ITEM base "${CMAKE_CURRENT_SOURCE_DIR}/base/hex_utils_test.cc")
list(REMOVE_ITEM base "${CMAKE_CURRENT_SOURCE_DIR}/base/fuzzer_test_support.cc")
list(REMOVE_ITEM base "${CMAKE_CURRENT_SOURCE_DIR}/base/data_url_fuzzer.cc")
list(REMOVE_ITEM base "${CMAKE_CURRENT_SOURCE_DIR}/base/directory_listing.cc")
list(REMOVE_ITEM base "${CMAKE_CURRENT_SOURCE_DIR}/base/directory_lister.cc")



if (WIN32)  
     # base 移除其他平台
     file(GLOB_RECURSE base_posix_to_remove *_posix.*)
	  list(REMOVE_ITEM base ${base_posix_to_remove})
	   file(GLOB_RECURSE base_mac_to_remove *_mac.*)
	  list(REMOVE_ITEM base ${base_mac_to_remove})
	  file(GLOB_RECURSE base_ios_to_remove *_ios.*)
	  list(REMOVE_ITEM base ${base_ios_to_remove})
	   file(GLOB_RECURSE base_linux_to_remove *_linux.*)
	  list(REMOVE_ITEM base ${base_linux_to_remove})
	  file(GLOB_RECURSE base_fuchsia_to_remove *_fuchsia.*)
	  list(REMOVE_ITEM base ${base_fuchsia_to_remove})
	  file(GLOB_RECURSE base_android_to_remove *_android.*)
	  list(REMOVE_ITEM base ${base_android_to_remove})
	   list(REMOVE_ITEM base "${CMAKE_CURRENT_SOURCE_DIR}/base/address_tracker_linux_fuzzer.cc")
	

elseif (IOS)


elseif (OSX)


elseif (ANDROID)

endif(WIN32)
 
ADD_LIBRARY (net ${base})

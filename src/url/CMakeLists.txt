project (url)
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

INCLUDE_DIRECTORIES (. .. ../base/third_party/icu/source/common)


FILE (GLOB header_files *.h)
FILE (GLOB source_files *.cc)
#移除单元测试
file(GLOB source_to_remove *unittest*)
list(REMOVE_ITEM source_files ${source_to_remove})
list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/url_parse_perftest.cc")
list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/run_all_perftests.cc")



#mozilla
FILE (GLOB third_party_mozilla third_party/mozilla/*.h third_party/mozilla/*.cc)
SOURCE_GROUP("third_party\\mozilla" FILES ${third_party_mozilla})


if (WIN32)  	
    list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/url_idna_icu_alternatives_android.cc")




elseif (IOS)


elseif (OSX)


elseif (ANDROID)

endif(WIN32)
 
ADD_LIBRARY (url ${header_files} ${source_files} ${ipc} ${third_party_mozilla})

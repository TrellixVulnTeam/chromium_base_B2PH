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

FILE (GLOB source_files  "gurl.cc"
    "gurl.h"
    "origin.cc"
    "origin.h"
    "scheme_host_port.cc"
    "scheme_host_port.h"
    "third_party/mozilla/url_parse.cc"
    "third_party/mozilla/url_parse.h"
    "url_canon.cc"
    "url_canon.h"
    "url_canon_etc.cc"
    "url_canon_filesystemurl.cc"
    "url_canon_fileurl.cc"
    "url_canon_host.cc"
    "url_canon_internal.cc"
    "url_canon_internal.h"
    "url_canon_internal_file.h"
    "url_canon_ip.cc"
    "url_canon_ip.h"
    "url_canon_mailtourl.cc"
    "url_canon_path.cc"
    "url_canon_pathurl.cc"
    "url_canon_query.cc"
    "url_canon_relative.cc"
    "url_canon_stdstring.cc"
    "url_canon_stdstring.h"
    "url_canon_stdurl.cc"
    "url_constants.cc"
    "url_constants.h"
    "url_file.h"
    "url_parse_file.cc"
    "url_parse_internal.h"
    "url_util.cc"
    "url_util.h"
    "url_util_internal.h"
     "url_canon_icu.cc"
      "url_canon_icu.h"
      "url_idna_icu.cc")


if (WIN32)  	
    




elseif (IOS)


elseif (OSX)


elseif (ANDROID)

endif(WIN32)
 
ADD_LIBRARY (url ${source_files})

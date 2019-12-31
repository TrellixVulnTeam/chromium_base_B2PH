project (ced)
if (WIN32)  
ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN -DUNICODE -D_UNICODE)
endif(WIN32)

set (CMAKE_CXX_STANDARD 14)

if(MSVC)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD ")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd ")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MDd ")


endif()

add_compile_options(-Wno-narrowing)
INCLUDE_DIRECTORIES (. .. src/compact_enc_det)

FILE (GLOB source_files  "src/compact_enc_det/compact_enc_det.cc"
    "src/compact_enc_det/compact_enc_det_generated_tables.h"
    "src/compact_enc_det/compact_enc_det_generated_tables2.h"
    "src/compact_enc_det/compact_enc_det_hint_code.cc"
    "src/compact_enc_det/compact_enc_det_hint_code.h"
    "src/compact_enc_det/util/basictypes.h"
    "src/compact_enc_det/util/case_insensitive_hash.h"
    "src/compact_enc_det/util/commandlineflags.h"
    "src/compact_enc_det/util/encodings/encodings.cc"
    "src/compact_enc_det/util/encodings/encodings.h"
    "src/compact_enc_det/util/encodings/encodings.pb.h"
    "src/compact_enc_det/util/languages/languages.cc"
    "src/compact_enc_det/util/languages/languages.h"
    "src/compact_enc_det/util/languages/languages.pb.h"
    "src/compact_enc_det/util/logging.h"
    "src/compact_enc_det/util/port.h"
    "src/compact_enc_det/util/string_util.h"
    "src/compact_enc_det/util/varsetter.h"
    "src/compact_enc_det/compact_enc_det.h")
ADD_LIBRARY (ced ${source_files})
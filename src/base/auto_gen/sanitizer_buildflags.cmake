
set(sanitizer_buildflags ${CMAKE_CURRENT_SOURCE_DIR}/sanitizer_buildflags.h)
file(WRITE ${sanitizer_buildflags} "#pragma once\n")
file(APPEND ${sanitizer_buildflags} "#define IS_HWASAN 0\n")

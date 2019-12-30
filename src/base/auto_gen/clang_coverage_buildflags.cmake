
set(clang_coverage_buildflags ${CMAKE_CURRENT_SOURCE_DIR}/clang_coverage_buildflags.h)
file(WRITE ${clang_coverage_buildflags} "#pragma once\n")
file(APPEND ${clang_coverage_buildflags} "#define CLANG_COVERAGE 0\n")


set(allocator_buildflags ${CMAKE_CURRENT_SOURCE_DIR}/allocator/buildflags.h)
file(WRITE ${allocator_buildflags} "#pragma once\n")
file(APPEND ${allocator_buildflags} "#define USE_ALLOCATOR_SHIM 0\n")
file(APPEND ${allocator_buildflags} "#define USE_TCMALLOC 0\n")

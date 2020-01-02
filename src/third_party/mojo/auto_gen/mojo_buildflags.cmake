
set(mojo_buildflags ${CMAKE_CURRENT_SOURCE_DIR}/public/cpp/bindings/mojo_buildflags.h)
file(WRITE ${mojo_buildflags} "#pragma once\n")
file(APPEND ${mojo_buildflags} "#define MOJO_TRACE_ENABLED 1\n")

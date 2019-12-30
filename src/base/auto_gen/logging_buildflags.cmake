
set(debugging_buildflags ${CMAKE_CURRENT_SOURCE_DIR}/logging_buildflags.h)
file(WRITE ${debugging_buildflags} "#pragma once\n")
file(APPEND ${debugging_buildflags} "#define ENABLE_LOG_ERROR_NOT_REACHED 1\n")
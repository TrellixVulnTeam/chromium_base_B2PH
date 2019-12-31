
set(net_buildflags ${CMAKE_CURRENT_SOURCE_DIR}/net_buildflags.h)
file(WRITE ${net_buildflags} "#pragma once\n")
file(APPEND ${net_buildflags} "#define POSIX_AVOID_MMAP 0\n")
file(APPEND ${net_buildflags} "#define DISABLE_FILE_SUPPORT 1\n")
file(APPEND ${net_buildflags} "#define DISABLE_FTP_SUPPORT 1\n")

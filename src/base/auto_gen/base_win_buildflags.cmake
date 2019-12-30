
set(base_win_buildflags ${CMAKE_CURRENT_SOURCE_DIR}/win/base_win_buildflags.h)
file(WRITE ${base_win_buildflags} "#pragma once\n")
file(APPEND ${base_win_buildflags} "#define SINGLE_MODULE_MODE_HANDLE_VERIFIER 0\n")

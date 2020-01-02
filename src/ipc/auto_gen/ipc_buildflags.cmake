
set(ipc_buildflags ${CMAKE_CURRENT_SOURCE_DIR}/ipc_buildflags.h)
file(WRITE ${ipc_buildflags} "#pragma once\n")
file(APPEND ${ipc_buildflags} "#define BUILDFLAG(x) x\n")
file(APPEND ${ipc_buildflags} "#define IPC_MESSAGE_LOG_ENABLED 0\n")

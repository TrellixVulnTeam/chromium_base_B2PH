
set(partition_alloc_buildflags ${CMAKE_CURRENT_SOURCE_DIR}/partition_alloc_buildflags.h)
file(WRITE ${partition_alloc_buildflags} "#pragma once\n")
file(APPEND ${partition_alloc_buildflags} "#define USE_PARTITION_ALLOC 1\n")

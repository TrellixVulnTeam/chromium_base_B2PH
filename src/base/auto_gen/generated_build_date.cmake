
set(generated_build_date ${CMAKE_CURRENT_SOURCE_DIR}/generated_build_date.h)
file(WRITE ${generated_build_date} "#pragma once\n")
file(APPEND ${generated_build_date} "#define BUILD_DATE \"05:00:00\"\n")

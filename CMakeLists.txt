#
project(test_base)
cmake_minimum_required(VERSION 3.2)

SET (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/build/CMakeModules/;${CMAKE_MODULE_PATH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin)

add_subdirectory(src/third_party/boringssl/src boringssl)
add_subdirectory(src/base)
#add_subdirectory(src/net)
add_subdirectory(src/url)
add_subdirectory(src/ipc)
add_subdirectory(src/third_party/ced)
add_subdirectory(src/third_party/protobuf)
add_subdirectory(src/client)












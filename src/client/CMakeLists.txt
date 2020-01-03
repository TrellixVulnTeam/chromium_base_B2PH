project (client)
ADD_DEFINITIONS(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)
INCLUDE_DIRECTORIES (. .. ../third_party/protobuf/src ../third_party)
FILE (GLOB header_files *.h)
FILE (GLOB source_files *.c *.cpp *.cc)


ADD_EXECUTABLE (client WIN32 WindowsProject1.rc ${header_files} ${source_files})
TARGET_LINK_LIBRARIES (client base crcrypto url ipc mojo crypto protobuf ssl wbemuuid version Dbghelp SetupAPI Winmm Powrprof ws2_32 Propsys Shlwapi delayimp)

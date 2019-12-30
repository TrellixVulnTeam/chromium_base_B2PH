@echo off

set PATH=%PATH%;../build/cmake/windows/bin
cd ..
mkdir tmp_3264_pc

cd tmp_3264_pc
cmake .. -G"Visual Studio 16 2019" -A Win32 -DCMAKE_GENERATOR_TOOLSET=ClangCL  -DCMAKE_CONFIGURATION_TYPES=Debug;Release;Release_MD
pause
@echo on

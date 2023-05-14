echo off
cls
echo "---------- [CMake | build | Release | Clang] -----------"
SET CC=clang
SET CXX=clang++
cmake -S . -B build_clang -G "MinGW Makefiles"
cmake --build build_clang --config Release --target cpp_terminal_tryout

echo off
cls
echo "---------- [CMake | build | Release | GCC | MinGW Makefiles] -----------"
SET CC=gcc
SET CXX=g++
cmake -S . -B build_gcc -G "MinGW Makefiles"  
cmake --build build_gcc --config Release --target cpp_terminal_tryout
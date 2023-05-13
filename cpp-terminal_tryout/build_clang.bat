echo off
cls
echo "---------- [CMake | build | Release | Clang | Ninja] -----------"
SET CC=C:\Program Files\LLVM\bin\clang
SET CXX=C:\Program Files\LLVM\bin\clang++
cmake -S . -B build_clang -G "Ninja"  
cmake --build build_clang --config Release --target cpp_terminal_tryout

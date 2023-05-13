echo off
cls
echo "---------- [CMake | build | Visual Studio 17 2022 | Release] -----------"
cmake -S . -B build -G "Visual Studio 17 2022" 
cmake --build build --config Release --target cpp_terminal_tryout

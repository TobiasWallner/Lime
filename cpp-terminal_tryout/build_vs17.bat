echo off
cls
echo "---------- [CMake | build | Release | MSVC | Visual Studio 17 2022] -----------"
cmake -S . -B build_VS17 -G "Visual Studio 17 2022"  
cmake --build build_VS17 --config Release --target cpp_terminal_tryout
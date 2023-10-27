# Lime
Lime is a modern and fast editor that works in the command line.

![image](https://github.com/TobiasWallner/Lime/assets/70602844/e7c876d4-5db2-4254-a2f7-d79c470e0ee2)

# Lime - The command line editor

Lime is a text editor that works directly in the terminal. It aims to take features of some of the best and most popular editors out there.  

## Usage

### download source

```
git clone --recurse-submodules https://github.com/TobiasWallner/Lime.git
```

last stable commit of the cpp-terminal library: 80d1dd8d427b9ffba0d5cc498f147ff0eda4535b

after cloning this repo. Go into the cpp-terminal library and checkout the commit above.

### Complilation on Windows

**with MSVC:**
```
cmake -S . -B build_vs17 -G "Visual Studio 17 2022"  
cmake --build build_vs17 --config Release --target lime
```
We compiled with VS17 but you may use a newer build generator as you like.

**with Clang**
The Developers of this repository prefere to build with Clang and Ninja like shown below.
```
SET CC=clang
SET CXX=clang++
cmake -S . -B build_clang -G "Ninja Multi-Config"
cmake --build build_clang --config Release --target lime
```

**with GCC flavoured by MSYS2**
```
SET CC=gcc
SET CXX=g++
cmake -S . -B build_gcc -G "Ninja Multi-Config"
cmake --build build_gcc --config Release --target lime
```

#### How to install CMake
You can get CMake from here [https://cmake.org/download/](https://cmake.org/download/)

#### How to install Visual Studio
1) Download Visual Studio from the microsoft omepage [https://visualstudio.microsoft.com/](https://visualstudio.microsoft.com/)
2) Install the programm
3) open the launcher, select C++ for Desktop and install all files.

#### How to install Clang
1) Download the release that fits your platform here [https://github.com/llvm/llvm-project/releases](https://github.com/llvm/llvm-project/releases) or compile the git repository yourself.
2) Add the binary folder that contains `clang` and `clang++` to the PATH variables.

#### How to install GCC with MSYS2**
1) Download MSYS2 from here [https://www.msys2.org/](https://www.msys2.org/) and follow the installation prozess
2) In the MSYS2 terminal install the following `pacman -S mingw-w64-ucrt-x86_64-gcc`
3) Add the gcc compiler to your PATH, it should be located somwhere at `...\msys64\ucrt64\bin`.

#### How to install Ninja
1) Install it by downloading binaries or cloning the repo and compileing yourself from here `https://ninja-build.org/`
2) Copy the `ninja.exe` executable to wherever you want and set a PATH variable to its location.


### Compilation on Linux

**with Clang**
with Clang and Ninja as a build tool generator but you may use any other generator as well:
```
export CC=clang
export CXX=clang++
cmake -S . -B build_clang -G "Ninja Multi-Config"
cmake --build build_clang --config Release --target lime
```

**with GCC flavoured by MSYS2**
```
export CC=gcc
export CXX=g++
cmake -S . -B build_gcc -G "Ninja Multi-Config"
cmake --build build_gcc --config Release --target lime
```

### Testing

This Project supports CMake tests using `ctest`.
In order to run the tests build the project with the flag `--target all`.
For example:
```
cmake --build build_clang --config Release --target all
```

then go into the build directory like for example with `cd build_clang` and run the command `ctest`.
CMake will then run all tests of this project. 
Test of external projects will be turned off and not executed.

Note that MSVC does not have a `--target all`. There if you just remove the `--target ...` command as a whole Visual Studio will build all test executables.

The Lime editor is compileing, tested and running successfully on the following machines and compilers:
+ The Windows Build:  
	Windows 10: Version 22H2
	MSYS2 GCC: gcc (Rev10, Built by MSYS2 project) 12.2.0  
	Clang: version 16.0.3, Target: x86_64-pc-windows-msvc, Thread model: posix  
	MSVC: Visual Studio 17 2022  version: 19.35.32217.1
	
+ The Linux Build:
	Debian 11
	GCC
		

## Trouble Shooting

If you use Clang on windows you will need some additional libraries that will have to be linked to your project in order to access the Windows Console API.
You can easily get those by installing Visual Studio (Community Eddition is enough) and then install the C++ Desktop Developement in the Visual Studio Package Manager (not to confuse wih vcpack).

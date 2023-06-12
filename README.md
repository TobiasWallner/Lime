# Lime
Lime is a modern and fast editor that works in the command line.

# Lime - The command line editor

Lime is a text editor that works directly in the terminal. It aims to take features of some of the best and most popular editors out there.  

 **Vim / NeoVim** Is a very powerful keyboard-only text editor that proviedes a lot of different ways to edit and navigate a text file. Thus it repurposes the same keys and key combinations with different modes like the *insert-mode*, *normal-mode*, *visual-mode* and more. Lime aims to take the idea of being a keyboard-only editor that can look and feel like an IDE without the need to switch between different modes all the time. NeoVim inspired the integration of a file tree and project viewer as well as console passthrough. 
 
 **Emacs** Emacs was a ground breaking editor for its time and still has many lovers today, especially for its customisations. Lime takes this as an inspiration to configurate the colours and appearance of the editor as well as syntax highlighting for the extfile. 
 
 **nano** nano is a very small and light weight editor that helps its useres by showing them the commands and shortcuts at hand. Many of its shortcuts are still used today like Ctrl-S to save and Ctrl-F to find. Modern Software has advanced and uses many more key-shortcuts and features that mostly everyone intuetively knows. Lime will take this mindset and advances it to give the user the modern intuitive experience and smoothly guides the user.  
 
 **Computer games** use the keys 'W'-up, 'A'-left, 'S'-down, 'D'-right for movement and navigation. Lime is inspired to use those or the same layout for the other hand with 'I'-up, 'J'-left, 'K'-down, 'L'-right for navigation as well.

## Usage

### download source

```
git clone --recurse-submodules https://stea.iue.tuwien.ac.at/CppProjects2023/GroupIProject
```

last stable commit of the cpp-terminal library: 49e3160358237a3af25e7c7c3e70e593812cd0fe

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

## Team
- Rafał Górniak 12232999
- Sondre Hatlehol 12202296
- Helena Schneider 11812332
- Stefan Schüttengruber 11807476
- Tobias Wallner 11775206 (Team Lead)
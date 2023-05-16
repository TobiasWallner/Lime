# Lime
Lime is a modern and fast editor that works in the command line.

# Lime - The command line editor

Lime is a text editor that works directly in the terminal. It aims to take features of some of the best and most popular editors out there.  

 **Vim / NeoVim** Is a very powerful keyboard-only text editor that proviedes a lot of different ways to edit and navigate a text file. Thus it repurposes the same keys and key combinations with different modes like the *insert-mode*, *normal-mode*, *visual-mode* and more. Lime aims to take the idea of being a keyboard-only editor that can look and feel like an IDE without the need to switch between different modes all the time. NeoVim inspired the integration of a file tree and project viewer as well as console passthrough. 
 
 **Emacs** Emacs was a ground breaking editor for its time and still has many lovers today, especially for its customisations. Lime takes this as an inspiration to configurate the colours and appearance of the editor as well as syntax highlighting for the extfile. 
 
 **nano** nano is a very small and light weight editor that helps its useres by showing them the commands and shortcuts at hand. Many of its shortcuts are still used today like Ctrl-S to save and Ctrl-F to find. Modern Software has advanced and uses many more key-shortcuts and features that mostly everyone intuetively knows. Lime will take this mindset and advances it to give the user the modern intuitive experience and smoothly guides the user.  
 
 **Computer games** use the keys 'W'-up, 'A'-left, 'S'-down, 'D'-right for movement and navigation. Lime is inspired to use those or the same layout for the other hand with 'I'-up, 'J'-left, 'K'-down, 'L'-right for navigation as well.

## Usage

### Complilation on Windows

**with MSVC:**
```bash
cmake -S . -B build_vs17 -G "Visual Studio 17 2022"  
cmake --build build_vs17 --config Release --target cpp_terminal_tryout
```
We compiled with VS17 but you may use a newer build generator as you like.

**with Clang **


The Developers of this repository prefere to build with Clang and Ninja like shown below.
```bash
SET CC=clang
SET CXX=clang++
cmake -S . -B build_clang -G "Ninja Multi-Config"
cmake --build build_clang --config Release --target cpp_terminal_tryout
```

### Compilation on Linux

with Clang and Ninja as a build tool generator but you may use any other generator as well:
```bash
export CC=clang
export CXX=clang++
cmake -S . -B build_clang -G "Ninja Multi-Config"
cmake --build build_clang --config Release --target cpp_terminal_tryout
```

### Compiled output

The compiled output can then be found in the folder "build_clang/Release/".

## Trouble Shooting

If you use Clang on windows you will need some additional libraries that will have to be linked to your project in order to access the Windows Console API.
You can easily get those by installing Visual Studio (Community Eddition is enough) and then install the C++ Desktop Developement in the Visual Studio Package Manager (not to confuse wih vcpack).

## Team
- Rafał Górniak 12232999
- Sondre Hatlehol 12202296
- Helena Schneider 11812332
- Stefan Schüttengruber 11807476
- Tobias Wallner 11775206 (Team Lead)


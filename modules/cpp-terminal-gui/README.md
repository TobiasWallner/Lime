# CPP-Terminal-GUI

CPP-Terminal-GUI provides gui elements for the comamnd line so that you can 
quickly build your software project within the terminal.

# Usage

This library requires the library [**cpp-terminal**](https://github.com/jupyter-xeus/cpp-terminal).

The following instructions explains how to add this library to your project for a project configuration
where you do not install or build libraries ahead of time, but build everything always from source
with the same compiler options.
Why would you decide to do that? 
For example when you compile for multiple different CPU and OS architectures.

In your top level CMakeLists.txt:
first add the cpp-terminal library:
```
add_subdirectory("path/to/cpp-terminal" "modules/cpp-terminal_build")
```

Then add this library
```
add_subdirectory("path/to/cpp-terminal-gui" "modules/cpp-terminal-gui_build")
```

Then set the include directories, set the compile options you want and link the cpp-terminal library to cpp-terminal-gui
```
#
# Target settings for cpp-terminal-gui
#

target_include_directories(cpp-terminal-gui PUBLIC
	path/to/cpp-terminal
)

target_compile_options(cpp-terminal-gui PUBLIC
	${COMPILER_FLAGS}
)

target_link_libraries(cpp-terminal-gui PUBLIC
	cpp-terminal
)
```
If you know how to achieve the above within the `CMakeLists.txt` of this library, please let me know and write me a mail: tobias.wallner1@gmx.net
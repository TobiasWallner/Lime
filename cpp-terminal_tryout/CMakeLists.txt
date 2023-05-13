cmake_minimum_required(VERSION 3.25.2)

project(cpp_terminal_tryout)


# Basic compiler settings
set(CMAKE_CXX_STANDARD 17)


# dependencies
set(MODULES_PATH "../modules")

# cpp-terminal
add_subdirectory("${MODULES_PATH}/cpp-terminal" "${CMAKE_CURRENT_BINARY_DIR}/cpp-terminal_build")


# project building process: 
add_executable(cpp_terminal_tryout
	cpp_terminal_tryout.cpp
    )

target_include_directories(cpp_terminal_tryout PUBLIC
	${MODULES_PATH}/cpp-terminal

    )

target_compile_options(cpp_terminal_tryout PUBLIC
    
	/Wall # Displays all warnings displayed by /W4 and all other warnings that /W4 doesn't include—for example, warnings that are off by default.
	
	# set compiler flags for debugging
	$<$<CONFIG:Debug>:
		/Od # disable optimisations for debugging
		/ZI # produces a PDB file in a format that supports the Edit and Continue feature. To use Edit and Continue debugging features, you must use this option. 
    >
	
	# set compiler flags for release versions
	$<$<CONFIG:Release>:
		/O2 # Creates fast code
	>
    )

target_link_libraries(cpp_terminal_tryout
	cpp-terminal
    
    )




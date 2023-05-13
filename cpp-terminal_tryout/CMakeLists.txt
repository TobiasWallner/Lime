cmake_minimum_required(VERSION 3.25.2)

project(cpp_terminal_tryout)


# Basic compiler settings
set(CMAKE_CXX_STANDARD 17)
set(warnings "-Wall -Wextra -Werror")


# dependencies

set(MODULES_PATH "../modules")

# cpp-terminal
add_subdirectory("${MODULES_PATH}/cpp-terminal" "${CMAKE_CURRENT_BINARY_DIR}/cpp-terminal_build")


# project building process: 
add_executable(cpp_terminal_tryout
	cpp_terminal_tryout.cpp
    )

target_include_directories(cpp_terminal_tryout PUBLIC
	${MODULES_PATH}/cpp-terminal/cpp-terminal

    )

target_compile_options(cpp_terminal_tryout PUBLIC
    -Wall -Wextra -Werror # Enable all error messages
    
	# set compiler flags for debugging
	$<$<CONFIG:Debug>:
		
    
	# set compiler flags for release versions
	$<$<CONFIG:Release>:
		
	>
    )

target_link_libraries(cpp_terminal_tryout
	
    
    )




cmake_minimum_required(VERSION 3.22.0)

project(lime)


# Basic compiler settings
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)


# ----------- dependencies ------------
set(MODULES_PATH "modules")

add_subdirectory("${MODULES_PATH}/cpp-terminal" "modules/cpp-terminal_build")
add_subdirectory("${MODULES_PATH}/cpp-terminal-gui" "modules/cpp-terminal-gui_build")

# project building process: 
add_executable(lime
	main.cpp
	lime/lime.cpp	lime/lime.hpp
)

# Compiler Options for Clang:
# ===========================
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(COMPILER_FLAGS 
		
		# set compiler flags for debugging
		#---------------------------------
		$<$<CONFIG:Debug>:
		
			# Enable Optimisations that will not disturb debugging
			-O0
			-g
		>
		
		# set compiler flags for release
		#-------------------------------
		$<$<CONFIG:Release>:
			
			# Enable all Optimisations
			-O3 
		>
	)
# Compiler Options for GNU:
# =========================
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(COMPILER_FLAGS
		
		# Enable link time optimisations
		-flto
		
		# set compiler flags for debugging
		#---------------------------------
		$<$<CONFIG:Debug>:
		
			# Enable Optimisations that will not disturb debugging
			-O0
			-g
		>
		
		# set compiler flags for release
		#-------------------------------
		$<$<CONFIG:Release>:
			
			# Enable all Optimisations
			-O3 
		>
	)
# Compiler Options for MSVC:
# ==========================
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(COMPILER_FLAGS
		/W3
	
		# set compiler flags for debugging
		#---------------------------------
		$<$<CONFIG:Debug>:
		
			# disable optimisations for debugging
			/Od 
			
			# produces a PDB file in a format that supports the Edit and Continue feature. 
			# To use Edit and Continue debugging features, you must use this option. 
			/ZI 
		>
		
		# set compiler flags for release versions
		#----------------------------------------
		$<$<CONFIG:Release>:
			
			# Creates fast code
			/O2 
		>
	)
else()
	message( SEND_ERROR "The compiler you are using is not handeled. Add your compiler and compiler flags to the if(CMAKE_CXX_COMPILER_ID STREQUAL <your compiler>) in the CMakeLists.txt of the root folder")
endif()

#
# Target settings for cpp-terminal-gui
#

target_include_directories(cpp-terminal-gui PUBLIC
	${MODULES_PATH}/cpp-terminal
)

target_compile_options(cpp-terminal-gui PUBLIC
	${COMPILER_FLAGS}
)

target_link_libraries(cpp-terminal-gui PUBLIC
	cpp-terminal
)

#
# Target settings for lime
#

target_include_directories(lime PUBLIC
	${MODULES_PATH}/cpp-terminal
	${MODULES_PATH}/cpp-terminal-gui
)

target_compile_options(lime PUBLIC
	${COMPILER_FLAGS}
)

target_link_libraries(lime PUBLIC
	cpp-terminal
    cpp-terminal-gui
)




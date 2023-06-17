cmake_minimum_required(VERSION 3.18.0)

project(lime)

# Basic compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# ----------- dependencies ------------
set(EXTERNAL_MODULES_PATH "external")
set(INTERNAL_MODULES_PATH "modules")

# external dependencies
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW) # allow to override variables within the CMakeLists.txt of the library
set(CPPTERMINAL_BUILD_EXAMPLES OFF) # disable building examples
set(CPPTERMINAL_ENABLE_INSTALL OFF) # disable the installation of the library
set(CPPTERMINAL_ENABLE_TESTING OFF) # disable the tests of the library ... when running 'ctest' we only want to see our own tests
add_subdirectory("${EXTERNAL_MODULES_PATH}/cpp-terminal")

# internal dependencies
enable_testing() # enable testing for internal libraries
add_subdirectory("${INTERNAL_MODULES_PATH}/utf8_string")
add_subdirectory("${INTERNAL_MODULES_PATH}/cpp-terminal-gui")
add_subdirectory("${INTERNAL_MODULES_PATH}/verification")

# ---------- unix specific libraries ------------

set(UNIX_LIBRARIES "")

if (UNIX)
	find_package(X11 REQUIRED)
	set(UNIX_LIBRARIES "X11")
endif()


# -------------------------------------------------

# project building process: 
add_executable(lime
	main.cpp
	
	# lime
	lime/Lime.cpp				lime/Lime.hpp
)

# Compiler Options for Clang:
# ===========================
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(COMPILER_FLAGS 
		
		# set compiler flags for debugging
		#---------------------------------
		$<$<CONFIG:Debug>:
			-O0	# disable optimisations
			-g	# enable debug information
		>
		
		# set compiler flags for release
		#-------------------------------
		$<$<CONFIG:Release>:
			-O3 # Enable all Optimisations
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
			-O0	# disable optimisations
			-g	# enable debug information
			-ggdb # extra flag for the debugger
		>
		
		# set compiler flags for release
		#-------------------------------
		$<$<CONFIG:Release>:
			-O3 # Enable all Optimisations
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
# Target settings for lime
#

target_compile_options(lime PUBLIC
	${COMPILER_FLAGS}
)


target_link_libraries(lime PUBLIC
	utf8_string
	cpp-terminal
	cpp-terminal-gui
	# ----- linux specific libraries -----
	${UNIX_LIBRARIES}
)


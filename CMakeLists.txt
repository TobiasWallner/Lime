cmake_minimum_required(VERSION 3.22.0)

project(Lime)


# Basic compiler settings
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)


# dependencies
set(MODULES_PATH "modules")

# cpp-terminal
add_subdirectory("${MODULES_PATH}/cpp-terminal" "${MODULES_PATH}/cpp-terminal_build")


# project building process: 
add_executable(Lime
	main.cpp
    )

target_include_directories(Lime PUBLIC
	${MODULES_PATH}/cpp-terminal

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
	message( SEND_ERROR "The compiler you are using is not handeled. Add your compiler and compiler flags to the if(CMAKE_CXX_COMPILER_ID STREQUAL <your compiler>) in the CMakeLists.txt")
endif()


target_compile_options(Lime PUBLIC

	${COMPILER_FLAGS}
	
    )

target_link_libraries(Lime
	cpp-terminal
    
    )




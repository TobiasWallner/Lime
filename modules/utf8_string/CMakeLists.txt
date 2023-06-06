
add_subdirectory(test)

add_library(utf8_string INTERFACE)

target_include_directories(utf8_string INTERFACE 
	${CMAKE_CURRENT_SOURCE_DIR}
)

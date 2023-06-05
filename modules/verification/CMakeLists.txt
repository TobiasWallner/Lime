add_library(verification STATIC
verification.cpp
verification.hpp
)

target_include_directories(verification INTERFACE 
${CMAKE_CURRENT_SOURCE_DIR}
)

enable_testing()
add_subdirectory(test)


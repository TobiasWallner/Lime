add_executable(verification_test
	verification_test.cpp
)

target_link_libraries(verification_test verification)

add_test(verification_ctest verification_test)



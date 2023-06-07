
# ------------ Char ----------------
add_executable(utf8_char_test
	test_utf8_char.cpp
)

target_link_libraries(utf8_char_test PUBLIC
					utf8_string
					verification)

add_test(utf8_char_ctest utf8_char_test)


# ------------- String ----------------

add_executable(utf8_string_test
	test_utf8_string.cpp
)

target_link_libraries(utf8_string_test PUBLIC
					utf8_string
					verification)

add_test(utf8_string_ctest utf8_string_test)

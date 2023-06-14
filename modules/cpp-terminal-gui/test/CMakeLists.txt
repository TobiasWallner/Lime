
# ------------------- CommandList Tests ----------------------

add_executable(iStylesList_test iStylesList_test.cpp)

target_link_libraries(iStylesList_test
	cpp-terminal-gui
)

add_test(iStylesList_ctest iStylesList_test)


# ------------------- ColoredString Tests ----------------------

add_executable(ColoredString_test ColoredString_test.cpp)

target_link_libraries(ColoredString_test PUBLIC
	cpp-terminal-gui
	verification
)

add_test(ColoredString_ctest ColoredString_test)


# ------------------- Text Editor Tests ----------------------

add_executable(TextEditor_test TextEditor_test.cpp)

target_link_libraries(TextEditor_test
	cpp-terminal-gui
)

add_test(TextEditor_ctest TextEditor_test)

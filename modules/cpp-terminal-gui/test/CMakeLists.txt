
# ------------------- CommandList Tests ----------------------

add_executable(cpp_terminal_gui_istyleslist_test iStylesList_test.cpp)

target_link_libraries(cpp_terminal_gui_istyleslist_test
	cpp-terminal-gui
)

add_test(cpp_terminal_gui_istyleslist_ctest cpp_terminal_gui_istyleslist_test)


# ------------------- ColoredString Tests ----------------------

add_executable(cpp_terminal_gui_colored_string_test ColoredString_test.cpp)

target_link_libraries(cpp_terminal_gui_colored_string_test PUBLIC
	cpp-terminal-gui
	verification
)

add_test(cpp_terminal_gui_colored_string_ctest cpp_terminal_gui_colored_string_test)


# ------------------- Text Editor Tests ----------------------

add_executable(cpp_terminal_gui_text_editor_test TextEditor_test.cpp)

target_link_libraries(cpp_terminal_gui_text_editor_test
	cpp-terminal-gui
)

add_test(cpp_terminal_gui_text_editor_ctest cpp_terminal_gui_text_editor_test)
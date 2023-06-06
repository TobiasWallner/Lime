
# ------------------- CommandList Tests ----------------------

add_executable(cpp_terminal_gui_command_list_test CommandList_test.cpp)

target_link_libraries(cpp_terminal_gui_command_list_test
	cpp-terminal-gui
)

add_test(cpp_terminal_gui_command_list_ctest cpp_terminal_gui_command_list_test)


# ------------------- ColoredString Tests ----------------------

add_executable(cpp_terminal_gui_colored_string_test ColoredString_test.cpp)

target_link_libraries(cpp_terminal_gui_colored_string_test
	cpp-terminal-gui
)

add_test(cpp_terminal_gui_colored_string_ctest cpp_terminal_gui_colored_string_test)


# ------------------- Text Editor Tests ----------------------

add_executable(cpp_terminal_gui_text_editor_test TextEditor_test.cpp)

target_link_libraries(cpp_terminal_gui_text_editor_test
	cpp-terminal-gui
)

add_test(cpp_terminal_gui_text_editor_ctest cpp_terminal_gui_text_editor_test)
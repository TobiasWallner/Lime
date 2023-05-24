
# ------------------- CommandList Tests ----------------------

add_executable(cpp_terminal_gui_command_list_test CommandList_test.cpp)

target_link_libraries(cpp_terminal_gui_command_list_test
	cpp-terminal-gui
)

add_test(cpp_terminal_gui_command_list_ctest cpp_terminal_gui_command_list_test)
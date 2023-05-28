

#include <cpp-terminal-gui\TextEditor.hpp>

// C std
#include <stdlib.h>

// C++ std
#include <iostream>
#include <algorithm>

#define assert(condition, message) if(!(condition)){ \
										std::cout << "test failed in file: " << __FILE__ << std::endl; \
										std::cout << "in function: " << __FUNCTION__ << std::endl; \
										std::cout << "at line: " << __LINE__ << std::endl; \
										std::cout << "with message: " << message << std::endl;	\
										exit(EXIT_FAILURE); \
									}

static void construct_empty_text_editor(){
	TermGui::TextEditor editor;
	assert(editor.get_line() == 0, "Line number of an empty editor should be 0 but is: " << editor.get_line())
	assert(editor.get_column() == 0, "Column number of an empty editor should be 0 but is: " << editor.get_line())
}

int main(){
	
	construct_empty_text_editor();
	
}
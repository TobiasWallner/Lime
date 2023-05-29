

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
	assert(editor.get_line() == 0, "Line number of an empty editor should be 0 but is: " << editor.get_line());
	assert(editor.get_column() == 0, "Column number of an empty editor should be 0 but is: " << editor.get_column());
}

static void insert_character_advances_column(){
	TermGui::TextEditor editor;
	editor.insert(utf8::Char("€"));
	assert(editor.get_line() == 0, "Line number after non line break insert should be 0 but is: " << editor.get_line());
	assert(editor.get_column() == 1, "The column number after the insertion of one character should be 1 but is: " << editor.get_column());
}

static void insert_3character_advances_column(){
	TermGui::TextEditor editor;
	editor.insert(utf8::Char("€"));
	editor.insert('8');
	editor.insert("ä");
	assert(editor.get_line() == 0, "Line number after non line break insert should be 0 but is: " << editor.get_line());
	assert(editor.get_column() == 3, "The column number after the insertion should be 3 but is: " << editor.get_column());
}

static void insert_string_advances_column(){
	TermGui::TextEditor editor;
	const bool flag = editor.insert("€äüp?ß");
	assert(flag == true, "the insertion should not have an error");
	assert(editor.get_line() == 0, "Line number after non line break insert should be 0 but is: " << editor.get_line());
	assert(editor.get_column() == 6, "The column number after the insertion should be 6 but is: " << editor.get_column());
}

int main(){
	
	construct_empty_text_editor();
	insert_character_advances_column();
	insert_3character_advances_column();
	
	
	insert_string_advances_column();
	
}
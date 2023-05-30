

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
									
#define assert_expected(variable_under_test, expected_value) \
			if(!((variable_under_test) == (expected_value))){ \
				std::cout << "test failed in file: " << __FILE__ << '\n'; \
				std::cout << "in function: " << __FUNCTION__ << '\n'; \
				std::cout << "at line: " << __LINE__ << '\n'; \
				std::cout << "failed: " << #variable_under_test << " == " << #expected_value << '\n';\
				std::cout << "expected: " << #variable_under_test << " == " << (expected_value) << ", but got: " << #variable_under_test " = " << (variable_under_test) << '\n' << std::endl;	\
				exit(EXIT_FAILURE); \
			}
									
									

static void construct_empty_text_editor(){
	TermGui::TextEditor editor;
	assert_expected(editor.line_number(), 0);
	assert_expected(editor.column_number(), 0);
	assert_expected(editor.number_of_lines(), 1);
}

static void insert_character_advances_column(){
	TermGui::TextEditor editor;
	editor.insert(utf8::Char("€"));
	assert_expected(editor.line_number(), 0);
	assert_expected(editor.column_number(), 1);
	assert_expected(editor.number_of_lines(), 1);
}

static void insert_3character_advances_column(){
	TermGui::TextEditor editor;
	editor.insert(utf8::Char("€"));
	editor.insert('8');
	editor.insert("ä");
	assert_expected(editor.line_number(), 0);
	assert_expected(editor.column_number(), 3);
	assert_expected(editor.number_of_lines(), 1);
}

static void insert_string_advances_column(){
	TermGui::TextEditor editor;
	const bool flag = editor.insert("€äüp?ß");
	assert_expected(flag, true);
	assert_expected(editor.line_number(), 0);
	assert_expected(editor.column_number(), 6);
	assert_expected(editor.number_of_lines(), 1);
}

static void insert_new_line_character(){
	TermGui::TextEditor editor;
	editor.insert('\n');
	assert_expected(editor.line_number(), 1);
	assert_expected(editor.column_number(), 0);
	assert_expected(editor.number_of_lines(), 2);
}

static void insert_text_and_move_back_twice(){
	TermGui::TextEditor editor;
	editor.insert("some text");
	editor.move_back();
	editor.move_back();
	assert_expected(editor.line_number(), 0);
	assert_expected(editor.column_number(), 9-2);
	assert_expected(editor.number_of_lines(), 1);
}

static void insert_text_movve_back_twice_insert_line_break(){
	TermGui::TextEditor editor;
	editor.insert("some text");
	editor.move_back();
	editor.move_back();
	editor.insert('\n');
	assert_expected(editor.line_number(), 1);
	assert_expected(editor.column_number(), 0);
	assert_expected(editor.number_of_lines(), 2);
	assert_expected(editor.front().size(), 7);
	assert_expected(editor.back().size(), 2);
}

static void insert_text_movve_back_twice_insert_line_break_move_up(){
	TermGui::TextEditor editor;
	editor.insert("some text");
	editor.move_back();
	editor.move_back();
	editor.insert('\n');
	editor.move_up();
	assert_expected(editor.line_number(), 0);
	assert_expected(editor.column_number(), 0);
	assert_expected(editor.number_of_lines(), 2);
	assert_expected(editor.front().size(), 7);
	assert_expected(editor.back().size(), 2);
}

int main(){
	
	construct_empty_text_editor();
	insert_character_advances_column();
	insert_3character_advances_column();
	insert_string_advances_column();
	insert_new_line_character();
	insert_text_and_move_back_twice();
	insert_text_movve_back_twice_insert_line_break();
	insert_text_movve_back_twice_insert_line_break_move_up();
	
}
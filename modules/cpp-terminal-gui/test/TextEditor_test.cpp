

#include <cpp-terminal-gui/TextEditor.hpp>

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
	assert_expected(editor.is_start_of_line(), true);
	assert_expected(editor.is_end_of_line(), true);
	assert_expected(editor.is_last_line(), true);
	assert_expected(editor.is_first_line(), true);
	assert_expected(editor.is_start_of_file(), true);
	assert_expected(editor.is_end_of_file(), true);
	
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

static void insert_after_new_line(){
	TermGui::TextEditor editor;
	editor.insert("123456");
	editor.insert('\n');
	editor.insert("123");
	assert_expected(editor.line_number(), 1);
	assert_expected(editor.column_number(), 3);
	assert_expected(editor.number_of_lines(), 2);
	assert_expected(editor.front().size(), 6);
	assert_expected(editor.back().size(), 3);
}

static void insert_then_Delete(){
	TermGui::TextEditor editor;
	editor.insert("some long text in the first line");
	auto size = editor.front().size();
	
	auto cursorLine = editor.cursor_line();
	auto cursorColumn = editor.cursor_column();
	
	editor.Delete();
	auto newSize = editor.front().size();
	
	assert_expected(editor.cursor_line(), cursorLine);
	assert_expected(editor.cursor_column(), cursorColumn-1);
	assert_expected(newSize, size - 1);
}

static void insert_then_new_line_then_Delete_the_new_line(){
	TermGui::TextEditor editor;
	editor.insert("some long text in the first line");
	editor.insert_new_line();
	editor.insert("some long text in the second line");
	auto size1 = editor.front().size();
	auto size2 = editor.back().size();
	editor.move_to_start_of_line();
	
	editor.Delete();
	auto newSize = editor.front().size();
	
	assert_expected(editor.cursor_line(), 0);
	assert_expected(editor.cursor_column(), size1);
	assert_expected(editor.number_of_lines(), 1);
	assert_expected(newSize, size1 + size2);
}

static void insert_move_to_start_of_line(){
	TermGui::TextEditor editor;
	editor.insert("some long text in the first line");
	editor.insert_new_line();
	editor.insert("some long text in the second line");
	editor.move_to_start_of_line();
	assert_expected(editor.cursor_line(), 1);
	assert_expected(editor.cursor_column(), 0);
}

static void insert_move_to_start_of_line_then_end_of_line(){
	TermGui::TextEditor editor;
	editor.insert("some long text in the first line");
	editor.insert_new_line();
	editor.insert("some long text in the second line");
	editor.move_to_start_of_line();
	editor.move_to_end_of_line();
	assert_expected(editor.cursor_line(), 1);
	assert_expected(editor.cursor_column(), sizeof("some long text in the second line")-1);
}

static void insert_move_to_start_of_file(){
	TermGui::TextEditor editor;
	editor.insert("some long text in the first line");
	editor.insert_new_line();
	editor.insert("some long text in the second line");
	editor.move_to_start_of_file();
	assert_expected(editor.cursor_line(), 0);
	assert_expected(editor.cursor_column(), 0);
}

static void insert_move_to_start_of_file_then_end_of_file(){
	TermGui::TextEditor editor;
	editor.insert("some long text in the first line");
	editor.insert_new_line();
	editor.insert("some long text in the second line");
	editor.move_to_start_of_file();
	editor.move_to_end_of_file();
	assert_expected(editor.cursor_line(), 1);
	assert_expected(editor.cursor_column(), sizeof("some long text in the second line")-1);
}

static void insert_then_Delete_from_file_start(){
	TermGui::TextEditor editor;
	editor.insert("some long text in the first line");
	editor.insert_new_line();
	editor.insert("some long text in the second line");
	editor.move_to_start_of_file();
	
	assert_expected(editor.cursor_line(), 0);
	assert_expected(editor.cursor_column(), 0);
	assert_expected(editor.is_start_of_file(), true);
	
	auto size1 = editor.front().size();
	auto size2 = editor.back().size();
	editor.Delete();
	auto newSize1 = editor.front().size();
	auto newSize2 = editor.back().size();
	
	assert_expected(editor.is_start_of_file(), true);
	assert_expected(editor.cursor_line(), 0);
	assert_expected(editor.cursor_column(), 0);
	assert_expected(size1, newSize1);
	assert_expected(size2, newSize2);
	assert_expected(editor.number_of_lines(), 2);
}

static void read_file(){
	TermGui::TextEditor editor;

	editor.read_file("test.txt");
	assert_expected(editor.is_start_of_file(), true);
	assert_expected(editor.number_of_lines(), 5);
}

static void writeRead_file(){
	TermGui::TextEditor editor1;
	TermGui::TextEditor editor2;
	editor1.insert("some long text in the first line");
	editor1.insert_new_line();
	editor1.insert("some long text in the second line");

	editor1.write_file("writeTest.txt");
	editor2.read_file("writeTest.txt");
	assert_expected(editor1.number_of_lines(), editor2.number_of_lines());
	assert_expected(editor1 == editor2, true);
}

static void equality_test_for_text_editors(){
	TermGui::TextEditor editor1;
	TermGui::TextEditor editor2;
	TermGui::TextEditor editor3;
	
	editor1.insert("Alle meine Entchen");
	editor2.insert("schwimmend auf dem See");
	editor3.insert("schwimmend auf dem See");
	
	assert_expected(editor1 == editor2, false);
	assert_expected(editor2 == editor3, true);
}

int main(){
	while(!std::filesystem::is_regular_file("main.cpp")){
		std::filesystem::current_path("..");
	}
	std::filesystem::current_path("modules/cpp-terminal-gui/test");
	
	construct_empty_text_editor();
	insert_character_advances_column();
	insert_3character_advances_column();
	insert_string_advances_column();
	insert_new_line_character();
	insert_text_and_move_back_twice();
	insert_text_movve_back_twice_insert_line_break();
	insert_text_movve_back_twice_insert_line_break_move_up();
	insert_after_new_line();
	
	insert_move_to_start_of_line();
	insert_move_to_start_of_line_then_end_of_line();
	insert_move_to_start_of_file();
	insert_move_to_start_of_file_then_end_of_file();
	
	insert_then_Delete();
	insert_then_new_line_then_Delete_the_new_line();
	insert_then_Delete();
	insert_then_Delete_from_file_start();

	read_file();
	writeRead_file();
	
	equality_test_for_text_editors();
	
	return EXIT_SUCCESS;
	
}

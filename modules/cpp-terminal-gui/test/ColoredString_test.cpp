

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
									

static void insert_into_static_string_and_erase_last(){
	TermGui::ColoredString string;
	string << "word1 " << TermGui::FgColor(255, 0, 0) << " red" << TermGui::FgColor(255, 255, 255);
	
	auto size = string.size();
	auto first_color_index = string.commands().front().index;
	auto last_color_index = string.commands().back().index;
	
	string.erase(string.size()-1);
	
	assert_expected(string.size(), size - 1);
	assert_expected(string.commands().size, 2);
	assert_expected(string.commands().front().index, first_color_index);
	assert_expected(string.commands().back().index, last_color_index-1);
}

static void insert_into_static_string_and_erase_between(){
	TermGui::ColoredString string;
	string << "word1 " << TermGui::FgColor(255, 0, 0) << " red" << TermGui::FgColor(255, 255, 255);
	
	auto size = string.size();
	auto first_color_index = string.commands().front().index;
	auto last_color_index = string.commands().back().index;
	
	string.erase((first_color_index + last_color_index)/2);
	
	assert_expected(string.size(), size - 1);
	assert_expected(string.commands().size, 2);
	assert_expected(string.commands().front().index, first_color_index);
	assert_expected(string.commands().back().index, last_color_index-1);
}

static void insert_into_static_string_and_erase_first(){
	TermGui::ColoredString string;
	string << "word1 " << TermGui::FgColor(255, 0, 0) << " red" << TermGui::FgColor(255, 255, 255);
	
	auto size = string.size();
	auto first_color_index = string.commands().front().index();
	auto last_color_index = string.commands().back().index();
	
	string.erase(0);
	
	assert_expected(string.size(), size - 1);
	assert_expected(string.commands().size, 2);
	assert_expected(string.commands().front().index, first_color_index-1);
	assert_expected(string.commands().back().index, last_color_index-1);
}

static void insert_into_static_string_and_erase_range(){
	TermGui::ColoredString string;
	string << "word1 " << TermGui::FgColor(255, 0, 0) << " red" << TermGui::FgColor(255, 255, 255);
	
	auto size = string.size();
	auto first_color_index = string.commands().front().index();
	auto last_color_index = string.commands().back().index();
	
	string.erase(first_color_index, last_color_index);
	
	auto erased = last_color_index - first_color_index;
	
	assert_expected(string.size(), size - erased);
	assert_expected(string.commands().size, 1);
	assert_expected(string.commands().front().index, first_color_index);
	assert_expected(static_cast<TermGui::FgColor>(string.commands().front().command), TermGui::FgColor(255, 0, 0));
}

int main(){
	
	insert_into_static_string_and_erase_last();
	insert_into_static_string_and_erase_between();
	insert_into_static_string_and_erase_first();
	insert_into_static_string_and_erase_range();
	
	return EXIT_SUCCESS;
}
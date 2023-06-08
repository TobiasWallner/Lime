

#include <cpp-terminal-gui/ColorString.hpp>

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
	TermGui::ColorString string;
	string << "word1 " << TermGui::fg_color(255, 0, 0) << " red" << TermGui::fg_color(255, 255, 255);
	
	const auto size = string.size();
	const auto first_color_index = string.styles().front().index;
	const auto last_color_index = string.styles().back().index;
	
	string.erase(string.size()-1);
	
	assert_expected(string.size(), size - 1);
	assert_expected(string.styles().size(), 2);
	assert_expected(string.styles().front().index, first_color_index);
	assert_expected(string.styles().back().index, last_color_index-1);
}

static void insert_into_static_string_and_erase_between(){
	TermGui::ColorString string;
	string << "word1 " << TermGui::fg_color(255, 0, 0) << " red" << TermGui::fg_color(255, 255, 255);
	
	const auto size = string.size();
	assert_expected(size, 10);
	
	const auto first_color_index = string.styles().front().index;
	const auto last_color_index = string.styles().back().index;
	
	assert_expected(string.styles().back().index, last_color_index);
	
	string.erase((first_color_index + last_color_index)/2);
	
	assert_expected(string.size(), size - 1);
	assert_expected(string.styles().size(), 2);
	assert_expected(string.styles().front().index, first_color_index);
	assert_expected(string.styles().back().index, last_color_index-1);
}

static void insert_into_static_string_and_erase_first(){
	TermGui::ColorString string;
	string << "word1 " << TermGui::fg_color(255, 0, 0) << " red" << TermGui::fg_color(255, 255, 255);
	
	auto size = string.size();
	auto first_color_index = string.styles().front().index;
	auto last_color_index = string.styles().back().index;
	
	string.erase(0);
	
	assert_expected(string.size(), size - 1);
	assert_expected(string.styles().size(), 2);
	assert_expected(string.styles().front().index, first_color_index-1);
	assert_expected(string.styles().back().index, last_color_index-1);
}

static void insert_into_static_string_and_erase_range(){
	TermGui::ColorString string;
	string << "word1 " << TermGui::fg_color(255, 0, 0) << " red" << TermGui::fg_color(255, 255, 255);
	
	const auto size = string.size();
	const auto first_color_index = string.styles().front().index;
	const auto last_color_index = string.styles().back().index;
	
	const auto erase_begin = first_color_index;
	const auto erase_end = last_color_index + 1;
	
	string.erase(erase_begin, erase_end);
	
	auto erased = erase_end - erase_begin - 1;
	
	assert_expected(string.size(), size - erased);
	assert_expected(string.styles().size(), 1);
	assert_expected(string.styles().front().index, first_color_index);
	
	auto expected = TermGui::fg_color(255, 255, 255);
	auto got = string.styles().front().styles.front();
	
	assert(got == expected, "wrong color survived.");
}

int main(){
	
	insert_into_static_string_and_erase_last();
	insert_into_static_string_and_erase_between();
	insert_into_static_string_and_erase_first();
	insert_into_static_string_and_erase_range();
	
	return EXIT_SUCCESS;
}

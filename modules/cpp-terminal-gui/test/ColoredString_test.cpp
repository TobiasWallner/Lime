

#include <cpp-terminal-gui\TextEditor.hpp>

#include <../verification/verification.hpp>
// C std
#include <stdlib.h>

// C++ std
#include <iostream>
#include <algorithm>
#include <filesystem>

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

static void verify_string_in_file(){
	TermGui::ColoredString string;
	string << "I see skyes of " << TermGui::FgColor(0, 0, 255), " blue" << TermGui::FgColor(255, 255, 255);
	string << " and clouds of " << TermGui::FgColor(0, 0, 0) << TermGui::BgColor(255, 255, 255) << " white\n";
	string << TermGui::FgColor(255, 255, 0) << TermGui::BgColor(0, 0, 0) << "The bright blessed days, ";
	string << TermGui::FgColor(255, 255, 255) << TermGui::BgColor(105,105,105) << "the dark sacred nights\n";
	string << TermGui::FgColor(255, 255, 255) << TermGui::BgColor(0, 0, 0) << "And I think to myself\n";
  	string << TermGui::FgColor(255, 0, 0)  << "Wh";
  	string << TermGui::FgColor(255, 165, 0)  << "at ";
  	string << TermGui::FgColor(255, 255, 0)  << "a ";
  	string << TermGui::FgColor(0, 128, 0)  << "wonder";
  	string << TermGui::FgColor(0, 0, 255)  << "ful ";
  	string << TermGui::FgColor(75, 0, 130)  << "wor";
  	string << TermGui::FgColor(238, 130, 238)  << "ld\n";
	
	std::cout << string;

	std::filesystem::path currPath = std::filesystem::current_path();
	verifyFile(currPath, "color_string", string);

}

int main(){
	
	insert_into_static_string_and_erase_last();
	insert_into_static_string_and_erase_between();
	insert_into_static_string_and_erase_first();
	insert_into_static_string_and_erase_range();
	verify_string_in_file();
	return EXIT_SUCCESS;
}


#include <cpp-terminal-gui/ColorString.hpp>

#include "../verification/verification.hpp"

#include <cpp-terminal/terminal.hpp>
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
									

static void insert_into_string_and_erase_last(){
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

static void insert_into_string_and_erase_between(){
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

static void insert_into_string_and_erase_first(){
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

static void insert_into_string_and_erase_range(){
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
	assert_expected(string.styles().size(), 2);
	assert_expected(string.styles().front().index, first_color_index);
	
	const auto expected = TermGui::fg_color(255, 255, 255);
	const auto& got = string.styles().front().styles.front();
	
	assert(got == expected, "wrong color survived.");
}

static void erase_from_text_after_last_style() {
	TermGui::ColorString string;
	string << "word1 " << TermGui::fg_color(255, 0, 0) << " red" << TermGui::fg_color(255, 255, 255) << " this is red now";

	const auto size = string.size();
	const auto first_color_index = string.styles().front().index;
	const auto last_color_index = string.styles().back().index;

	string.erase(string.size() - 1);

	assert_expected(string.size(), size - 1);
	assert_expected(string.styles().size(), 2);
	assert_expected(string.styles().front().index, first_color_index);
	assert_expected(string.styles().back().index, last_color_index);
}

static void verify_string_in_file(){
	TermGui::ColorString string;
	string << "I see skyes of " << TermGui::fg_color(0, 0, 255) << " blue" << TermGui::fg_color(255, 255, 255);
	string << " and clouds of " << TermGui::fg_color(0, 0, 0) << TermGui::bg_color(255, 255, 255) << " white\n";
	string << TermGui::fg_color(255, 255, 0) << TermGui::bg_color(0, 0, 0) << "The bright blessed days, ";
	string << TermGui::fg_color(255, 255, 255) << TermGui::bg_color(105,105,105) << "the dark sacred nights\n";
	string << TermGui::fg_color(255, 255, 255) << TermGui::bg_color(0, 0, 0) << "And I think to myself\n";
  	string << TermGui::fg_color(255, 0, 0)  << "Wh";
  	string << TermGui::fg_color(255, 165, 0)  << "at ";
  	string << TermGui::fg_color(255, 255, 0)  << "a ";
  	string << TermGui::fg_color(0, 128, 0)  << "wonder";
  	string << TermGui::fg_color(0, 0, 255)  << "ful ";
  	string << TermGui::fg_color(75, 0, 130)  << "wor";
  	string << TermGui::fg_color(238, 130, 238)  << "ld\n";
	std::string out;
	string.render(out);
	std::cout << out;

	for(char c : out){
		printf("%x ", c);
	}
	
	std::cout << std::endl;
	
	std::cout << out << std::endl;
	

	std::filesystem::path currPath = std::filesystem::current_path();
	assert(fileVerify(currPath, "color_string", out), "The file did not get verified");

}



int main(){
	
	try{
		Term::terminal.setOptions(
					Term::Option::ClearScreen, 	// start from a cear screen
					Term::Option::NoSignalKeys, // deactivate key combinations that generate a signal or interrupt
					Term::Option::NoCursor, 	// deactivate the cursor, we will display it ourselfs
					Term::Option::Raw			// get the raw and unprozessed io data from the console buffers
		);
		
		insert_into_string_and_erase_last();
		insert_into_string_and_erase_between();
		insert_into_string_and_erase_first();
		insert_into_string_and_erase_range();
		erase_from_text_after_last_style();
		verify_string_in_file();
	
	}catch(...){
		std::cout << "Error: exception thrown" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

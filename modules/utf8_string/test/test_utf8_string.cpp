#include "../utf8_string.hpp"

#include <iostream>
#include <string_view>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <cstring>

#define assert(condition, message) if(!(condition)){ \
										std::cout << "test failed at line: " << __LINE__ << std::endl; \
										std::cout << "with message: " << message << std::endl;	\
										exit(EXIT_FAILURE); \
									}
									
int main(){

	{ // createing string results in string being empty
		utf8::wstring s;
		assert(s.empty() == true, "string should be empty.");
	}
	{ // assign string with ascii sequence results in string not empty
		utf8::wstring s("löka");
		assert(s.empty() == false, "string should not be empty.");
	}
	{// pushing back to empty string changes size to 1
		utf8::wstring s;
		s.push_back('a');
		const auto expected = 1;
		assert(s.size() == expected, "pushed back one character, size should be" << expected << " but is " << s.size());
	}
	{// pushing back to empty string changes size to 1
		utf8::wstring s;
		s.push_back("ß");
		const auto expected = 1;
		assert(s.size() == expected, "pushed back one character, size should be" << expected << " but is " << s.size());
	}
	{// pushing back to empty string changes size to 1
		utf8::wstring s;
		s.push_back("水");
		const auto expected = 1;
		assert(s.size() == expected, "pushed back one character, size should be" << expected << " but is " << s.size());
	}
	{// pushing back to empty string changes size to 1
		utf8::wstring s;
		s.push_back("🦄");
		const auto expected = 1;
		assert(s.size() == expected, "pushed back one character, size should be" << expected << " but is " << s.size());
	}
	{// push back 4 times should give size 4
		utf8::wstring s;
		s.push_back("🦄");
		s.push_back("水");
		s.push_back("ß");
		s.push_back('a');
		const auto expected = 4;
		assert(s.size() == expected, "pushed back one character, size should be" << expected << " but is " << s.size());
	}
	{// assign string of 5 characters gives size of 5
		utf8::wstring s("🦄水ßaä");
		const auto expected = 5;
		assert(s.size() == expected, "pushed back one character, size should be" << expected << " but is " << s.size());
	}
	{// to string provides the same c_str again for ascii only
		std::string input("ascii");
		utf8::wstring utf8(input);
		auto result = utf8.to_std_string();
		assert(result == input, "error converting from std::string to utf8::wstring and back to std::string\n"
								"result should be: '" << input << "' but is '" << result << "'.");
		
	}
	{// read utf8 characters should output same characters
		std::string input("🦄水ßaä");
		utf8::wstring utf8(input);
		auto result = utf8.to_std_string();
		assert(result == input, "error converting from std::string to utf8::wstring and back to std::string\n"
								"result should be: '" << input << "' but is '" << result << "'.");
	}
	{// use the assignment operator
		std::string input("🦄水ßaä");
		utf8::wstring utf8;
		utf8 = input;
		auto result = utf8.to_std_string();
		assert(result == input, "error result should be: '" << input << "' but is '" << result << "'.");
	}
	{// use the += operator
		std::string input1("🦄水");
		std::string input2("ßaä");
		utf8::wstring utf8;
		utf8 += input1;
		utf8 += input2;
		auto expected = input1 + input2;
		auto result = utf8.to_std_string();
		assert(result == expected, "error result should be: '" << expected << "' but is '" << result << "'.");
	}
	{// use the + operator
		std::string input1("🦄水");
		std::string input2("ßaä");
		utf8::wstring utf8_1;
		utf8::wstring utf8_2;
		utf8_1 = input1;
		utf8_2 = input2;
		auto expected = input1 + input2;
		auto utf8 = utf8_1 + utf8_2;
		auto result = utf8.to_std_string();
		assert(result == expected, "error result should be: '" << expected << "' but is '" << result << "'.");
	}
	{// use the + operator move lhs
		std::string input1("🦄水");
		std::string input2("ßaä");
		utf8::wstring utf8_1;
		utf8::wstring utf8_2;
		utf8_1 = input1;
		utf8_2 = input2;
		auto expected = input1 + input2;
		auto utf8 = std::move(utf8_1) + utf8_2;
		auto result = utf8.to_std_string();
		assert(result == expected, "error result should be: '" << expected << "' but is '" << result << "'.");
		assert(utf8_1.empty(), "lhs has not been moved");
	}
	{// use the + operator move rhs
		std::string input1("🦄水");
		std::string input2("ßaä");
		utf8::wstring utf8_1;
		utf8::wstring utf8_2;
		utf8_1 = input1;
		utf8_2 = input2;
		auto expected = input1 + input2;
		auto utf8 = utf8_1 + std::move(utf8_2);
		auto result = utf8.to_std_string();
		assert(result == expected, "error result should be: '" << expected << "' but is '" << result << "'. And/Or rhs has not been moved");
		assert(utf8_2.empty(), "rhs has not been moved");
	}
	{// use the + operator move both only moves rhs
		std::string input1("🦄水");
		std::string input2("ßaä");
		utf8::wstring utf8_1;
		utf8::wstring utf8_2;
		utf8_1 = input1;
		utf8_2 = input2;
		auto expected = input1 + input2;
		auto utf8 = std::move(utf8_1) + std::move(utf8_2);
		auto result = utf8.to_std_string();
		assert(result == expected, "error result should be: '" << expected << "' but is '" << result << "'. And/Or rhs has not been moved");
		assert(utf8_1.empty() == true, "lhs has not been moved");
		assert(utf8_2.empty() == false, "rhs has not been moved");
	}
	{// use += to concattenate two strings
		std::string input1("🦄水");
		std::string input2("ßaä");
		utf8::wstring utf8_1;
		utf8::wstring utf8_2;
		utf8_1 = input1;
		utf8_2 = input2;
	
		utf8_1 += utf8_2;
		auto expected = input1 + input2;
		auto result = utf8_1.to_std_string();
		assert(result == expected, "error result should be: '" << expected << "' but is '" << result << "'.");
	}
	{// use append to concattenate two strings
		std::string input1("🦄水");
		std::string input2("ßaä");
		utf8::wstring utf8_1;
		utf8::wstring utf8_2;
		utf8_1 = input1;
		utf8_2 = input2;
	
		utf8_1.append(utf8_2);
		auto expected = input1 + input2;
		auto result = utf8_1.to_std_string();
		assert(result == expected, "error result should be: '" << expected << "' but is '" << result << "'.");
	}
	
	std::cout << "finished tests" << std::endl;
	return 0;
}

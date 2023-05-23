
#include "../utf8_char.hpp"

#include <iostream>
#include <string_view>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <cstring>

#define assert(condition, message) if(!(condition)){ \
										std::cout << "test failed at line: " << __LINE__ << std::endl; \
										std::cout << "with message: " << message << std::endl;	\
										exit(-1); \
									}
int main(){
	
	{// utf8 identifier test
		const char* input = "d";
		const auto length = utf8::identify(input[0]);
		const auto expected = utf8::Identifier::Bytes1;
		assert(length == expected, "Error: identify, it should be " << static_cast<int>(expected) << " but is " << static_cast<int>(length));
	}
	{// utf8 identifier test
		const char* input = "ß";
		const auto length = utf8::identify(input[0]);
		const auto expected = utf8::Identifier::Bytes2;
		assert(length == expected, "Error: identify, it should be " << static_cast<int>(expected) << " but is " << static_cast<int>(length));
	}
	{// utf8 identifier test
		const char* input = "水";
		const auto length = utf8::identify(input[0]);
		const auto expected = utf8::Identifier::Bytes3;
		assert(length == expected, "Error: identify, it should be " << static_cast<int>(expected) << " but is " << static_cast<int>(length));
	}
	{// utf8 identifier test
		const char* input = "水";
		const auto length = utf8::identify(input[1]);
		const auto expected = utf8::Identifier::NotFirst;
		assert(length == expected, "Error: identify, it should be " << static_cast<int>(expected) << " but is " << static_cast<int>(length));
	}
	{// utf8 identifier test
		const char* input = "🦄";
		const auto length = utf8::identify(input[0]);
		const auto expected = utf8::Identifier::Bytes4;
		assert(length == expected, "Error: identify, it should be " << static_cast<int>(expected) << " but is " << static_cast<int>(length));
	}
	{// ascii has one byte
		char input = 'g';
		utf8::Char c(input);
		const auto expected = 1;
		assert(c.size() == expected, "length of ascii character is wrong, it should be " << expected << " but is " << c.size());
	}
	{// ascii from string has one byte
		const char* input = "d";
		utf8::Char c(input);
		const auto expected = std::strlen(input);
		assert(c.size() == expected, "length of ascii character is wrong, it should be " << expected << " but is " << c.size());
	}
	{// utf8 from string has 2 bytes with array
		const char input[] = "ß";
		utf8::Char c(input);
		const auto expected = std::strlen(input);
		assert(c.size() == expected, "length of ascii character is wrong, it should be " << expected << " but is " << c.size());
	}
	{// utf8 from string has 2 bytes with std::string
		std::string input = "ß";
		utf8::Char c(input);
		const auto expected = input.size();
		assert(c.size() == expected, "length of ascii character is wrong, it should be " << expected << " but is " << c.size());
	}
	{// utf8 from string has 2 bytes with std::string_view
		std::string_view input = "ß";
		utf8::Char c(input);
		const auto expected = input.size();
		assert(c.size() == expected, "length of ascii character is wrong, it should be " << expected << " but is " << c.size());
	}
	{// size check with utf8 character that is 3 byte long
		const char* input = "水";
		utf8::Char c(input);
		const auto expected = std::strlen(input);
		assert(c.size() == expected, "length of ascii character is wrong, it should be " << expected << " but is " << c.size());
	}
	{// size check with utf8 character that is 4 bytes long
		const char* input = "€";
		utf8::Char c(input);
		const auto expected = std::strlen(input);
		assert(c.size() == expected, "length of ascii character is wrong, it should be " << expected << " but is " << c.size());
	}
	{// size check with utf8 character that is 4 bytes long
		const char* input = "🦄";
		utf8::Char c(input);
		const auto expected = std::strlen(input);
		assert(c.size() == expected, "length of ascii character is wrong, it should be " << expected << " but is " << c.size());
	}
	{// std::string conversion
		std::string input = "ß";
		utf8::Char c(input);
		std::string result = c.to_std_string();
		assert(input == result, "error converting from std::string to utf8::Char and back to std::string\n"
								"result should be: '" << input << "' but is '" << result << "'.");
	}
	{// std::string_view conversion
		std::string_view input = "ß";
		utf8::Char c(input);
		std::string_view result = c.to_std_string_view();
		assert(input == result, "error converting from std::string to utf8::Char and back to std::string\n"
								"result should be: '" << input << "' but is '" << result << "'.");
	}
	{// std::string conversion
		std::string input = "ü";
		utf8::Char c(input);
		std::string result = c.to_std_string();
		assert(input == result, "error converting from std::string to utf8::Char and back to std::string\n"
								"result should be: '" << input << "' but is '" << result << "'.");
	}
	{// std::string conversion with iterators
		std::string input = "ü";
		utf8::Char c(input.cbegin(), input.cend());
		std::string result = c.to_std_string();
		assert(input == result, "error converting from std::string to utf8::Char and back to std::string\n"
								"result should be: '" << input << "' but is '" << result << "'.");
	}
	{// std::string conversion with assign function and iterators
		std::string input = "ü";
		utf8::Char c;
		c.assign(input.cbegin(), input.cend());
		std::string result = c.to_std_string();
		assert(input == result, "error converting from std::string to utf8::Char and back to std::string\n"
								"result should be: '" << input << "' but is '" << result << "'.");
	}
	{// std::string conversion with iterators returns iterator past the read symbol
		std::string input = "ß";
		utf8::Char c;
		auto itr = c.assign(input.cbegin(), input.cend());
		assert(itr == input.cend(), "error the return iterator of the utf8::Char::assign(first, last) function returned the wrong iterator.");
	}
	
	{// comparison of two characters with the same content results in true
		assert((utf8::Char('o') == 'o') == true, "error equality comparison with char is broken");
	}
	{// comparison of two characters with different content results in false
		assert((utf8::Char('o') == 'p') == false, "error equality comparison with char is broken");
	}
	{// comparison with symbol in utf8 string of the same character results in true
		assert((utf8::Char("ö") == "ö") == true, "error equality comparison with char is broken");
	}
	{// comparison of the same symbol with a string that starts with the same symbol but continues results in false
		assert((utf8::Char("ö") == "österreich") == false, "error equality comparison with char is broken");
	}
	{// comparison with an std::string with one utf8 symbol results in true
		assert((utf8::Char("ö") == std::string("ö")) == true, "error equality comparison with char is broken");
	}
	{// comparison with an std::string with more than one utf8 symbol results in false
		assert((utf8::Char("ö") == std::string("österreich")) == false, "error equality comparison with char is broken");
	}
	
	//------- smaller comparisons ---------
	assert((utf8::Char('a') < 'b') == true, "error comparison with char is broken");
	assert((utf8::Char('b') < 'a') == false, "error comparison with char is broken");
	assert((utf8::Char('a') < 'a') == false, "error comparison with char is broken");
	
	assert((utf8::Char('a') < "b") == true, "error comparison with char string is broken");
	assert((utf8::Char('b') < "a") == false, "error comparison with char string is broken");
	assert((utf8::Char('b') < "b") == false, "error comparison with char string is broken");
	
	assert((utf8::Char('a') < std::string("b")) == true, "error comparison with char range is broken");
	assert((utf8::Char('b') < std::string("a")) == false, "error comparison with char range is broken");
	assert((utf8::Char('b') < std::string("b")) == false, "error comparison with char range is broken");
	
	
	assert((utf8::Char('a') > 'b') == false, "error comparison with char is broken");
	assert((utf8::Char('b') > 'a') == true, "error comparison with char is broken");
	assert((utf8::Char('a') > 'a') == false, "error comparison with char is broken");
	
	assert((utf8::Char('a') > "b") == false, "error comparison with char string is broken");
	assert((utf8::Char('b') > "a") == true, "error comparison with char string is broken");
	assert((utf8::Char('b') > "b") == false, "error comparison with char string is broken");
	
	assert((utf8::Char('a') > std::string("b")) == false, "error comparison with char range is broken");
	assert((utf8::Char('b') > std::string("a")) == true, "error comparison with char range is broken");
	assert((utf8::Char('b') > std::string("b")) == false, "error comparison with char range is broken");
	
	
	assert((utf8::Char('a') <= 'b') == true, "error comparison with char is broken");
	assert((utf8::Char('b') <= 'a') == false, "error comparison with char is broken");
	assert((utf8::Char('a') <= 'a') == true, "error comparison with char is broken");
	
	assert((utf8::Char('a') <= "b") == true, "error comparison with char string is broken");
	assert((utf8::Char('b') <= "a") == false, "error comparison with char string is broken");
	assert((utf8::Char('b') <= "b") == true, "error comparison with char string is broken");
	
	assert((utf8::Char('a') <= std::string("b")) == true, "error comparison with char range is broken");
	assert((utf8::Char('b') <= std::string("a")) == false, "error comparison with char range is broken");
	assert((utf8::Char('b') <= std::string("b")) == true, "error comparison with char range is broken");
	
	
	assert((utf8::Char('a') >= 'b') == false, "error comparison with char is broken");
	assert((utf8::Char('b') >= 'a') == true, "error comparison with char is broken");
	assert((utf8::Char('a') >= 'a') == true, "error comparison with char is broken");
	
	assert((utf8::Char('a') >= "b") == false, "error comparison with char string is broken");
	assert((utf8::Char('b') >= "a") == true, "error comparison with char string is broken");
	assert((utf8::Char('b') >= "b") == true, "error comparison with char string is broken");
	
	assert((utf8::Char('a') >= std::string("b")) == false, "error comparison with char range is broken");
	assert((utf8::Char('b') >= std::string("a")) == true, "error comparison with char range is broken");
	assert((utf8::Char('b') >= std::string("b")) == true, "error comparison with char range is broken");

	
	
	std::cout << "finished tests" << std::endl;
	return 0;
}

#include "utf8_string.hpp"

#include <iostream>


void _assert(bool condition, const char* message, int line_number){
	if(!condition){
		std::cout << "test failed at line: " << line_number << std::endl;
		std::cout << "with message: " << message << std::endl;	
		exit(-1);
	}	
}

#define assert(condition, message) _assert(condition, message, __LINE__)

int main(){
	
	// length checks of utf8::Char
	{
		char c = 'g';
		utf8::Char c32(c);
		assert(c32.length() == 1, "length of ascii character is wrong");
	}
	
	{
		char16_t c = u'd';
		utf8::Char c32(c);
		assert(c32.length() == 1, "length of char16 character is wrong");
	}
	{
		char16_t c = u'ß';
		utf8::Char c32(c);
		assert(c32.length() == 2, "length of char16 character is wrong");
	}
	{
		char32_t c = U'h';
		utf8::Char c32(c);
		assert(c32.length() == 1, "length of char16 character is wrong");
	}
	{
		char32_t c = U'水';
		utf8::Char c32(c);
		assert(c32.length() == 2, "length of char16 character is wrong");
	}
	{
		char32_t c = U'€';
		utf8::Char c32(c);
		assert(c32.length() == 3, "length of char16 character is wrong");
	}
	{
		char32_t c = U'🦄';
		utf8::Char c32(c);
		assert(c32.length() == 4, "length of char16 character is wrong");
	}
	return 0;
}
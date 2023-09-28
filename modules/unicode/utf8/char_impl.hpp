
#pragma once

// c++ std
#include <cinttypes>
#include <string>
#include <string_view>
#include <ostream>
#include <cinttypes>

// project
#include "char_type.hpp"
#include "char_reference.hpp"

/* TODO: finish refactoring of the implementation */

namespace utf8{

enum class Identifier : std::int8_t{
	NotFirst = 0,
	Bytes1 = 1,
	Bytes2 = 2,
	Bytes3 = 3,
	Bytes4 = 4,
	Unsupported = 5,
};

constexpr bool is_start_byte(Identifier id){
	return  id == Identifier::Bytes1 
			|| id == Identifier::Bytes2
			|| id == Identifier::Bytes3 
			|| id == Identifier::Bytes4;
}

constexpr const char* to_string(Identifier i){
	switch(i){
		case Identifier::NotFirst : return "NotFirst";
		case Identifier::Bytes1 : return "Bytes1";
		case Identifier::Bytes2 : return "Bytes2";
		case Identifier::Bytes3 : return "Bytes3";
		case Identifier::Bytes4 : return "Bytes4";
		default : return "Unsupported";
	}
}

/// returns the length of the utf8 sequence if it is the first character
/// returns '0' if it is a character in the sequence but not the first one
/// returns '-1' if it is an unsupported character by the utf8 standard
constexpr Identifier identify(char c){
	const auto prefix = (c & 0b11111000) >> 3;
	const Identifier lookupTable[32]{
		/*00000*/	Identifier::Bytes1,
		/*00001*/	Identifier::Bytes1,
		/*00010*/	Identifier::Bytes1,
		/*00011*/	Identifier::Bytes1,
		/*00100*/	Identifier::Bytes1,
		/*00101*/	Identifier::Bytes1,
		/*00110*/	Identifier::Bytes1,
		/*00111*/	Identifier::Bytes1,
		/*01000*/	Identifier::Bytes1,
		/*01001*/	Identifier::Bytes1,
		/*01010*/	Identifier::Bytes1,
		/*01011*/	Identifier::Bytes1,
		/*01100*/	Identifier::Bytes1,
		/*01101*/	Identifier::Bytes1,
		/*01110*/	Identifier::Bytes1,
		/*01111*/	Identifier::Bytes1,
		
		/*10000*/	Identifier::NotFirst,
		/*10001*/	Identifier::NotFirst,
		/*10010*/	Identifier::NotFirst,
		/*10011*/	Identifier::NotFirst,
		/*10100*/	Identifier::NotFirst,
		/*10101*/	Identifier::NotFirst,
		/*10110*/	Identifier::NotFirst,
		/*10111*/	Identifier::NotFirst,
		/*11000*/	Identifier::Bytes2,
		/*11001*/	Identifier::Bytes2,
		/*11010*/	Identifier::Bytes2,
		/*11011*/	Identifier::Bytes2,
		/*11100*/	Identifier::Bytes3,
		/*11101*/	Identifier::Bytes3,
		/*11110*/	Identifier::Bytes4,
		/*11111*/	Identifier::Unsupported,
	};
	const auto result = lookupTable[prefix];
	return result;
}

constexpr bool is_start_byte(char c){
	return is_start_byte(identify(c));
}

constexpr Char::Char(char_const_reference ch){this->assign(ch);}
constexpr Char::Char(char_reference ch){this->assign(ch);}

/// construct from an ascii character
constexpr Char::Char(char ascii) : utf8{ascii, 0, 0, 0} {}

constexpr Char::Char(const char* character) : utf8{0} {this->assign(character);}

constexpr Char::Char(char32_t utf32) : utf8{0} {
	// First code point 	Last code point 	Byte 1 		Byte 2 		Byte 3 		Byte 4
	// U+0000 				U+007F 				0xxxxxxx 	
	// U+0080 				U+07FF 				110xxxxx 	10xxxxxx 	
	// U+0800 				U+FFFF 				1110xxxx 	10xxxxxx 	10xxxxxx 	
	// U+10000 				U+10FFFF 			11110xxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
	if(utf32 <= 0x0000'007FL){
		// it is just an ascii character
		this->utf8[0] = static_cast<char>(utf32);
	}else if(utf32 <= 0x0000'07FFL){
		this->utf8[1] = static_cast<char>(utf32 & 0b00111111L) | 0b10000000;
		this->utf8[0] = static_cast<char>((utf32 >> 6) & 0b00011111L) | 0b11000000;
	}else if(utf32 <= 0x0000'FFFFL){
		this->utf8[2] = static_cast<char>(utf32 & 0b00111111L) | 0b10000000;
		this->utf8[1] = static_cast<char>((utf32 >> 6) & 0b00111111L) | 0b10000000;
		this->utf8[0] = static_cast<char>((utf32 >> 12) & 0b00001111) | 0b11100000; 
	}else if(utf32 <= 0x0010'FFFFL){
		this->utf8[3] = static_cast<char>(utf32 & 0b00111111L) | 0b10000000;
		this->utf8[2] = static_cast<char>((utf32 >> 6) & 0b00111111L) | 0b10000000;
		this->utf8[1] = static_cast<char>((utf32 >> 12) & 0b00111111L) | 0b10000000;
		this->utf8[0] = static_cast<char>((utf32 >> 18) & 0b00000111L) | 0b11110000;
	}
}

constexpr Char::Char(std::int32_t utf8) : Char(static_cast<char32_t>(utf8)){}

/// reads one utf8 character from the range and stores it in the char
template<class CharItr>
constexpr Char::Char(CharItr first) : utf8{0} {this->assign(first);}

/// reads one utf8 character from the range and stores it in the char, 
/// makes sure to not read more than the buffer
template<class CharItr>
constexpr Char::Char(CharItr first, CharItr last) : utf8{0} {this->assign(first, last);}

template<class CharItr>
constexpr Char::Char(CharItr first, size_type n) : utf8{0} {this->assign(first, first + n);}

/// reads first utf8 character from the string and makes sure not to read more than the strings content
Char::Char(const std::string& str) : utf8{0} {this->assign(str);}

/// reads first utf8 character from the string and makes sure not to read more than the strings content
Char::Char(const std::string_view& str) : utf8{0} {this->assign(str);}

/// assigns the ascii character to the utf8 character
constexpr void Char::assign(char ascii){utf8[0] = ascii;}

constexpr void Char::assign(Char c){
	this->utf8[0] = c.utf8[0];
	this->utf8[1] = c.utf8[1];
	this->utf8[2] = c.utf8[2];
	this->utf8[3] = c.utf8[3];
}

/// reads the first utf8 character and stores it in the character, within the range
/// returns the iterator to the next character after that.
/// returns the same first iterator if the provided character is not the first byte of an utf8 encoded character
template<class CharItr>
constexpr CharItr Char::assign(CharItr first, CharItr last){
	const auto identifier = identify(static_cast<char>(*first));
	if((identifier == Identifier::Unsupported) || (identifier == Identifier::NotFirst)){
		return first;
	}else{
		const auto length = static_cast<size_type>(identifier);
		for(int i = 0; i != length && first != last && i < 4; ++i, (void)(++first)){
			this->utf8[i] = static_cast<char>(*first);
		}
		return first;
	}		
}

/// reads the first utf8 character and stores it in the character
/// returns the iterator to the next character after that.
/// returns the same iterator if the character is not a valid utf8 character
template<class CharItr>
constexpr CharItr Char::assign(CharItr first){
	const auto identifier = identify(static_cast<char>(*first));
	if((identifier == Identifier::Unsupported) || (identifier == Identifier::NotFirst)){
		return first;
	}else{
		const auto length = static_cast<size_type>(identifier);
		for(int i = 0; i != length && i < 4; ++i, (void)(++first)){
			this->utf8[i] = static_cast<char>(*first);
		}
		return first;
	}	
}

constexpr void Char::assign(char_const_reference ch){this->assign(static_cast<Char>(ch));}
constexpr void Char::assign(char_reference ch){this->assign(static_cast<Char>(ch));}

/// reads the first utf8 character from the string and stores it in the character
/// returns the string iterator after the read character
std::string::const_iterator Char::assign(const std::string& str){return this->assign(str.cbegin(), str.cend());}

/// reads the first utf8 character from the string and stores it in the character
/// returns the string iterator after the read character
std::string_view::const_iterator Char::assign(const std::string_view& str){return this->assign(str.cbegin(), str.cend());}

/// reads the first utf8 character from the string and stores it in the character
/// returns the string iterator after the read character
Char::const_iterator Char::assign(const_iterator str, size_type size){return this->assign(str, str+size);}


/// assigns an ascii character to the utf8 character
constexpr Char& Char::operator=(char c){ this->assign(c); return *this; }

template<class CharItr>
constexpr Char& Char::operator=(CharItr itr){ this->assign(itr); return *this; }

Char& Char::operator=(const std::string& str){ this->assign(str); return *this; }
Char& Char::operator=(const std::string_view& str){ this->assign(str); return *this; }

/// returns the first character of the utf8 character string
constexpr char& Char::front() { return this->utf8[0]; }
constexpr const char& Char::front() const {return this->utf8[0];}

/// returns the begin iterator of the utf8 character string
constexpr Char::iterator Char::begin() {return this->utf8;}
constexpr Char::const_iterator Char::begin() const {return this->utf8;}
constexpr Char::const_iterator Char::cbegin() const {return this->utf8;}

/// returns the end iterator of the utf8 character string
constexpr Char::iterator Char::end() {return this->begin() + this->size();}
constexpr Char::const_iterator Char::end() const {return this->begin() + this->size();}
constexpr Char::const_iterator Char::cend() const {return this->begin() + this->size();}

/// returns a string view object pointing to the utf8 symbol
constexpr std::string_view Char::to_std_string_view() const {return std::string_view(this->begin(), this->size());}

/// returns the character at the index of the utf8 string
constexpr Char::reference Char::operator[](size_type index){return this->utf8[index];}
constexpr Char::const_reference Char::operator[](size_type index) const {return this->utf8[index];}

/// returns the number of bytes that the unicode character needs
constexpr Char::size_type Char::size() const {
	const auto first_char = this->front();
	const auto result = identify(first_char);
	return static_cast<size_type>(result);
}

/// provides a pointer to the underlieing data
constexpr Char::pointer Char::data() {return this->utf8;}
constexpr Char::const_pointer Char::data() const {return this->utf8;}

/// writes this character into an std::string
void Char::print(std::string& str) const {str.append(this->utf8, this->size());}

/// turns this utf8 character string into an std::string
std::string Char::to_std_string() const {return std::string(this->utf8, this->size());}

/// writes this char into an std::ostream useing the stream operator <<
template<class OStream>
std::ostream& Char::operator << (OStream& stream, Char c) {
	stream.write(c.utf8, c.size()); return stream;
}

/// reads an utf8 char from the input stream
template<class IStream>
IStream& Char::operator >> (IStream& stream, Char& c) {
	c[0] = stream.get();
	const auto length = identify(c[0]);
	switch(length){
		case 2: {
			c[1] = stream.get();
		}break;
		case 3: {
			c[1] = stream.get();
			c[2] = stream.get();
		}break;
		case 4: {
			c[1] = stream.get();
			c[2] = stream.get();
			c[3] = stream.get();
		}break;
	}
	return stream;
}

constexpr Char::int_type to_int() const {
	switch(this->size()){
		case 1: return (static_cast<int_type>(utf8[0]));
		case 2: return (static_cast<int_type>(utf8[0] & 0b01111111) << 8) 
						| static_cast<int_type>(utf8[1]);
		case 3: return (static_cast<int_type>(utf8[0] & 0b00111111) << 16)
						| (static_cast<int_type>(utf8[1]) << 8)
						| (static_cast<int_type>(utf8[2]));
		case 4: return (static_cast<int_type>(utf8[0] & 0b00011111) << 24)
						| (static_cast<int_type>(utf8[1]) << 16)
						| (static_cast<int_type>(utf8[2]) << 8)
						| (static_cast<int_type>(utf8[3]));
		default: return -1;
	}	
}

/// equality comparison with the same type
constexpr bool Char::operator==(Char lhs, Char rhs){return lhs.to_int() == rhs.to_int();}

constexpr bool Char::operator==(Char lhs, char rhs){return lhs.to_int() == static_cast<Char::int_type>(rhs);}
constexpr bool Char::operator==(char lhs, Char rhs){return (rhs == lhs);}

/// equality comparison with a const char* string symbol that is null terminated
constexpr bool Char::operator==(Char lhs, const char* rhs){
	auto lhsItr = lhs.cbegin();
	auto lhsEnd = lhs.cend();
	for(; lhsItr != lhsEnd; ++lhsItr, (void)++rhs){
		if(*rhs == '\0' || *lhsItr != *rhs) return false;
	}
	return *rhs == '\0';
}

/// equality comparison with a const char* string symbol
constexpr bool Char::operator==(const char* lhs, Char rhs){return (rhs == lhs);}

/// equality comparison with a range. has to support cbegin() and cend() !
template<class Range>
bool operator==(Char lhs, const Range& rhs){
	auto lhsItr = lhs.cbegin();
	const auto lhsEnd = lhs.cend();
	auto rhsItr = rhs.cbegin();
	const auto rhsEnd = rhs.cend();
	for(; lhsItr != lhsEnd && rhsItr != rhsEnd; ++lhsItr, (void)++rhsItr){
		if(*lhsItr != *rhsItr) return false;
	}
	return lhsItr == lhsEnd && rhsItr == rhsEnd;
}

constexpr bool Char::operator!=(Char lhs, char rhs){return !(lhs == rhs);}
constexpr bool Char::operator!=(char lhs, Char rhs){return !(lhs == rhs);}

/// equality comparison with a range. has to support cbegin() and cend() !
template<class Range>
constexpr bool Char::operator==(const Range& lhs, Char rhs){return (rhs == lhs);}

/// unequality comparison with self
constexpr bool Char::operator!=(Char lhs, Char rhs){return lhs.to_int() != rhs.to_int();}

/// unequality comparison with c_string
constexpr bool Char::operator!=(Char lhs, const char* rhs){return !(lhs == rhs);}
constexpr bool Char::operator!=(const char* lhs, Char rhs){return !(lhs == rhs);}

/// unequality comparison with char range
template<class Range>
bool Char::operator!=(const Range& lhs, Char rhs){return !(lhs == rhs);}

/// unequality comparison with char range
template<class Range>
bool Char::operator!=(Char lhs, const Range& rhs){return !(lhs == rhs);}

constexpr bool Char::operator<(Char lhs, const char* rhs){
	auto lhsItr = lhs.cbegin();
	auto lhsEnd = lhs.cend();
	for(; lhsItr != lhsEnd; ++lhsItr, (void)++rhs){
		if(*rhs == '\0' || *lhsItr >= *rhs) return false;
	}
	return *rhs == '\0';
}

constexpr bool Char::operator>(Char lhs, const char* rhs){
	auto lhsItr = lhs.cbegin();
	auto lhsEnd = lhs.cend();
	for(; lhsItr != lhsEnd; ++lhsItr, (void)++rhs){
		if(*rhs == '\0' || *lhsItr <= *rhs) return false;
	}
	return *rhs == '\0';
}

template<class Range>
bool Char::operator<(Char lhs, const Range& rhs){
	auto lhsItr = lhs.cbegin();
	const auto lhsEnd = lhs.cend();
	auto rhsItr = rhs.cbegin();
	const auto rhsEnd = rhs.cend();
	for(; lhsItr != lhsEnd && rhsItr != rhsEnd; ++lhsItr, (void)++rhsItr){
		if(*lhsItr >= *rhsItr) return false;
	}
	return lhsItr == lhsEnd && rhsItr == rhsEnd;
}

template<class Range>
bool Char::operator>(Char lhs, const Range& rhs){
	auto lhsItr = lhs.cbegin();
	const auto lhsEnd = lhs.cend();
	auto rhsItr = rhs.cbegin();
	const auto rhsEnd = rhs.cend();
	for(; lhsItr != lhsEnd && rhsItr != rhsEnd; ++lhsItr, (void)++rhsItr){
		if(*lhsItr <= *rhsItr) return false;
	}
	return lhsItr == lhsEnd && rhsItr == rhsEnd;
}

constexpr bool Char::operator<(Char lhs, char rhs){return lhs.to_int() < static_cast<Char::int_type>(rhs);}
constexpr bool Char::operator<(char lhs, Char rhs){return static_cast<Char::int_type>(lhs) < rhs.to_int();}
constexpr bool Char::operator<(Char lhs, Char rhs){return lhs.to_int() < rhs.to_int();}
constexpr bool Char::operator<(const char* lhs, Char rhs){return rhs > lhs;}
template<class Range> constexpr bool Char::operator<(const Range& lhs, Char rhs){return rhs > lhs;}


constexpr bool Char::operator>(Char lhs, char rhs){return rhs < lhs;}
constexpr bool Char::operator>(char lhs, Char rhs){return rhs < lhs;}
constexpr bool Char::operator>(Char lhs, Char rhs){return lhs.to_int() > rhs.to_int();}
constexpr bool Char::operator>(const char* lhs, Char rhs){return rhs < lhs;}
template<class Range> constexpr bool Char::operator>(const Range& lhs, Char rhs){return rhs < lhs;}

constexpr bool Char::operator<=(Char lhs, char rhs){return !(lhs > rhs);}
constexpr bool Char::operator<=(char lhs, Char rhs){return !(lhs > rhs);}
constexpr bool Char::operator<=(Char lhs, Char rhs){return lhs.to_int() <= rhs.to_int();}
constexpr bool Char::operator<=(Char lhs, const char* rhs){return !(lhs > rhs);}
constexpr bool Char::operator<=(const char* lhs, Char rhs){return !(rhs < lhs);}
template<class Range> constexpr bool Char::operator<=(Char lhs, const Range& rhs){return !(lhs > rhs);}
template<class Range> constexpr bool Char::operator<=(const Range& lhs, Char rhs){return !(rhs < lhs);}

constexpr bool Char::operator>=(Char lhs, char rhs){return !(lhs < rhs);}
constexpr bool Char::operator>=(char lhs, Char rhs){return !(lhs < rhs);}
constexpr bool Char::operator>=(Char lhs, Char rhs){return lhs.to_int() >= rhs.to_int();}
constexpr bool Char::operator>=(Char lhs, const char* rhs){return !(lhs < rhs);}
constexpr bool Char::operator>=(const char* lhs, Char rhs){return !(rhs > lhs);}
template<class Range> constexpr bool Char::operator>=(Char lhs, const Range& rhs){return !(lhs < rhs);}
template<class Range> constexpr bool Char::operator>=(const Range& lhs, Char rhs){return !(rhs > lhs);}


constexpr bool is_control(Char c){
	const auto i = c.to_int();
	return (0x0 <= i && i <= 0x1f) || (0x7f <= i && i <= 0x9f);
}

constexpr bool is_whitespace(Char c){
	return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

constexpr bool is_special(Char c) {
	const auto i = c.to_int();
	return (0x21 <= i && i <= 0x2f) || (0x3a <= i && i <= 0x40) || (0x5b <= i && i <= 0x60) || (0x7b <= i && i <= 0x7e);
}

constexpr bool is_ascii(Char c){
	const auto i = c.to_int();
	return (i <= 0x7f);
}

constexpr bool is_ascii_lower_alpha(Char c){
	return ('a' <= c && c <= 'z');
}

constexpr bool is_ascii_upper_alpha(Char c){
	return ('A' <= c && c <= 'Z');
}

constexpr bool is_ascii_alpha(Char c){
	return is_ascii_lower_alpha(c) || is_ascii_upper_alpha(c);
}

constexpr bool is_ascii_digit(Char c){
	return ('0' <= c && c <= '9');
}

constexpr bool is_ascii_alnum(Char c){
	return is_ascii_alpha(c) || is_ascii_digit(c);
}

constexpr bool is_basic_latin(Char c){
	auto i = c.to_int();
	return (0x0 <= i && i <= 0x7f);
}

constexpr bool is_latin_1_suplement(Char c){
	auto i = c.to_int();
	return (0x80 <= i && i <= 0xff);
}

constexpr bool is_latin_extended_a(Char c){
	auto i = c.to_int();
	return (0x100 <= i && i <= 0x17f);
}

constexpr bool is_latin_extended_b(Char c){
	auto i = c.to_int();
	return (0x180 <= i && i <= 0x24f);
}

constexpr bool is_ipa_extension(Char c){
	auto i = c.to_int();
	return (0x250 <= i && i <= 0x2af);
}

constexpr bool is_spaceing_modifier(Char c){
	auto i = c.to_int();
	return (0x2b0 <= i && i <= 0x2ff);
}

constexpr bool is_combining_diacritical_mark(Char c){
	auto i = c.to_int();
	return (0x300 <= i && i <= 0x36f);
}

constexpr bool is_greek(Char c){
	auto i = c.to_int();
	return (0x370 <= i && i <= 0x3ff);
}

constexpr bool is_cyrillic(Char c){
	auto i = c.to_int();
	return (0x400 <= i && i <= 0x4ff);
}

constexpr bool is_cyrillic_supplement(Char c){
	auto i = c.to_int();
	return (0x500 <= i && i <= 0x52f);
}

constexpr bool is_armenian(Char c){
	auto i = c.to_int();
	return (0x530 <= i && i <= 0x58f);
}

constexpr bool is_hebrew(Char c){
	auto i = c.to_int();
	return (0x590 <= i && i <= 0x5ff);
}

constexpr bool is_arabic(Char c){
	auto i = c.to_int();
	return (0x600 <= i && i <= 0x6ff);
}

constexpr bool is_syriac(Char c){
	auto i = c.to_int();
	return (0x700 <= i && i <= 0x74f);
}

constexpr bool is_arabic_suplement(Char c){
	auto i = c.to_int();
	return (0x750 <= i && i <= 0x77f);
}

constexpr bool is_thaana(Char c){
	auto i = c.to_int();
	return (0x780 <= i && i <= 0x7bf);
}

constexpr bool is_nko(Char c){
	auto i = c.to_int();
	return (0x7C0 <= i && i <= 0x7ff);
}

constexpr bool is_samaritan(Char c){
	auto i = c.to_int();
	return (0x800 <= i && i <= 0x83f);
}

constexpr bool is_mandaic(Char c){
	auto i = c.to_int();
	return (0x840 <= i && i <= 0x85f);
}

constexpr bool is_syriac_supplement(Char c){
	auto i = c.to_int();
	return (0x860 <= i && i <= 0x86f);
}

constexpr bool is_arabic_extended_b(Char c){
	auto i = c.to_int();
	return (0x870 <= i && i <= 0x89f);
}

constexpr bool is_arabic_extended_a(Char c){
	auto i = c.to_int();
	return (0x8a0 <= i && i <= 0x8ff);
}

constexpr bool is_devanagari(Char c){
	auto i = c.to_int();
	return (0x900 <= i && i <= 0x87f);
}

constexpr bool is_bengali(Char c){
	auto i = c.to_int();
	return (0x980 <= i && i <= 0x9ff);
}

}

#pragma once

// c++ std
#include <cinttypes>
#include <string>
#include <string_view>
#include <ostream>
#include <cinttypes>

// project
#include "char_reference.hpp"

namespace utf8{

enum class Identifier : std::int8_t;

constexpr bool is_start_byte(Identifier id);
constexpr const char* to_string(Identifier i);

/// returns the length of the utf8 sequence if it is the first character
/// returns '0' if it is a character in the sequence but not the first one
/// returns '-1' if it is an unsupported character by the utf8 standard
constexpr Identifier identify(char c);
constexpr bool is_start_byte(char c);

/**
	Stores an UTF8 symbol in a 4-byte wide buffer.
	
	It works and functions mostly as a container and also provides most standard container functions.
	
	It also has multiple functions that writes and prints this character into new or existing char strings,
	output streams and the like.
	
*/
class Char{
private:
	char utf8[4];
	
public:
	using value_type = char;
	using int_type = std::uint32_t;
	using size_type = std::size_t;
	using difference_type = long;
	using iterator = char*;
	using const_iterator = const char*;
	using reference = char&;
	using const_reference = const char&;
	using pointer = char*;
	using const_pointer = const char*;

	/// default constuctor constructable
	inline Char() = default;
	
	/// supports default copy construction
	constexpr inline Char(const Char&) = default;
	
	/// supports default copy assignment
	constexpr inline Char& operator=(const Char&) = default;
	
	/// construct from an ascii character
	constexpr inline Char(char ascii);
	
	constexpr inline Char(const char* character);
	
	constexpr inline Char(char32_t utf32);
	
	constexpr inline Char(std::int32_t utf8);
	
	/// reads one utf8 character from the range and stores it in the char
	template<class CharItr>
	constexpr inline explicit Char(CharItr first);
	
	/// reads one utf8 character from the range and stores it in the char, 
	/// makes sure to not read more than the buffer
	template<class CharItr>
	constexpr inline Char(CharItr first, CharItr last);
	
	template<class CharItr>
	constexpr inline Char(CharItr first, size_type n);
	
	/// reads first utf8 character from the string and makes sure not to read more than the strings content
	inline explicit Char(const std::string& str);
	
	/// reads first utf8 character from the string and makes sure not to read more than the strings content
	inline explicit Char(const std::string_view& str);
	
	/// assigns the ascii character to the utf8 character
	constexpr inline void assign(char ascii);
	
	/// reads the first utf8 character and stores it in the character, within the range
	/// returns the iterator to the next character after that.
	/// returns the same first iterator if the provided character is not the first byte of an utf8 encoded character
	template<class CharItr>
	constexpr inline CharItr assign(CharItr first, CharItr last);
	
	/// reads the first utf8 character and stores it in the character
	/// returns the iterator to the next character after that.
	/// returns the same iterator if the character is not a valid utf8 character
	template<class CharItr>
	constexpr inline CharItr assign(CharItr first);
	
	constexpr inline void assign(char_const_reference ch);
	constexpr inline void assign(char_reference ch);
	
	/// reads the first utf8 character from the string and stores it in the character
	/// returns the string iterator after the read character
	inline std::string::const_iterator assign(const std::string& str);
	
	/// reads the first utf8 character from the string and stores it in the character
	/// returns the string iterator after the read character
	inline std::string_view::const_iterator assign(const std::string_view& str);

	/// reads the first utf8 character from the string and stores it in the character
	/// returns the string iterator after the read character
	inline const_iterator assign(const_iterator str, size_type size);
	
	
	/// assigns an ascii character to the utf8 character
	constexpr inline Char& operator=(char c);
	
	template<class CharItr>
	constexpr inline Char& operator=(CharItr itr);
	
	inline Char& operator=(const std::string& str);
	inline Char& operator=(const std::string_view& str);
	
	/// returns the first character of the utf8 character string
	constexpr inline char& front();
	constexpr inline const char& front() const;
	
	/// returns the begin iterator of the utf8 character string
	constexpr inline iterator begin();
	constexpr inline const_iterator begin() const;
	constexpr inline const_iterator cbegin() const;

	/// returns the end iterator of the utf8 character string
	constexpr inline iterator end();
	constexpr inline const_iterator end() const;
	constexpr inline const_iterator cend() const;
	
	/// returns a string view object pointing to the utf8 symbol
	constexpr inline std::string_view to_std_string_view() const;
	
	/// returns the character at the index of the utf8 string
	constexpr inline reference operator[](size_type index);
	constexpr inline const_reference operator[](size_type index) const;
	
	/// returns the number of bytes that the unicode character needs
	constexpr inline size_type size() const;
	
	/// provides a pointer to the underlieing data
	constexpr inline pointer data();
	constexpr inline const_pointer data() const;
	
	/// writes this character into an std::string
	inline void print(std::string& str) const;
	
	/// turns this utf8 character string into an std::string
	inline std::string to_std_string() const;
	
	/// writes this char into an std::ostream useing the stream operator <<
	template<class OStream>
	friend inline std::ostream& operator << (OStream& stream, Char c);
	
	/// reads an utf8 char from the input stream
	template<class IStream>
	friend IStream& operator >> (IStream& stream, Char& c);
	
	constexpr inline int_type to_int() const;
	
	/// equality comparison with the same type
	friend constexpr inline bool operator==(Char lhs, Char rhs);
	
	friend constexpr inline bool operator==(Char lhs, char rhs);
	friend constexpr inline bool operator==(char lhs, Char rhs);
	
	/// equality comparison with a const char* string symbol that is null terminated
	friend constexpr bool operator==(Char lhs, const char* rhs);
	
	/// equality comparison with a const char* string symbol
	friend constexpr inline bool operator==(const char* lhs, Char rhs);
	
	/// equality comparison with a range. has to support cbegin() and cend() !
	template<class Range>
	friend bool operator==(Char lhs, const Range& rhs);
	
	friend constexpr inline bool operator!=(Char lhs, char rhs);
	friend constexpr inline bool operator!=(char lhs, Char rhs);
	
	/// equality comparison with a range. has to support cbegin() and cend() !
	template<class Range>
	friend constexpr inline bool operator==(const Range& lhs, Char rhs);
	
	/// unequality comparison with self
	friend constexpr inline bool operator!=(Char lhs, Char rhs);
	
	/// unequality comparison with c_string
	friend constexpr inline bool operator!=(Char lhs, const char* rhs);
	friend constexpr inline bool operator!=(const char* lhs, Char rhs);
	
	/// unequality comparison with char range
	template<class Range>
	friend inline bool operator!=(const Range& lhs, Char rhs);
	
	/// unequality comparison with char range
	template<class Range>
	friend inline bool operator!=(Char lhs, const Range& rhs);
	friend constexpr bool operator<(Char lhs, const char* rhs);
	friend constexpr bool operator>(Char lhs, const char* rhs);
	
	template<class Range>
	friend bool operator<(Char lhs, const Range& rhs);
	
	template<class Range>
	friend bool operator>(Char lhs, const Range& rhs);
	
	friend constexpr inline bool operator<(Char lhs, char rhs);
	friend constexpr inline bool operator<(char lhs, Char rhs);
	friend constexpr inline bool operator<(Char lhs, Char rhs);
	friend constexpr inline bool operator<(const char* lhs, Char rhs);
	template<class Range> friend constexpr inline bool operator<(const Range& lhs, Char rhs);
	
	
	friend constexpr inline bool operator>(Char lhs, char rhs);
	friend constexpr inline bool operator>(char lhs, Char rhs);
	friend constexpr inline bool operator>(Char lhs, Char rhs);
	friend constexpr inline bool operator>(const char* lhs, Char rhs);
	template<class Range> friend constexpr inline bool operator>(const Range& lhs, Char rhs);
	
	friend constexpr inline bool operator<=(Char lhs, char rhs);
	friend constexpr inline bool operator<=(char lhs, Char rhs);
	friend constexpr inline bool operator<=(Char lhs, Char rhs);
	friend constexpr inline bool operator<=(Char lhs, const char* rhs);
	friend constexpr inline bool operator<=(const char* lhs, Char rhs);
	template<class Range> friend constexpr inline bool operator<=(Char lhs, const Range& rhs);
	template<class Range> friend constexpr inline bool operator<=(const Range& lhs, Char rhs);
	
	friend constexpr inline bool operator>=(Char lhs, char rhs);
	friend constexpr inline bool operator>=(char lhs, Char rhs);
	friend constexpr inline bool operator>=(Char lhs, Char rhs);
	friend constexpr inline bool operator>=(Char lhs, const char* rhs);
	friend constexpr inline bool operator>=(const char* lhs, Char rhs);
	template<class Range> friend constexpr inline bool operator>=(Char lhs, const Range& rhs);
	template<class Range> friend constexpr inline bool operator>=(const Range& lhs, Char rhs);
};


inline constexpr bool is_control(Char c);
inline constexpr bool is_whitespace(Char c);
inline constexpr bool is_special(Char c);
inline constexpr bool is_ascii(Char c);
inline constexpr bool is_ascii_lower_alpha(Char c);
inline constexpr bool is_ascii_upper_alpha(Char c);
inline constexpr bool is_ascii_alpha(Char c);
inline constexpr bool is_ascii_digit(Char c);
inline constexpr bool is_ascii_alnum(Char c);
inline constexpr bool is_basic_latin(Char c);
inline constexpr bool is_latin_1_suplement(Char c);
inline constexpr bool is_latin_extended_a(Char c);
inline constexpr bool is_latin_extended_b(Char c);
inline constexpr bool is_ipa_extension(Char c);
inline constexpr bool is_spaceing_modifier(Char c);
inline constexpr bool is_combining_diacritical_mark(Char c);
inline constexpr bool is_greek(Char c);
inline constexpr bool is_cyrillic(Char c);
inline constexpr bool is_cyrillic_supplement(Char c);
inline constexpr bool is_armenian(Char c);
inline constexpr bool is_hebrew(Char c);
inline constexpr bool is_arabic(Char c);
inline constexpr bool is_syriac(Char c);
inline constexpr bool is_arabic_suplement(Char c);
inline constexpr bool is_thaana(Char c);
inline constexpr bool is_nko(Char c);
inline constexpr bool is_samaritan(Char c);
inline constexpr bool is_mandaic(Char c);
inline constexpr bool is_syriac_supplement(Char c);
inline constexpr bool is_arabic_extended_b(Char c);
inline constexpr bool is_arabic_extended_a(Char c);
inline constexpr bool is_devanagari(Char c);
inline constexpr bool is_bengali(Char c);

}
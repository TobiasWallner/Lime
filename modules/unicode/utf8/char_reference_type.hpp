#pragma once

#include "string_type.hpp"

namespace utf8{
	
class char_const_reference{
public:
	using iterator = char*;
	using const_iterator = const char*;
	using pointer = char*;
	using const_pointer = const char*;
	using size_type = size_t;
	
protected:	
	iterator _pos = nullptr;

public:
	constexpr char_const_reference() = default;
	constexpr char_const_reference(const char_const_reference&) = default;
	constexpr char_const_reference& operator= (const char_const_reference&) = default;

	constexpr char_const_reference(iterator pos);
	constexpr size_type size() const;
	
	constexpr operator utf8::Char () const;
	explicit constexpr operator std::string_view() const;
	
	constexpr const_pointer operator&() const;
	
	friend constexpr bool operator == (char_const_reference lhs, char_const_reference rhs);
	friend constexpr bool operator != (char_const_reference lhs, char_const_reference rhs);
	friend constexpr bool operator < (char_const_reference lhs, char_const_reference rhs);
	friend constexpr bool operator <= (char_const_reference lhs, char_const_reference rhs);
	friend constexpr bool operator > (char_const_reference lhs, char_const_reference rhs);
	friend constexpr bool operator >= (char_const_reference lhs, char_const_reference rhs);
};

class char_reference{
public:
	using iterator = char*;
	using const_iterator = const char*;
	using pointer = char*;
	using const_pointer = const char*;
	using size_type = size_t;

private:	
	utf8::string* _str = nullptr;
	iterator _pos = nullptr;
	
public:
	constexpr char_reference() = default;
	constexpr char_reference(const char_reference&) = default;
	constexpr char_reference& operator= (const char_reference&) = default;
	constexpr char_reference(utf8::string* str, iterator pos);
	
	constexpr size_type size() const;
	
	constexpr pointer operator&();

	constexpr operator const_reference();	
	constexpr operator utf8::Char () const;
	explicit constexpr operator std::string_view() const;
	
	constexpr char_reference& operator = (char c);
	constexpr char_reference& operator = (utf8::Char c);
};
	
} // namespace utf8
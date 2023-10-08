#pragma once

#include <iterator>

#include "char_type.hpp"
#include "char_reference_type.hpp"

namespace utf8{
	
class string_const_iterator : public std::bidirectional_iterator_tag {
private:
	const char* itr;
	
public:
	using value_type = utf8::Char;
	using reference = utf8::char_reference;
	using const_reference = utf8::char_const_reference;
	using size_type = size_t;
	using difference_type = long;
	using pointer = char*;
	using const_pointer = const char*;
	using iterator_category = std::contiguous_iterator_tag;

	constexpr string_const_iterator(const char* pos);
	constexpr string_const_iterator(const string_const_iterator&);
	constexpr string_const_iterator& operator=(const string_const_iterator&);
	
	constexpr const_reference operator*();
	constexpr string_const_iterator operator ++ ();
	constexpr string_const_iterator operator ++ (int);
	constexpr string_const_iterator operator -- ();
	constexpr string_const_iterator operator -- (int);
	
	friend constexpr bool operator == (string_const_iterator lhs, string_const_iterator rhs);
	friend constexpr bool operator != (string_const_iterator lhs, string_const_iterator rhs);
	friend constexpr bool operator < (string_const_iterator lhs, string_const_iterator rhs);
	friend constexpr bool operator <= (string_const_iterator lhs, string_const_iterator rhs);
	friend constexpr bool operator > (string_const_iterator lhs, string_const_iterator rhs);
	friend constexpr bool operator >= (string_const_iterator lhs, string_const_iterator rhs);
};

constexpr string_const_iterator scan_i32(string_const_iterator first, string_const_iterator last, std::int32_t& valueOut);

class string;

class string_iterator : std::contiguous_iterator_tag{
private:
	string* _str;
	char* itr;
	
public:
	using value_type = utf8::Char;
	using reference = utf8::char_reference;
	using const_reference = utf8::char_const_reference;
	using size_type = size_t;
	using difference_type = long;
	using pointer = char*;
	using const_pointer = const char*;
	using iterator_category = std::contiguous_iterator_tag;

	constexpr string_iterator(string* str, char* pos);
	constexpr string_iterator(const string_iterator&);
	constexpr string_iterator& operator=(const string_iterator&);
	
	constexpr string* container();
	constexpr const string* container() const;
	
	constexpr operator string_const_iterator();
	constexpr reference operator*();
	constexpr const_reference operator*() const;
	constexpr string_iterator operator ++ ();
	constexpr string_iterator operator ++ (int);
	constexpr string_iterator operator -- ();
	constexpr string_iterator operator -- (int);
};

constexpr size_t distance(string_const_iterator first, string_const_iterator last);

}

namespace std{
	template<>
	class iterator_traits<utf8::string_const_iterator>{
	public:
		using value_type = utf8::Char;
		using reference = utf8::char_reference;
		using const_reference = utf8::char_const_reference;
		using size_type = size_t;
		using difference_type = long;
		using pointer = char*;
		using const_pointer = const char*;
		using iterator_category = std::bidirectional_iterator_tag;
	};

	template<>
	class iterator_traits<utf8::string_iterator>{
	public:
		using value_type = utf8::Char;
		using reference = utf8::char_reference;
		using const_reference = utf8::char_const_reference;
		using size_type = size_t;
		using difference_type = long;
		using pointer = char*;
		using const_pointer = const char*;
		using iterator_category = std::bidirectional_iterator_tag;
	};
	
}
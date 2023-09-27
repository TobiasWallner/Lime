#pragma once

#include "char.hpp"
#include "char_reference.hpp"
#include "string.hpp"

namespace utf8{
	
class string_const_iterator{
private:
	const char* itr;
	
public:
	using const_reference = utf8::char_const_reference;

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

class string_iterator{
private:
	string* _str;
	char* itr;
	
public:
	using reference = utf8::char_reference;
	using const_reference = utf8::char_const_reference;

	constexpr string_iterator(const char* pos);
	constexpr string_iterator(const string_iterator&);
	constexpr string_iterator& operator(const string_iterator&);
	
	constexpr operator string_const_iterator();
	constexpr reference operator*();
	constexpr string_iterator operator ++ (int);
	constexpr string_iterator operator -- ();
	constexpr string_iterator operator -- (int);
};

}

namespace std{

constexpr sizt_t distance(utf8::string_const_iterator first, utf8::string_const_iterator last);

}
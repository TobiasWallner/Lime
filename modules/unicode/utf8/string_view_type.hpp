#pragma once

#include <cinttypes>

#include "string_type.hpp"
#include "string_iterator_type.hpp"
#include "char_reference_type.hpp"


namespace utf8{

class string_view{
public:
	using size_type = utf8::string::size_type;
	using iterator = utf8::string_iterator;
	using const_iterator = utf8::string_const_iterator;
	using reference = utf8::char_reference;
	using const_reference = utf8::char_const_reference;
	
private:
	utf8::string* _str;
	const_iterator _begin;
	const_iterator _end;
	
public:
	
	constexpr string_view(utf8::string* str, const char* first, const char* last);
	constexpr string_view(utf8::string* str, const_iterator first, const_iterator last);
	constexpr string_view(iterator first, iterator last);
	constexpr string_view(const string_view& other);
	constexpr string_view(utf8::string& str);
	constexpr string_view& operator=(const string_view& other);
	
	constexpr iterator begin();
	constexpr const_iterator begin() const;
	constexpr const_iterator cbegin() const;
	
	constexpr iterator end();
	constexpr const_iterator end() const;
	constexpr const_iterator cend() const;
	
	constexpr size_type size() const;
	constexpr size_type length() const;
	
	constexpr bool empty() const;
	
	constexpr void swap(string_view& other);
};

class const_string_view{
	public:
	using size_type = utf8::string::size_type;
	using iterator = utf8::string_iterator;
	using const_iterator = utf8::string_const_iterator;
	using reference = utf8::char_reference;
	using const_reference = utf8::char_const_reference;
	
private:
	const_iterator _begin;
	const_iterator _end;
	
public:
	constexpr const_string_view(const char* str);
	constexpr const_string_view(const char* first, const char* last);
	constexpr const_string_view(const_iterator first, const_iterator last);
	constexpr const_string_view(const utf8::string& str);
	constexpr const_string_view(const string_view& other);
	constexpr const_string_view& operator=(const string_view& other);
	constexpr const_string_view(const const_string_view& other);
	constexpr const_string_view& operator=(const const_string_view& other);
	
	constexpr iterator begin();
	constexpr const_iterator begin() const;
	constexpr const_iterator cbegin() const;
	
	constexpr iterator end() ;
	constexpr const_iterator end() const;
	constexpr const_iterator cend() const;
	
	constexpr size_type size() const;
	constexpr size_type length() const;

	constexpr bool empty() const;
	constexpr void swap(const_string_view& other);
};

}
#pragma once

#include <cinttypes>

#include "string_iterator.hpp"
#include "char_reference.hpp"


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
	
	constexpr string_view(utf8::string* str, const char* first, const char* last) : _str(str), _begin(first), _end(last);
	constexpr string_view(utf8::string* str, const_iterator first, const_iterator last) : _str(str), _begin(first), _end(last);
	constexpr string_view(const string_view& other) = default;
	constexpr string_view& operator=(const string_view& other) = default;
	
	constexpr iterator begin() {return _str->unconst(_begin);}
	constexpr const_iterator begin() const {return _begin;}
	constexpr const_iterator cbegin() const {return _begin;}
	
	constexpr iterator end() {return _str->unconst(_end);}
	constexpr const_iterator end() const {return _end;}
	constexpr const_iterator cend() const {return _end;}
	
	constexpr size_type size() const {return std::distance(this->cbegin(), this->cend());}
	constexpr size_type length() const {return std::distance(this->cbegin(), this->cend());}
	
	constexpr bool empty() const {return this->cbegin() == this->cend();}
	
	constexpr void swap(string_view& other){
		std::swap(this->_str, other.str);
		std::swap(this->_begin, other._begin);
		std::swap(this->_end, other._end);
	}
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
	constexpr const_string_view(const char* first, const char* last) : _begin(first), _end(last);
	constexpr const_string_view(const_iterator first, const_iterator last) : _begin(first), _end(last);
	constexpr const_string_view(const string_view& other) = default;
	constexpr const_string_view& operator=(const string_view& other) = default;
	constexpr const_string_view(const const_string_view& other) = default;
	constexpr const_string_view& operator=(const const_string_view& other) = default;
	
	constexpr iterator begin() {return _begin;}
	constexpr const_iterator begin() const {return _begin;}
	constexpr const_iterator cbegin() const {return _begin;}
	
	constexpr iterator end() {return _end;}
	constexpr const_iterator end() const {return _end;}
	constexpr const_iterator cend() const {return _end;}
	
	constexpr size_type size() const {return std::distance(this->begin(), this->end());}
	constexpr size_type length() const {return std::distance(this->begin(), this->end());}
	
	constexpr bool empty() const {return this->begin() == this->end();}
	
	constexpr void swap(string_view& other){
		std::swap(this->_str, other.str);
		std::swap(this->_begin, other._begin);
		std::swap(this->_end, other._end);
	}
};

}
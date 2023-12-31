#pragma once

#include <cinttypes>
#include <memory>

#include "common.hpp"
#include "string_view_type.hpp"
#include "string_iterator.hpp"
#include "char_reference.hpp"


namespace utf8{


constexpr const_string_view::const_string_view(const utf8::string& str) : _begin(str.begin()), _end(str.end()) {}
constexpr const_string_view::const_string_view(const char* str) : _begin(str), _end(strend(str)){}
constexpr const_string_view::const_string_view(const char* first, const char* last) : _begin(first), _end(last){}
constexpr const_string_view::const_string_view(const_string_view::const_iterator first, const_string_view::const_iterator last) : _begin(first), _end(last){}
constexpr const_string_view::const_string_view(std::string_view other) : _begin(std::to_address(other.begin())), _end(std::to_address(other.end())){}
constexpr const_string_view& const_string_view::operator=(std::string_view other) {this->_begin = std::to_address(other.begin()); this->_end = std::to_address(other.end());return *this;}

constexpr const_string_view::const_iterator const_string_view::begin() const {return _begin;}
constexpr const_string_view::const_iterator const_string_view::cbegin() const {return _begin;}

constexpr const_string_view::const_iterator const_string_view::end() const {return _end;}
constexpr const_string_view::const_iterator const_string_view::cend() const {return _end;}

constexpr const_string_view::size_type const_string_view::size() const {return utf8::distance(this->begin(), this->end());}
constexpr const_string_view::size_type const_string_view::length() const {return utf8::distance(this->begin(), this->end());}

constexpr bool const_string_view::empty() const {return this->begin() == this->end();}

constexpr void const_string_view::swap(const_string_view& other){
	std::swap(this->_begin, other._begin);
	std::swap(this->_end, other._end);
}

constexpr const_string_view::operator std::string_view() const {return std::string_view(std::to_address(this->cbegin()), std::to_address(std::to_address(this->cend())));}

constexpr bool operator == (const_string_view lhs, const_string_view rhs){ return static_cast<std::string_view>(lhs) == static_cast<std::string_view>(rhs); }
constexpr bool operator == (const char* lhs, const_string_view rhs) { return static_cast<std::string_view>(lhs) == static_cast<std::string_view>(rhs); }
constexpr bool operator == (std::string_view lhs, const_string_view rhs) { return lhs == static_cast<std::string_view>(rhs); }
constexpr bool operator == (const_string_view lhs, const char* rhs) { return static_cast<std::string_view>(lhs) == static_cast<std::string_view>(rhs); }
constexpr bool operator == (const_string_view lhs, std::string_view rhs) { return static_cast<std::string_view>(lhs) == rhs; }

constexpr bool operator != (const_string_view lhs, const_string_view rhs) { return static_cast<std::string_view>(lhs) != static_cast<std::string_view>(rhs); }
constexpr bool operator != (const char* lhs, const_string_view rhs) { return static_cast<std::string_view>(lhs) != static_cast<std::string_view>(rhs); }
constexpr bool operator != (std::string_view lhs, const_string_view rhs) { return lhs != static_cast<std::string_view>(rhs); }
constexpr bool operator != (const_string_view lhs, const char* rhs) { return static_cast<std::string_view>(lhs) != static_cast<std::string_view>(rhs); }
constexpr bool operator != (const_string_view lhs, std::string_view rhs) { return static_cast<std::string_view>(lhs) != rhs; }




constexpr string_view::string_view(utf8::string* str, const char* first, const char* last) : _str(str), _begin(first), _end(last){}
constexpr string_view::string_view(utf8::string* str, const_iterator first, const_iterator last) : _str(str), _begin(first), _end(last){}
constexpr string_view::string_view(string_view::iterator first, string_view::iterator last) : _str(first.container()), _begin(first), _end(last){}
constexpr string_view::string_view(utf8::string& str) : _str(&str), _begin(str.cbegin()), _end(str.cend()){}

constexpr string_view::iterator string_view::begin() {return _str->unconst(_begin);}
constexpr string_view::const_iterator string_view::begin() const {return _begin;}
constexpr string_view::const_iterator string_view::cbegin() const {return _begin;}

constexpr string_view::iterator string_view::end() {return _str->unconst(_end);}
constexpr string_view::const_iterator string_view::end() const {return _end;}
constexpr string_view::const_iterator string_view::cend() const {return _end;}

constexpr string_view::size_type string_view::size() const {return utf8::distance(this->cbegin(), this->cend());}
constexpr string_view::size_type string_view::length() const {return utf8::distance(this->cbegin(), this->cend());}

constexpr bool string_view::empty() const {return this->cbegin() == this->cend();}

constexpr void string_view::swap(string_view& other){
	std::swap(this->_str, other._str);
	std::swap(this->_begin, other._begin);
	std::swap(this->_end, other._end);
}

constexpr string_view::operator std::string_view() const {return std::string_view(std::to_address(this->cbegin()), std::to_address(this->cend()));}
constexpr string_view::operator utf8::const_string_view() const{return utf8::const_string_view(std::to_address(this->cbegin()), std::to_address(this->cend()));}


constexpr bool operator == (string_view lhs, string_view rhs) { return static_cast<std::string_view>(lhs) == static_cast<std::string_view>(rhs); }
constexpr bool operator == (const char* lhs, string_view rhs) { return static_cast<std::string_view>(lhs) == static_cast<std::string_view>(rhs); }
constexpr bool operator == (std::string_view lhs, string_view rhs) { return lhs == static_cast<std::string_view>(rhs); }
constexpr bool operator == (const_string_view lhs, string_view rhs) { return static_cast<std::string_view>(lhs) == static_cast<std::string_view>(rhs); }
constexpr bool operator == (string_view lhs, const char* rhs) { return static_cast<std::string_view>(lhs) == static_cast<std::string_view>(rhs); }
constexpr bool operator == (string_view lhs, std::string_view rhs) { return static_cast<std::string_view>(lhs) == rhs; }
constexpr bool operator == (string_view lhs, const_string_view rhs) { return static_cast<std::string_view>(lhs) == static_cast<std::string_view>(rhs); }

constexpr bool operator != (string_view lhs, string_view rhs) { return static_cast<std::string_view>(lhs) != static_cast<std::string_view>(rhs); }
constexpr bool operator != (const char* lhs, string_view rhs) { return static_cast<std::string_view>(lhs) != static_cast<std::string_view>(rhs); }
constexpr bool operator != (std::string_view lhs, string_view rhs) { return lhs != static_cast<std::string_view>(rhs); }
constexpr bool operator != (const_string_view lhs, string_view rhs) { return static_cast<std::string_view>(lhs) != static_cast<std::string_view>(rhs); }
constexpr bool operator != (string_view lhs, const char* rhs) { return static_cast<std::string_view>(lhs) != static_cast<std::string_view>(rhs); }
constexpr bool operator != (string_view lhs, std::string_view rhs) { return static_cast<std::string_view>(lhs) != rhs; }
constexpr bool operator != (string_view lhs, const_string_view rhs) { return static_cast<std::string_view>(lhs) != static_cast<std::string_view>(rhs); }

}
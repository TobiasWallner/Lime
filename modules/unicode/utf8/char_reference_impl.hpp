#pragma once

#include "char_reference_type.hpp"
#include "char.hpp"
#include "string.hpp"

namespace utf8{
	
constexpr char_const_reference::char_const_reference(const_iterator pos) : _pos(pos){}

constexpr char_const_reference::const_pointer char_const_reference::operator&() const {return this->_pos;}
constexpr char_const_reference::size_type char_const_reference::size() const {return static_cast<size_type>(utf8::identify(*this->_pos));}

constexpr char_const_reference::operator std::string_view() const {return std::string_view(this->_pos, this->size());}

constexpr const char& char_const_reference::operator[] (size_type i) const {return this->_pos[i];}

constexpr bool operator == (char_const_reference lhs, char_const_reference rhs){ return utf8::Char(lhs) == utf8::Char(rhs); }
constexpr bool operator == (char lhs, char_const_reference rhs){ return utf8::Char(lhs) == utf8::Char(rhs); }
constexpr bool operator == (char_const_reference lhs, char rhs){ return utf8::Char(lhs) == utf8::Char(rhs); }

constexpr bool operator != (char_const_reference lhs, char_const_reference rhs){ return utf8::Char(lhs) != utf8::Char(rhs); }
constexpr bool operator != (char lhs, char_const_reference rhs){ return utf8::Char(lhs) != utf8::Char(rhs); }
constexpr bool operator != (char_const_reference lhs, char rhs){ return utf8::Char(lhs) != utf8::Char(rhs); }

constexpr bool operator < (char_const_reference lhs, char_const_reference rhs){ return utf8::Char(lhs) < utf8::Char(rhs); }
constexpr bool operator < (char lhs, char_const_reference rhs){ return utf8::Char(lhs) < utf8::Char(rhs); }
constexpr bool operator < (char_const_reference lhs, char rhs){ return utf8::Char(lhs) < utf8::Char(rhs); }

constexpr bool operator <= (char_const_reference lhs, char_const_reference rhs){ return utf8::Char(lhs) <= utf8::Char(rhs); }
constexpr bool operator <= (char lhs, char_const_reference rhs){ return utf8::Char(lhs) <= utf8::Char(rhs); }
constexpr bool operator <= (char_const_reference lhs, char rhs){ return utf8::Char(lhs) <= utf8::Char(rhs); }

constexpr bool operator > (char_const_reference lhs, char_const_reference rhs){ return utf8::Char(lhs) > utf8::Char(rhs); }
constexpr bool operator > (char lhs, char_const_reference rhs){ return utf8::Char(lhs) > utf8::Char(rhs); }
constexpr bool operator > (char_const_reference lhs, char rhs){ return utf8::Char(lhs) > utf8::Char(rhs); }

constexpr bool operator >= (char_const_reference lhs, char_const_reference rhs){ return utf8::Char(lhs) >= utf8::Char(rhs); }
constexpr bool operator >= (char lhs, char_const_reference rhs){ return utf8::Char(lhs) >= utf8::Char(rhs); }
constexpr bool operator >= (char_const_reference lhs, char rhs){ return utf8::Char(lhs) >= utf8::Char(rhs); }

constexpr char_reference::char_reference(utf8::string* str, char_reference::iterator pos) : _str(str), _pos(pos){}

constexpr char_reference::operator char_const_reference(){return char_const_reference(this->_pos);}

constexpr char_reference::size_type char_reference::size() const {return static_cast<char_reference::size_type>(utf8::identify(*this->_pos));}

constexpr char_reference::operator std::string_view() const {return std::string_view(this->_pos, this->size());}

constexpr char_reference& char_reference::operator = (char c){this->_str->replace(this->_pos, c); return *this;}
constexpr char_reference& char_reference::operator = (const utf8::Char& c){this->_str->replace(this->_pos, c);return *this;}	

constexpr char_reference::pointer char_reference::operator&(){return this->_pos;}
	
constexpr char& char_reference::operator[] (size_type i){return this->_pos[i];}
constexpr const char& char_reference::operator[]  (size_type i) const {return this->_pos[i];}
	
constexpr bool operator == (char_reference lhs, char_reference rhs) {return utf8::Char(lhs) == utf8::Char(rhs);}
constexpr bool operator == (char_reference lhs, char_const_reference rhs) {return utf8::Char(lhs) == utf8::Char(rhs);}
constexpr bool operator == (char_const_reference lhs, char_reference rhs) {return utf8::Char(lhs) == utf8::Char(rhs);}
constexpr bool operator == (char lhs, char_reference rhs) {return utf8::Char(lhs) == utf8::Char(rhs);}
constexpr bool operator == (char_reference lhs, char rhs) {return utf8::Char(lhs) == utf8::Char(rhs);}

constexpr bool operator != (char_reference lhs, char_reference rhs) {return utf8::Char(lhs) != utf8::Char(rhs);}
constexpr bool operator != (char_reference lhs, char_const_reference rhs) {return utf8::Char(lhs) != utf8::Char(rhs);}
constexpr bool operator != (char_const_reference lhs, char_reference rhs) {return utf8::Char(lhs) != utf8::Char(rhs);}
constexpr bool operator != (char lhs, char_reference rhs) {return utf8::Char(lhs) != utf8::Char(rhs);}
constexpr bool operator != (char_reference lhs, char rhs) {return utf8::Char(lhs) != utf8::Char(rhs);}

constexpr bool operator < (char_reference lhs, char_reference rhs) {return utf8::Char(lhs) < utf8::Char(rhs);}
constexpr bool operator < (char_reference lhs, char_const_reference rhs) {return utf8::Char(lhs) < utf8::Char(rhs);}
constexpr bool operator < (char_const_reference lhs, char_reference rhs) {return utf8::Char(lhs) < utf8::Char(rhs);}
constexpr bool operator < (char lhs, char_reference rhs) {return utf8::Char(lhs) < utf8::Char(rhs);}
constexpr bool operator < (char_reference lhs, char rhs) {return utf8::Char(lhs) < utf8::Char(rhs);}

constexpr bool operator <= (char_reference lhs, char_reference rhs) {return utf8::Char(lhs) <= utf8::Char(rhs);}
constexpr bool operator <= (char_reference lhs, char_const_reference rhs) {return utf8::Char(lhs) <= utf8::Char(rhs);}
constexpr bool operator <= (char_const_reference lhs, char_reference rhs) {return utf8::Char(lhs) <= utf8::Char(rhs);}
constexpr bool operator <= (char lhs, char_reference rhs) {return utf8::Char(lhs) <= utf8::Char(rhs);}
constexpr bool operator <= (char_reference lhs, char rhs) {return utf8::Char(lhs) <= utf8::Char(rhs);}

constexpr bool operator > (char_reference lhs, char_reference rhs) {return utf8::Char(lhs) > utf8::Char(rhs);}
constexpr bool operator > (char_reference lhs, char_const_reference rhs) {return utf8::Char(lhs) > utf8::Char(rhs);}
constexpr bool operator > (char_const_reference lhs, char_reference rhs) {return utf8::Char(lhs) > utf8::Char(rhs);}
constexpr bool operator > (char lhs, char_reference rhs) {return utf8::Char(lhs) > utf8::Char(rhs);}
constexpr bool operator > (char_reference lhs, char rhs) {return utf8::Char(lhs) > utf8::Char(rhs);}

constexpr bool operator >= (char_reference lhs, char_reference rhs) {return utf8::Char(lhs) >= utf8::Char(rhs);}
constexpr bool operator >= (char_reference lhs, char_const_reference rhs) {return utf8::Char(lhs) >= utf8::Char(rhs);}
constexpr bool operator >= (char_const_reference lhs, char_reference rhs) {return utf8::Char(lhs) >= utf8::Char(rhs);}
constexpr bool operator >= (char lhs, char_reference rhs) {return utf8::Char(lhs) >= utf8::Char(rhs);}
constexpr bool operator >= (char_reference lhs, char rhs) {return utf8::Char(lhs) >= utf8::Char(rhs);}
} // namespace utf8
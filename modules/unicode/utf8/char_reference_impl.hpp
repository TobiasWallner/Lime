#pragma once

#include "char_reference_type.hpp"
#include "string.hpp"

namespace utf8{
	
constexpr char_const_reference::char_const_reference(char_const_reference::iterator pos) : _pos(pos){}

constexpr char_const_reference::const_pointer char_const_reference::operator&() const {return this->_pos;}

constexpr char_const_reference::size_type char_const_reference::size() const {return static_cast<size_type>(utf8::identify(*this->_pos));}

constexpr char_const_reference::operator utf8::Char () const {return utf8::Char(&*this->_pos);}
explicit constexpr char_const_reference::operator std::string_view() const {return std::string_view(this->_pos, this->size());}

friend constexpr bool char_const_reference::operator == (char_const_reference::char_const_reference lhs, char_const_reference::char_const_reference rhs){ return utf8::Char(lhs) == utf8::Char(rhs); }
friend constexpr bool char_const_reference::operator != (char_const_reference::char_const_reference lhs, char_const_reference::char_const_reference rhs){ return utf8::Char(lhs) != utf8::Char(rhs); }
friend constexpr bool char_const_reference::operator < (char_const_reference::char_const_reference lhs, char_const_reference::char_const_reference rhs){ return utf8::Char(lhs) < utf8::Char(rhs); }
friend constexpr bool char_const_reference::operator <= (char_const_reference::char_const_reference lhs, char_const_reference::char_const_reference rhs){ return utf8::Char(lhs) <= utf8::Char(rhs); }
friend constexpr bool char_const_reference::operator > (char_const_reference::char_const_reference lhs, char_const_reference::char_const_reference rhs){ return utf8::Char(lhs) > utf8::Char(rhs); }
friend constexpr bool char_const_reference::operator >= (char_const_reference::char_const_reference lhs, char_const_reference::char_const_reference rhs){ return utf8::Char(lhs) >= utf8::Char(rhs); }


constexpr char_reference::char_reference(utf8::string* str, char_reference::iterator pos) : _str(str), _pos(pos){}

constexpr operator char_reference::const_reference(){return char_reference::const_reference(this->_pos);}

constexpr char_reference::size_type char_reference::size() const {return static_cast<char_reference::size_type>(utf8::identify(*this->_pos));}

constexpr char_reference::operator utf8::Char () const {return utf8::Char(&*iterator);}
explicit constexpr char_reference::operator std::string_view() const {return std::string_view(this->_pos, this->size());}

constexpr char_reference::char_reference& char_reference::operator = (char c){
	*this->_pos = c;
	const auto erase_from = this->_pos+1;
	const auto erase_to = this->_pos + this->size();
	str.erase(erase_from, erase_to);
	return *this;
}

constexpr char_reference::pointer char_reference::operator&(){return this->_pos;}

constexpr char_reference::char_reference& char_reference::operator = (utf8::Char c){
	if(this->size() > c.size()){
		std::copy(c.begin(), c.end(), this->_pos);
		this->_str->erase(this->_pos + c.size(), this->_pos + lhs_size);
	}else if(this->size() < c.size()){
		std::copy(c.begin(), c.begin() + this->size(), this->_pos);
		this->_str->insert(this->_pos + this->size(), c.begin() + this->size(), c.begin() + c.size())
	}else{
		std::copy(c.begin(), c.end(), this->_pos);
	}
	return *this;
}	

	
} // namespace utf8
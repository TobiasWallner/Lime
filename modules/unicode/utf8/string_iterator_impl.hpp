#pragma once

#include <algorithm>

#include "string_iterator_type.hpp"
#include "char.hpp"
#include "char_reference.hpp"

namespace utf8{

constexpr string_const_iterator::string_const_iterator(const char* pos) : itr(pos){}
constexpr string_const_iterator::string_const_iterator(const string_const_iterator&) = default;
constexpr string_const_iterator& string_const_iterator::operator=(const string_const_iterator&) = default;

constexpr string_const_iterator::const_reference string_const_iterator::operator*(){return string_const_iterator::const_reference(this->itr);}
constexpr string_const_iterator string_const_iterator::operator ++ (){
	this->itr += static_cast<std::int32_t>(utf8::identify(*(this->itr)));
	return *this;
}

constexpr string_const_iterator string_const_iterator::operator ++ (int){
	const auto copy = *this;
	++*this;
	return copy;
}

constexpr string_const_iterator string_const_iterator::operator -- (){
	while(!utf8::is_start_byte(*(this->itr))){
		--this->itr;
	}
	return *this;
}

constexpr string_const_iterator string_const_iterator::operator -- (int) {
	const auto copy = *this;
	--*this;
	return copy;
}

constexpr bool operator == (string_const_iterator lhs, string_const_iterator rhs){return lhs.itr == rhs.itr;}
constexpr bool operator != (string_const_iterator lhs, string_const_iterator rhs){return lhs.itr != rhs.itr;}
constexpr bool operator < (string_const_iterator lhs, string_const_iterator rhs){return lhs.itr < rhs.itr;}
constexpr bool operator <= (string_const_iterator lhs, string_const_iterator rhs){return lhs.itr <= rhs.itr;}
constexpr bool operator > (string_const_iterator lhs, string_const_iterator rhs){return lhs.itr > rhs.itr;}
constexpr bool operator >= (string_const_iterator lhs, string_const_iterator rhs){return lhs.itr >= rhs.itr;}

constexpr string_const_iterator scan_i32(string_const_iterator first, string_const_iterator last, std::int32_t& valueOut){
	if(first == last) return first;
	
	std::int32_t result = 0;
	bool sign = false;
	
	if(*first == '-'){
		sign = true;
		++first;
	}else if(*first == '+'){
		++first;
	}
	
	for(; first != last; ++first){
		result *= 10;
		if('0' <= *first && *first <= '9'){
			result += (*first)[0] - '0';
		}else{
			break;
		}
	}
	
	valueOut = (sign) ? -result : result;	
	return first;
}

constexpr string_const_iterator::const_pointer string_const_iterator::operator->() const {return this->itr;}

constexpr string_iterator::string_iterator(string* str, char* pos) : _str(str), itr(pos){}
constexpr string_iterator::string_iterator(const string_iterator&) = default;
constexpr string_iterator& string_iterator::operator=(const string_iterator&) = default;

constexpr string_iterator::operator string_const_iterator() {return string_const_iterator(this->itr);}


constexpr string_iterator::reference string_iterator::operator*(){return reference(this->_str, this->itr);}
constexpr string_iterator::const_reference string_iterator::operator*() const {return const_reference(this->itr);}
constexpr string_iterator string_iterator::operator ++ (){
	this->itr += static_cast<std::int32_t>(utf8::identify(*(this->itr)));
	return *this;
}

constexpr string_iterator string_iterator::operator ++ (int){
	const auto copy = *this;
	++*this;
	return copy;
}

constexpr string_iterator string_iterator::operator -- (){
	while(!utf8::is_start_byte(*(this->itr))){
		--this->itr;
	}
	return *this;
}

constexpr string_iterator string_iterator::operator -- (int) {
	const auto copy = *this;
	--*this;
	return copy;
}

constexpr string* string_iterator::container() {return this->_str;}
constexpr const string* string_iterator::container() const {return this->_str;}

constexpr size_t distance(string_const_iterator first, string_const_iterator last){
	size_t count = 0;
	while(first != last){
		++first;
		++count;
	}
	return count;
}

constexpr string_iterator::pointer string_iterator::operator->(){return this->itr;}
constexpr string_iterator::const_pointer string_iterator::operator->() const {return this->itr;}

}

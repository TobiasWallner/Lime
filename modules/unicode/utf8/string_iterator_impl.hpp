#pragma once

#include "string_iterator_type.hpp"
#include "char.hpp"
#include "char_reference.hpp"

namespace utf8{

constexpr string_const_iterator::string_const_iterator(const char* pos) : itr(pos){}
constexpr string_const_iterator::string_const_iterator(const string_const_iterator&) = default;
constexpr string_const_iterator::string_const_iterator& operator=(const string_const_iterator&) = default;

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

friend constexpr bool operator == (string_const_iterator lhs, string_const_iterator rhs){return lhs.itr == rhs.itr;}
friend constexpr bool operator != (string_const_iterator lhs, string_const_iterator rhs){return lhs.itr != rhs.itr;}
friend constexpr bool operator < (string_const_iterator lhs, string_const_iterator rhs){return lhs.itr < rhs.itr;}
friend constexpr bool operator <= (string_const_iterator lhs, string_const_iterator rhs){return lhs.itr <= rhs.itr;}
friend constexpr bool operator > (string_const_iterator lhs, string_const_iterator rhs){return lhs.itr > rhs.itr;}
friend constexpr bool operator >= (string_const_iterator lhs, string_const_iterator rhs){return lhs.itr >= rhs.itr;}

constexpr string_iterator::string_iterator(const char* pos) : itr(pos){}
constexpr string_iterator::string_iterator(const string_iterator&) = default;
constexpr string_iterator::string_iterator& operator(const string_iterator&) = default;

constexpr string_iterator::operator string_const_iterator() {return string_const_iterator(this->itr);}

constexpr string_iterator::reference string_iterator::operator*(){return const_reference(this->itr);}
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

}

namespace std{

constexpr sizt_t distance(utf8::string_const_iterator first, utf8::string_const_iterator last){
	const char* _first = &*first;
	const char* _last = &*last;
	return std::count_if(_first, _last, utf8::is_start_byte);
}

}
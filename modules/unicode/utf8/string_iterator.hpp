#pragma once

#include <stdexcept>
#include "char.hpp"
#include "char_reference.hpp"

namespace utf8{
	
class string_const_iterator{
private:
	const char* itr;
	
public:
	using const_reference = utf8::char_const_reference;

	constexpr string_const_iterator(const char* pos) : itr(pos){}
	constexpr string_const_iterator(const string_const_iterator&) = default;
	constexpr string_const_iterator& operator=(const string_const_iterator&) = default;
	
	constexpr const_reference operator*(){return const_reference(this->itr);}
	constexpr string_const_iterator operator ++ (){
		this->itr += static_cast<std::int32_t>(utf8::identify(*(this->itr)));
		return *this;
	}
	
	constexpr string_const_iterator operator ++ (int){
		const auto copy = *this;
		++*this;
		return copy;
	}
	
	constexpr string_const_iterator operator -- (){
		while(!utf8::is_start_byte(*(this->itr))){
			--this->itr;
		}
		return *this;
	}
	
	constexpr string_const_iterator operator -- (int) {
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
	
}

class string;

class string_iterator{
private:
	string* _str;
	char* itr;
	
public:
	using reference = utf8::char_reference;
	using const_reference = utf8::char_const_reference;

	constexpr string_iterator(const char* pos) : itr(pos){}
	constexpr string_iterator(const string_iterator&) = default;
	constexpr string_iterator& operator(const string_iterator&) = default;
	
	constexpr operator string_const_iterator() {return string_const_iterator(this->itr);}
	
	constexpr reference operator*(){return const_reference(this->itr);}
	constexpr string_iterator operator ++ (){
		this->itr += static_cast<std::int32_t>(utf8::identify(*(this->itr)));
		return *this;
	}
	
	constexpr string_iterator operator ++ (int){
		const auto copy = *this;
		++*this;
		return copy;
	}
	
	constexpr string_iterator operator -- (){
		while(!utf8::is_start_byte(*(this->itr))){
			--this->itr;
		}
		return *this;
	}
	
	constexpr string_iterator operator -- (int) {
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
#pragma once

#include <type_traits>

#include "string_type.hpp"

#include "char.hpp"
#include "char_reference.hpp"
#include "string_iterator.hpp"
#include "string_view.hpp"
#include "common.hpp"

namespace utf8{

// constructors
constexpr string::string() : string::Base(){}
constexpr string::string(char ch){this->append(ch);}
constexpr string::string(utf8::Char ch){this->append(ch);}
constexpr string::string(string::size_type count, char ch) : string::Base(count, ch){}
constexpr string::string(string::size_type count, utf8::Char ch){this->append(count, ch);}
constexpr string::string(const string& other, string::size_type pos) : string::Base(other, pos){}
constexpr string::string(const string& other, string::size_type pos, string::size_type count) : string::Base(other, pos, count){}
constexpr string::string(const char* s, string::size_type count) : string::Base(s, count){}
constexpr string::string(const utf8::Char* s, string::size_type count){this->append(s, count);}
constexpr string::string(const char* s) : string::Base(s){}
constexpr string::string(const utf8::Char* s) : string::Base(){this->append(s);}
template<class InputIt> 
constexpr string::string(InputIt first, InputIt last) {this->append(first, last);}
constexpr string::string(const string& other) = default;
constexpr string::string(string&& other) = default;
template<class StringViewLike>
constexpr string::string(const StringViewLike& t) : string::Base(t){}
template<class StringViewLike>
constexpr string::string(const StringViewLike& t, string::size_type pos, string::size_type n) : string::Base(t, pos, n){}

// assignments
constexpr string& string::operator=(char ch){this->string::Base::operator=(ch); return *this;}
constexpr string& string::operator=(utf8::Char ch){return this->assign(ch);}
constexpr string& string::operator=(const string& str) = default;
constexpr string& string::operator=(string&& str) = default;
constexpr string& string::operator=(const char* s) {this->string::Base::operator=(s); return *this;}
template<class StringViewLike> 
constexpr string& string::operator=(const StringViewLike& t) {this->string::Base::operator=(t); return *this;}

constexpr string& string::assign(char ch){this->string::Base::operator=(ch); return *this;}
constexpr string& string::assign(utf8::Char ch){this->clear(); return this->append(ch);}
constexpr string& string::assign(string::size_type count, char ch) {this->string::Base::assign(count, ch); return *this;}
constexpr string& string::assign(string::size_type count, utf8::Char ch){
	this->clear();
	this->reserve(count * ch.size());
	for(auto itr = this->begin(); itr != this->end(); ++itr){
		*itr = ch;
	}
	return *this;
}
constexpr string& string::assign(const string& str) {this->string::Base::assign(str); return *this;}
constexpr string& string::assign(const string& str, string::size_type pos) {this->string::Base::assign(str, pos); return *this;}
constexpr string& string::assign(const string& str, string::size_type pos, string::size_type count) {this->string::Base::assign(str, pos, count); return *this;}
constexpr string& string::assign(string&& str) noexcept {this->string::Base::assign(std::move(str)); return *this;}
constexpr string& string::assign(const char* s, string::size_type count) {this->string::Base::assign(s, count); return *this;}
constexpr string& string::assign(const utf8::Char* s, string::size_type count) {this->clear(); return this->append(s, count);}
constexpr string& string::assign(const char* s) {this->string::Base::assign(s); return *this;}
constexpr string& string::assign(const utf8::Char* s) {this->clear(); return this->append(s);}
template<class InputIt> 
constexpr string& string::assign(InputIt first, InputIt last) {this->clear(); return this->append(first, last);}
template<class StringViewLike> 
constexpr string& string::assign(const StringViewLike& t) {return this->assign(t.begin(), t.end());}
template<class StringViewLike>
constexpr string& string::assign(const StringViewLike& t, size_type pos) {return this->assign(t.begin() + pos, t.end());}
template<class StringViewLike>
constexpr string& string::assign(const StringViewLike& t, size_type pos, size_type count) {return this->assign(t.begin() + pos, t.begin() + pos + count);}

// allocator
constexpr string::allocator_type string::get_allocator() const noexcept {return this->string::Base::get_allocator();}

// Element Access
constexpr string::reference string::front() {return string::reference(*this->begin());}
constexpr string::const_reference string::front() const {return string::const_reference(*this->cbegin());}

constexpr string::reference string::back() {return string::reference(*(--this->end()));}
constexpr string::const_reference string::back() const {return string::const_reference(*(--this->end()));}

constexpr char* string::data() noexcept {return this->string::Base::data();}
constexpr const char* string::data() const noexcept {return this->string::Base::data();}

constexpr const char* string::c_str() const noexcept {return this->string::Base::c_str();}

constexpr string::operator std::string_view() const noexcept {return this->Base::operator std::string_view();}

constexpr string::iterator string::begin() noexcept {return string::iterator(this, &*this->string::Base::begin());}
constexpr string::const_iterator string::begin() const noexcept {return string::const_iterator(&*this->string::Base::cbegin());}
constexpr string::const_iterator string::cbegin() const noexcept {return string::const_iterator(&*this->string::Base::cbegin());}

constexpr string::iterator string::end() noexcept {return string::iterator(this, &*this->string::Base::end());}
constexpr string::const_iterator string::end() const noexcept {return string::const_iterator(&*this->string::Base::end());}
constexpr string::const_iterator string::cend() const noexcept {return string::const_iterator(&*this->string::Base::end());}

//constexpr string::reverse_iterator string::rbegin() noexcept {return string::iterator(&*this->end());}
//constexpr string::const_reverse_iterator string::rbegin() const noexcept {return string::iterator(&*this->cend());}
//constexpr string::const_reverse_iterator string::crbegin() const noexcept {return string::iterator(&*this->cend());}

//constexpr string::iterator string::rend() noexcept {return string::iterator(&*this->begin());}
//constexpr string::const_reverse_iterator string::rend() const noexcept {return string::iterator(&*this->begin());}
//constexpr string::const_reverse_iterator string::crend() const noexcept {return string::iterator(&*this->begin());}

// Capacity
constexpr bool string::empty() const noexcept {return this->string::Base::empty();}
constexpr string::size_type string::byte_size() const noexcept {return this->string::Base::size();}
constexpr string::size_type string::size() const noexcept {return utf8::distance(this->begin(), this->end());}
constexpr string::size_type string::length() const noexcept {return utf8::distance(this->begin(), this->end());}
constexpr string::size_type string::max_size() const noexcept {return this->string::Base::max_size();}
constexpr void string::reserve(string::size_type new_cap) {this->string::Base::reserve(new_cap);}
constexpr string::size_type string::capacity() const noexcept {return this->string::Base::capacity();}
constexpr void string::shrink_to_fit() {this->string::Base::shrink_to_fit();}

// Operations
constexpr void string::clear() noexcept {this->string::Base::clear();}
constexpr string::iterator string::insert(string::const_iterator pos, char ascii){
	const std::size_t index = &*pos - &*this->cbegin();
	const utf8::string::Base::const_iterator itr = this->Base::cbegin() + index;
	char* r = &*this->string::Base::insert(itr, ascii);
	const string::iterator result(this, r);
	return result;
}	
constexpr string::iterator string::insert(string::const_iterator pos, utf8::Char ch){
	const std::size_t index = &*pos - &*this->cbegin();
	const utf8::string::Base::const_iterator itr = this->Base::cbegin() + index;
	char* r = &*this->string::Base::insert(itr, ch.cbegin(), ch.cend());
	const string::iterator result(this, r);
	return result;
}	
constexpr string::iterator string::insert(string::const_iterator pos, string::size_type count, char ascii){
	const std::size_t index = &*pos - &*this->cbegin();
	const utf8::string::Base::const_iterator itr = this->Base::cbegin() + index;
	char* r = &*this->string::Base::insert(itr, count, ascii);
	const string::iterator result(this, r);
	return result;
}	
constexpr string::iterator string::insert(string::const_iterator pos, string::size_type count, utf8::Char ch){
	const std::size_t index = &*pos - &*this->cbegin();
	const utf8::string::Base::const_iterator itr = this->Base::cbegin() + index;
	char* r = &*this->string::Base::insert(itr, count * ch.size(), ' ');
	for(string::size_type i = 0; i != count; (void)++i, (void)++pos){
		*pos = ch;
	}
	const string::iterator result(this, r);
	return result;
}
constexpr string::iterator string::insert(string::const_iterator pos, const char* str){
	const std::size_t index = &*pos - &*this->cbegin();
	const utf8::string::Base::const_iterator itr = this->Base::cbegin() + index;
	char* r = &*this->string::Base::insert(itr, str, utf8::strend(str));
	const string::iterator result(this, r);
	return result;
}
constexpr string::iterator string::insert(string::const_iterator pos, const char* str, string::size_type count){
	const std::size_t index = &*pos - &*this->cbegin();
	const utf8::string::Base::const_iterator itr = this->Base::cbegin() + index;
	char* r = &*this->string::Base::insert(itr, str, str + count);
	const string::iterator result(this, r);
	return result;
}
template< class InputIt >
constexpr string::iterator string::insert(string::const_iterator pos, InputIt first, InputIt last){
	const std::size_t index = &*pos - &*this->cbegin();
	const utf8::string::Base::const_iterator itr = this->Base::cbegin() + index;
	char* r = &*this->string::Base::insert(itr, first, last);
	const string::iterator result(this, r);
	return result;
}
constexpr string::iterator string::insert(string::const_iterator pos, std::initializer_list<char> ilist){
	const std::size_t index = &*pos - &*this->cbegin();
	const utf8::string::Base::const_iterator itr = this->Base::cbegin() + index;
	char* r = &*this->string::Base::insert(itr, ilist);
	const string::iterator result(this, r);
	return result;
}

constexpr string::iterator string::erase(string::const_iterator pos){
	const std::size_t index = &*pos - &*this->cbegin();
	const utf8::string::Base::const_iterator erase_from = this->Base::cbegin() + index;
	const utf8::string::Base::const_iterator erase_to = erase_from + (*pos).size();
	char* r = &*this->string::Base::erase(erase_from, erase_to);
	const string::iterator result(this, r);
	return result;
}
constexpr string::iterator string::erase(string::const_iterator first, string::const_iterator last){
	const std::size_t index_first = &*first - &*this->cbegin();
	const std::size_t index_last = &*last - &*this->cbegin();
	const utf8::string::Base::const_iterator erase_from = this->Base::cbegin() + index_first;
	const utf8::string::Base::const_iterator erase_to = this->Base::cbegin() + index_last + (*last).size();
	char* r = &*this->string::Base::erase(erase_from, erase_to);
	const string::iterator result(this, r);
	return result;
}

constexpr void string::push_back(char ascii){this->string::Base::push_back(ascii);}
constexpr void string::push_back(utf8::Char ch){this->string::Base::append(ch.begin(), ch.end());}

constexpr void string::pop_back(){this->erase(--this->end(), this->end());}
				
constexpr string& string::append(char ascii){this->push_back(ascii); return *this;}
constexpr string& string::append(utf8::Char ch){this->push_back(ch); return *this;}
constexpr string& string::append(string::size_type count, char ascii){this->string::Base::append(count, ascii); return *this;}
constexpr string& string::append(string::size_type count, utf8::Char ch){
	this->reserve(count * ch.size());
	for(string::size_type i = 0; i != count; ++count) this->push_back(ch);
	return *this;
}
constexpr string& string::append(const string& str){this->string::Base::append(&*str.begin(), &*str.end()); return *this;}
constexpr string& string::append(const string::Base& str){this->string::Base::append(str); return *this;}
constexpr string& string::append(const string::Base& str, string::size_type pos, string::size_type count){this->string::Base::append(str, pos, count); return *this;}
constexpr string& string::append(const char* str, string::size_type count){this->string::Base::append(str, count); return *this;}
constexpr string& string::append(const utf8::Char* str, string::size_type count){while(str != str + count) this->append(*str++); return *this;}
constexpr string& string::append(const char* str){this->string::Base::append(str); return *this;}
constexpr string& string::append(const utf8::Char* str){while(*str != '\0') this->append(*str++); return *this;}

template< class InputIt >
requires std::is_same<char, typename std::iterator_traits<InputIt>::value_type>::value
constexpr string& string::append(InputIt first, InputIt last){this->Base::append(first, last); return *this;}
template< class InputIt >
requires std::is_same<utf8::Char, typename std::iterator_traits<InputIt>::value_type>::value
constexpr string& string::append(InputIt first, InputIt last){
	size_t byte_size = 0;
	for(auto itr = first; itr != last; ++first) byte_size += itr.size();
	this->reserve(byte_size);
	for(auto itr = first; itr != last; ++first) this->push_back(*itr);
	return *this;
}
constexpr string& string::append(string_const_iterator first, string_const_iterator last){this->Base::append(&*first, &*last); return *this;}
constexpr string& string::append(string_iterator first, string_iterator last){this->Base::append(&*first, &*last); return *this;}
constexpr string& string::append(std::initializer_list<char> ilist){this->string::Base::append(ilist); return *this;}
template< class StringViewLike >
constexpr string& string::append(const StringViewLike& t){return this->append(t.cbegin(), t.cend());}
template< class StringViewLike >
constexpr string& string::append(const StringViewLike& t, string::size_type pos, string::size_type count){this->string::Base::append(t, pos, count); return *this;}

constexpr string& string::operator+=(char ascii){return this->append(ascii);}
constexpr string& string::operator+=(utf8::Char ch){return this->append(ch);}
constexpr string& string::operator+=(const string& str ){return this->append(str);}
constexpr string& string::operator+=(const string::Base& str ){return this->append(str);}
constexpr string& string::operator+=(const char* s){return this->append(s);}
template< class StringViewLike >
constexpr string& string::operator+=(const StringViewLike& t){return this->append(t);}

constexpr int string::compare(const string& str) const noexcept {
	if(this->Base::size() == str.Base::size()){
		return std::strncmp(this->data(), str.data(), this->Base::size());
	}
	return false;
}
constexpr int string::compare(const char* s) const {return this->string::Base::compare(s);}
template< class StringViewLike >
constexpr int string::compare(const StringViewLike& t) const {return this->string::Base::compare(t);}

template<class CharItr>
constexpr bool string::starts_with(CharItr first, CharItr last) const noexcept {return utf8::starts_with(this->begin(), this->end(), first, last);}
template<class StringViewLike>
constexpr bool string::starts_with(StringViewLike sv) const noexcept {return this->string::starts_with(sv.begin(), sv.end());}
constexpr bool string::starts_with(char ch) const noexcept {return this->string::Base::starts_with(ch);}
constexpr bool string::starts_with(utf8::Char ch) const noexcept {return this->starts_with(ch.to_std_string_view());}
constexpr bool string::starts_with(const char* str) const {return this->string::Base::starts_with(str);}

template<class CharItr>
constexpr bool string::ends_with(CharItr first, CharItr last) const noexcept {return utf8::ends_with(this->begin(), this->end(), first, last);}
template<class StringViewLike>
constexpr bool string::ends_with(StringViewLike sv) const noexcept {return this->string::starts_with(sv.begin(), sv.end());}
constexpr bool string::ends_with(char ch) const noexcept {return this->string::Base::ends_with(ch);}
constexpr bool string::ends_with(utf8::Char ch) const noexcept {return this->ends_with(ch.to_std_string_view());}
constexpr bool string::ends_with(const char* str) const {return this->string::Base::ends_with(str);}

template<class CharItr>
constexpr bool string::contains(CharItr first, CharItr last) const noexcept{return utf8::contains(this->begin(), this->end(), first, last);}
template<class StringViewLike>
constexpr bool string::contains(StringViewLike sv) const noexcept{return utf8::contains(this->begin(), this->end(), sv.begin(), sv.end());}
constexpr bool string::contains(char ch) const noexcept {return utf8::contains(this->begin(), this->end(), ch);}
constexpr bool string::contains(utf8::Char ch) const noexcept {return utf8::contains(this->begin(), this->end(), ch);}
constexpr bool string::contains(const char* str) const {return utf8::contains(this->begin(), this->end(), str);}

constexpr string& string::replace(string::const_iterator pos, char c){
	auto unconst_pos = this->unconst(pos);
	*&*unconst_pos = c;
	const auto erase_pos = (&*pos - &*this->Base::begin()) + 1;
	const auto erase_count = (*pos).size() - 1;
	this->Base::erase(erase_pos, erase_count);
	return *this;
}
constexpr string& string::replace(string::const_iterator pos, utf8::Char c){
	if((*pos).size() > c.size()){
		const auto copy_first = c.cbegin();
		const auto copy_last = c.cend();
		const auto dest = &*this->unconst(pos);
		std::copy(copy_first, copy_last, dest);
		
		const auto erase_pos = (&*pos - &*this->Base::cbegin()) + c.size();
		const auto erase_count = (*pos).size() - c.size();
		this->Base::erase(erase_pos, erase_count);
	}else if((*pos).size() < c.size()){
		const auto copy_first = c.cbegin();
		const auto copy_last = c.cbegin() + (*pos).size();
		const auto copy_dest = &*this->unconst(pos);
		std::copy(copy_first, copy_last, copy_dest);
		
		const auto insert_pos = this->Base::cbegin() + ((&*pos - &*this->cbegin()) + (*pos).size());
		const auto insert_first =  c.cbegin() + (*pos).size();
		const auto insert_last = c.cend();
		this->Base::insert(insert_pos, insert_first, insert_last);
	}else{
		const auto copy_first = c.cbegin();
		const auto copy_last = c.cend();
		const auto copy_dest = &*this->unconst(pos);
		std::copy(copy_first, copy_last, copy_dest);
	}
	return *this;
}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, const string& str) {return this->replace(first, last, str.cbegin(), str.cend());}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, const char* cstr, string::size_type count2) {
	return this->replace(first, last, string::const_iterator(cstr), string::const_iterator(cstr + count2));
}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, const char* cstr){
	auto thisItr = this->unconst(first);
	auto strItr = string::const_iterator(cstr);
	for(; thisItr != last && *strItr != '\0'; (void)++thisItr, (void)++strItr){
		*thisItr = *strItr;
	}
	return *this;
}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, string::size_type count2, char ch) {
	auto thisItr = this->unconst(first);
	string::size_type i = 0;
	for(; thisItr != last && i != count2; (void)++thisItr, (void)++i){
		*thisItr = ch;
	}
	return *this;
}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, string::size_type count2, utf8::Char ch) {
	auto thisItr = this->unconst(first);
	string::size_type i = 0;
	for(; thisItr != last && i != count2; (void)++thisItr, (void)++i){
		*thisItr = ch;
	}
	return *this;
}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, char ch) {
	auto thisItr = this->unconst(first);
	for(; thisItr != last; (void)++thisItr){
		*thisItr = ch;
	}
	return *this;
}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, utf8::Char ch) {
	auto thisItr = this->unconst(first);
	for(; thisItr != last; (void)++thisItr){
		*thisItr = ch;
	}
	return *this;
}
template< class InputIt >
constexpr string& string::replace(string::const_iterator first, string::const_iterator destLast, InputIt sourceFirst, InputIt sourceLast){
	for(auto itr = this->unconst(first); itr != destLast && sourceFirst != sourceLast; (void)++itr, (void)++sourceFirst){
		*itr = *sourceFirst;
	}
	return *this;
}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, std::initializer_list<char> ilist) {return this->replace(first, last, ilist.begin(), ilist.end());}
template< class StringViewLike >
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, const StringViewLike& t) {return this->replace(first, last, t.begin(), t.end());}

constexpr void string::resize(string::size_type count){this->string::Base::resize(count);}
constexpr void string::resize(string::size_type count, char ch){this->string::Base::resize(count, ch);}

constexpr void string::swap( string& other ) {this->string::Base::swap(other);}

// search

constexpr string::const_iterator string::find(const string& str, string::const_iterator pos) const noexcept {
	string::size_type foundPos = this->string::Base::find(str, utf8::distance(&*this->cbegin(), &*pos));
	return string::const_iterator(&*this->cbegin() + foundPos);
}

constexpr string::const_iterator string::find(const string& str) const noexcept {return this->find(str, this->cbegin());}

constexpr string::const_iterator string::find(const char* s, string::const_iterator pos, string::size_type count) const {
	string::size_type foundPos = this->string::Base::find(s, (&*pos - &*this->cbegin()), count);
	return string::const_iterator(&*this->cbegin() + foundPos);
}
constexpr string::const_iterator string::find(const char* s, string::const_iterator pos) const {
	string::size_type foundPos = this->string::Base::find(s, (&*pos - &*this->cbegin()));
	return string::const_iterator(&*this->cbegin() + foundPos);
}
constexpr string::const_iterator string::find(const char* s) const {return this->find(s, this->cbegin());}
constexpr string::const_iterator string::find(char ch, string::const_iterator pos) const noexcept{
	string::size_type foundPos = this->string::Base::find(ch, (&*pos - &*this->cbegin()));
	return string::const_iterator(&*this->cbegin() + foundPos);
}
constexpr string::const_iterator string::find(char ch) const noexcept {return this->find(ch, this->begin());}
template<class StringViewLike>
constexpr string::const_iterator string::find(const StringViewLike& t, string::const_iterator pos) const noexcept {
	string::size_type foundPos = this->string::Base::find(t, (&*pos - &*this->cbegin()));
	return string::const_iterator(&*this->cbegin() + foundPos);
}
template<class StringViewLike>
constexpr string::const_iterator string::find(const StringViewLike& t) const noexcept{return this->find(t, this->begin());}

constexpr string::const_iterator string::rfind(const string& str, string::const_iterator pos) const noexcept {
	string::size_type foundPos = this->string::Base::rfind(str, (&*pos - &*this->cbegin()));
	return string::const_iterator(&*this->cbegin() + foundPos);
}
constexpr string::const_iterator string::rfind(const string& str) const noexcept{return this->rfind(str, this->begin());}
constexpr string::const_iterator string::rfind(const char* s, string::const_iterator pos, string::size_type count) const {
	string::size_type foundPos = this->string::Base::rfind(s, (&*pos - &*this->cbegin()), count);
	return string::const_iterator(&*this->cbegin() + foundPos);
}
constexpr string::const_iterator string::rfind(const char* s, string::const_iterator pos) const {
	string::size_type foundPos = this->string::Base::rfind(s, (&*pos - &*this->cbegin()));
	return string::const_iterator(&*this->cbegin() + foundPos);
}
constexpr string::const_iterator string::rfind(const char* s) const{return this->rfind(s, this->begin());}
constexpr string::const_iterator string::rfind(char ch, string::const_iterator pos) const noexcept{
	string::size_type foundPos = this->string::Base::rfind(ch, (&*pos - &*this->cbegin()));
	return string::const_iterator(&*this->cbegin() + foundPos);
}
constexpr string::const_iterator string::rfind(char ch) const noexcept{return this->rfind(ch, this->begin());}
template<class StringViewLike>
constexpr string::const_iterator string::rfind(const StringViewLike& t, string::const_iterator pos) const noexcept {
	string::size_type foundPos = this->string::Base::rfind(t, (&*pos - &*this->cbegin()));
	return string::const_iterator(&*this->cbegin() + foundPos);
}
template<class StringViewLike>
constexpr string::const_iterator string::rfind(const StringViewLike& t) const noexcept{return this->rfind(t, this->begin());}


// helper
constexpr string::iterator string::unconst(string::const_iterator itr) {return string::iterator(this, &*this->begin() + (&*itr - &*this->begin()));}

} // namespace utf8
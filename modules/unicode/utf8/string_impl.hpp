#pragma once

#include "string_type.hpp"

#include "char.hpp"
#include "char_reference.hpp"
#include "string_iterator.hpp"
#include "string_view.hpp"

namespace utf8{

// constructors
constexpr string::string() : string::Base(){}
constexpr string::string(string::size_type count, char ch) : string::Base(count, ch){}
constexpr string::string(const string& other, string::size_type pos) : string::Base(other, pos){}
constexpr string::string(const string& other, string::size_type pos, string::size_type count) : string::Base(other, pos, count){}
constexpr string::string(const char* s, string::size_type count) : string::Base(s, count){}
constexpr string::string( const char* s) : string::Base(s){}
template<class InputIt> 
constexpr string::string(InputIt first, InputIt last) : string::Base(first, last) {};
constexpr string::string(const string& other) = default;
constexpr string::string(string&& other) = default;
constexpr string::string(std::initializer_list<char> ilist) : string::Base(ilist);
template<class StringViewLike>
constexpr explicit string::string(const StringViewLike& t) : string::Base(t){}
template<class StringViewLike>
constexpr string::string(const StringViewLike& t, string::size_type pos, string::size_type n) : string::Base(t, pos, n){}

// assignments
constexpr string& string::operator=(const string& str) = default;
constexpr string& string::operator=(string&& str) = default;
constexpr string& string::operator=(const char* s) {this->string::Base::operator=(s); return *this;}
constexpr string& string::operator=(char ch) {this->string::Base::operator=(ch); return *this;}
constexpr string& string::operator=(std::initializer_list<char> ilist) {this->string::Base::operator=(ilist); return *this;}
template<class StringViewLike> 
constexpr string& string::operator=(const StringViewLike& t) {this->string::Base::operator=(t); return *this;}

constexpr string& string::assign(string::size_type count, char ch) {this->string::Base::assign(count, ch); return *this;}
constexpr string& string::assign(const string& str) {this->string::Base::assign(str); return *this;}
constexpr string& string::assign(const string& str, string::size_type pos) {this->string::Base::assign(str, pos); return *this;}
constexpr string& string::assign(const string& str, string::size_type pos, string::size_type count) {this->string::Base::assign(str, pos, count); return *this;}
constexpr string& string::assign(string&& str) noexcept {this->string::Base::assign(std::move(str)); return *this;}
constexpr string& string::assign(const char* s, string::size_type count) {this->string::Base::assign(s, count); return *this;}
constexpr string& string::assign(const char* s) {this->string::Base::assign(s); return *this;}
template<class InputIt> 
constexpr string& string::assign(InputIt first, InputIt last) {this->string::Base::assign(first, last); return *this;}
constexpr string& string::assign(std::initializer_list<char> ilist) {this->string::Base::assign(ilist); return *this;}
template<class StringViewLike> 
constexpr string& string::assign(const StringViewLike& t) {this->string::Base::assign(t); return *this;}
template<class StringViewLike>
constexpr string& string::assign(const StringViewLike& t, string::size_type pos) {this->string::Base::assign(t, pos); return *this;}
constexpr string& string::assign(const StringViewLike& t, string::size_type pos, string::size_type count)  {this->string::Base::assign(t, pos, count); return *this;}

// allocator
constexpr string::allocator_type string::get_allocator() const noexcept {return this->string::Base::get_allocator();}

// Element Access
constexpr string::reference string::front() {return *this->begin();}
constexpr string::reference string::front() const {return *this->cbegin();}

constexpr string::reference string::back() {return *this->rbegin();}
constexpr string::const_reference string::back() {return *this->crbegin();}

constexpr char* string::data() noexcept {return this->string::Base::data();}
constexpr const char* string::data() const noexcept {return this->string::Base::data();}

constexpr char* string::c_str() noexcept {return this->string::Base::c_str();}
constexpr const char* string::c_str() const noexcept {return this->string::Base::c_str();}

constexpr string::operator utf8::string_view() const noexcept {return utf8::string_view(this->data(), this->byte_size(), this->size())}
constexpr string::operator std::string_view() const noexcept {return std::string_view(this->data(), this->byte_size())}

constexpr string::iterator string::begin() noexcept {return string::iterator(&*this->string::Base::begin());}
constexpr string::const_iterator string::begin() const noexcept {return string::iterator(&*this->string::Base::cbegin());}
constexpr string::const_iterator string::cbegin() const noexcept {return string::iterator(&*this->string::Base::cbegin());}

constexpr string::iterator string::end() noexcept {return string::iterator(&*this->string::Base::end());}
constexpr string::const_iterator string::end() const noexcept {return string::const_iterator(&*this->string::Base::end());}
constexpr string::const_iterator string::cend() const noexcept {return string::const_iterator(&*this->string::Base::end());}

constexpr string::reverse_iterator string::rbegin() noexcept {return string::iterator(&*this->end());}
constexpr string::const_reverse_iterator string::rbegin() const noexcept {return string::iterator(&*this->cend());}
constexpr string::const_reverse_iterator string::crbegin() const noexcept {return string::iterator(&*this->cend());}

constexpr string::iterator string::rend() noexcept {return string::iterator(&*this->begin());}
constexpr string::const_reverse_iterator string::rend() const noexcept {return string::iterator(&*this->begin());}
constexpr string::const_reverse_iterator string::crend() const noexcept {return string::iterator(&*this->begin());}

// Capacity
constexpr bool string::empty() const noexcept {return this->string::Base::empty();}
constexpr string::size_type string::byte_size() const noexcept {return this->string::Base::size();}
constexpr string::size_type string::size() const noexcept {return std::distance(this->begin(), this->end());}
constexpr string::size_type string::length() const noexcept {return std::distance(this->begin(), this->end());}
constexpr string::size_type string::max_size() const noexcept {return this->string::Base::max_size();}
constexpr void string::reserve(string::size_type new_cap) {this->string::Base::reserve(new_cap);}
constexpr string::size_type string::capacity() const noexcept {this->string::Base::capacity();}
constexpr void string::shrink_to_fit() const {this->string::Base::shrink_to_fit();}

// Operations
constexpr void string::clear() noexcept {this->string::Base::clear();}
constexpr string::iterator string::insert(string::const_iterator pos, char ascii){return string::iterator(&*this->string::Base::insert(&*pos, ascii));}
constexpr string::iterator string::insert(string::const_iterator pos, utf8::Char ch){return string::iterator(&*this->string::Base::insert(&*pos, ch.begin(), ch.last()));}
constexpr string::iterator string::insert(string::const_iterator pos, string::size_type count, char ascii){return string::iterator(&*this->string::Base::insert(&*pos, count, ch));}
constexpr string::iterator string::insert(string::const_iterator pos, string::size_type count, utf8::Char ch){
	string::iterator result(&*this->string::Base::insert(&*pos, count * ch.size(), '\0'));
	for(string::size_type i = 0; i != count; (void)++i, (void)++pos){
		*pos = ch;
	}
}
constexpr string::iterator string::insert(string::const_iterator pos, const char* str){return string::iterator(&*this->string::Base::insert(std::distance(&*this->string::Base::begin(), &*pos), str));}
constexpr string::iterator string::insert(string::const_iterator pos, const char* str, string::size_type count){return string::iterator(&*this->string::Base::insert(std::distance(&*this->string::Base::begin(), &*pos), str, count));}
template< class InputIt >
constexpr string::iterator string::insert(string::const_iterator pos, InputIt first, InputIt last){return string::iterator(&*this->string::Base::insert(&*pos, first, last));}
constexpr string::iterator string::insert(string::const_iterator pos, std::initializer_list<char> ilist){string::iterator(&*this->string::Base::insert(&*pos, first, last));}
constexpr string& string::erase(string::size_type index = 0){this->erase(this->begin() + index); return *this;}
constexpr string& string::erase(string::size_type index = 0, string::size_type count){auto first = this->begin() + index;auto last = first + count;this->erase(first, last);return *this;}
constexpr string::iterator string::erase(string::const_iterator position){return string::iterator(&*this->erase(&*position));}
constexpr string::iterator string::erase(string::const_iterator first, string::const_iterator last){return string::iterator(&*this->erase(&*first, &*last));}
constexpr void string::push_back(char ascii){this->string::Base::push_back(ascii);}
constexpr void string::push_back(utf8::Char ch){this->string::Base::append(ch.begin(), ch.end());}

constexpr void string::pop_back(){auto first = this->end() - 1;auto last = this->end();this->string::Base::erase(&*first, &*last);}
				
constexpr string& string::append(string::size_type count, char ascii){this->string::Base::append(count, ascii); return *this;}
constexpr string& string::append(const string& str){this->string::Base::append(&*str.begin(), &*str.end()); return *this;}
constexpr string& string::append(const string::Base& str){this->string::Base::append(str); return *this;}
constexpr string& string::append(const string::Base& str, string::size_type pos, string::size_type count = this->string::Base::npos){this->string::Base::append(str, pos, count); return *this;}
constexpr string& string::append(const char* str, string::size_type count){this->string::Base::append(str, count); return *this;}
constexpr string& string::append(const char* str){this->string::Base::append(str); return *this;}
template< class InputIt >
constexpr string& string::append(InputIt first, InputIt last){this->string::Base::append(first, last); return *this;}
constexpr string& string::append(std::initializer_list<CharT> ilist){this->string::Base::append(ilist); return *this;}
template< class StringViewLike >
constexpr string& string::append(const StringViewLike& t){this->string::Base::append(t); return *this;}
template< class StringViewLike >
constexpr string& string::append(const StringViewLike& t, string::size_type pos, string::size_type count = this->string::Base::npos){
	this->string::Base::append(t, pos, count);
}

constexpr string& string::operator+=(const string& str ){return this->append(str);}
constexpr string& string::operator+=(const string::Base& str ){return this->append(str);}
constexpr string& string::operator+=(char ascii){return this->append(ascii);}
constexpr string& string::operator+=(utf8::Char ch){return this->append(ch);}
constexpr string& string::operator+=(const char* s){return this->append(s);}
constexpr string& string::operator+=(std::initializer_list<CharT> ilist){return this->append(ilist);}
template< class StringViewLike >
constexpr basic_string& string::operator+=(const StringViewLike& t){return this->append(t);}

constexpr int string::compare(const string& str) const noexcept {
	if(this->byte_size() == str.byte_size()){
		return std::strncmp(this->data(), str.data(), this->byte_size());
	}
	return false;
}
constexpr int string::compare(const basic_string& str) const noexcept {return this->string::Base::compare(str);}
constexpr int string::compare(const CharT* s) const {return this->string::Base::compare(s);}
template< class StringViewLike >
constexpr int string::compare(const StringViewLike& t) const {return this->string::Base::compare(t);}

constexpr bool string::starts_with(std::string_view sv) const noexcept {return this->string::Base::starts_with(sv);}
constexpr bool string::starts_with(utf8::string_view sv) const noexcept {return this->string::Base::starts_with(static_cast<std::string_view>(sv));}
constexpr bool string::starts_with(char ch) const noexcept {return this->string::Base::starts_with(ch);}
constexpr bool string::starts_with(utf8::Char ch) const noexcept {return this->starts_with(ch.to_std_string_view());}
constexpr bool string::starts_with(const char* str) const {return this->string::Base::starts_with(str);}

constexpr bool string::ends_with(std::string_view sv) const noexcept {return this->string::Base::ends_with(sv);}
constexpr bool string::ends_with(utf8::string_view sv) const noexcept {return this->string::Base::ends_with(static_cast<std::string_view>(sv));}
constexpr bool string::ends_with(char ch) const noexcept {return this->string::Base::ends_with(ch);}
constexpr bool string::ends_with(utf8::Char ch) const noexcept {return this->ends_with(ch.to_std_string_view());}
constexpr bool string::ends_with(const char* str) const {return this->string::Base::ends_with(str);}

constexpr bool string::contains(std::string_view sv) const noexcept {return this->string::Base::contains(sv);}
constexpr bool string::contains(utf8::string_view sv) const noexcept {return this->contains(static_cast<std::string_view>(sv));}
constexpr bool string::contains(char ch) const noexcept {return this->string::Base::contains(ch);}
constexpr bool string::contains(utf8::Char ch) const noexcept {return this->contains(ch.to_std_string_view());}
constexpr bool string::contains(const char* str) const {return this->string::Base::contains(str);}

constexpr string& string::replace(string::const_iterator first, string::const_iterator last, const string& str) {return this->replace(first, last, str.cbegin(), str.cend());}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, const char* cstr, string::size_type count2) {
	return this->replace(first, last, string::const_iterator(cstr), string::const_iterator(cstr + count2));
}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, const char* cstr){
	auto thisItr = this->unconst(destFirst);
	auto strItr = string::const_iterator(cstr);
	for(; thisItr != destLast && *strItr != '\0'; (void)++thisItr, (void)++strItr){
		*thisItr = *strItr;
	}
	return *this;
}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, string::size_type count2, char ch) {
	auto thisItr = this->unconst(destFirst);
	string::size_type i = 0;
	for(; thisItr != last && i != count2; (void)++thisItr, (void)++i){
		*thisItr == ch;
	}
	return *this;
}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, string::size_type count2, utf8::Char ch) {
	auto thisItr = this->unconst(destFirst);
	string::size_type i = 0;
	for(; thisItr != last && i != count2; (void)++thisItr, (void)++i){
		*thisItr == ch;
	}
	return *this;
}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, char ch) {
	auto thisItr = this->unconst(destFirst);
	for(; thisItr != last; (void)++thisItr){
		*thisItr == ch;
	}
	return *this;
}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, utf8::Char ch) {
	auto thisItr = this->unconst(destFirst);
	for(; thisItr != last; (void)++thisItr){
		*thisItr == ch;
	}
	return *this;
}
template< class InputIt >
constexpr string& string::replace(string::const_iterator destFirst, string::const_iterator destLast, InputIt sourceFirst, InputIt sourceLast){
	for(auto itr = this->unconst(destFirst); itr != destLast && sourceFirst != sourceLast; (void)++itr, (void)++sourceFirst){
		*itr = *sourceFirst;
	}
	return *this;
}
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, std::initializer_list<CharT> ilist) {return this->replace(first, last, ilist.begin(), ilist.end());}
template< class StringViewLike >
constexpr string& string::replace(string::const_iterator first, string::const_iterator last, const StringViewLike& t) {return this->replace(first, last, t.begin(), t.end());}

constexpr void string::resize(string::size_type count){this->string::Base::resize(count);}
constexpr void string::resize(string::size_type count, char ch){this->string::Base::resize(count, ch);}

constexpr void swap( string& other ) {this->string::Base::swap(other);}

// search

constexpr string::const_iterator string::find(const string& str, string::const_iterator pos = this->cbegin()) const noexcept {
	string::size_type foundPos = this->string::Base::find(str, std::distance(&*this->cbegin(), &*pos));
	return string::const_iterator(&*this->cbegin() + foundPos);
}
constexpr string::const_iterator string::find(const CharT* s, string::const_iterator pos, string::size_type count) const {
	string::size_type foundPos = this->string::Base::find(s, std::distace(&*this->cbegin(), &*pos), count);
	return string::const_iterator(&*this->cbegin() + foundPos);
}
constexpr string::const_iterator string::find(const CharT* s, string::const_iterator pos =  = this->cbegin()) const {
	string::size_type foundPos = this->string::Base::find(s, std::distace(&*this->cbegin(), &*pos));
	return string::const_iterator(&*this->cbegin() + foundPos);
}
constexpr string::const_iterator string::find(char ch, string::const_iterator pos = this->cbegin()) const noexcept{
	string::size_type foundPos = this->string::Base::find(ch, std::distace(&*this->cbegin(), &*pos));
	return string::const_iterator(&*this->cbegin() + foundPos);
}
template<class StringViewLike>
constexpr string::const_iterator string::find(const StringViewLike& t, string::const_iterator pos = this->cbegin()) const noexcept {
	string::size_type foundPos = this->string::Base::find(t, std::distace(&*this->cbegin(), &*pos));
	return string::const_iterator(&*this->cbegin() + foundPos);
}

constexpr string::const_iterator string::rfind(const string& str, string::const_iterator pos = this->cend()) const noexcept {
	string::size_type foundPos = this->string::Base::rfind(str, std::distance(&*this->cend(), &*pos));
	return string::const_iterator(&*this->cbegin() + foundPos);
}
constexpr string::const_iterator string::rfind(const CharT* s, string::const_iterator pos, string::size_type count) const {
	string::size_type foundPos = this->string::Base::rfind(s, std::distace(&*this->cend(), &*pos), count);
	return string::const_iterator(&*this->cbegin() + foundPos);
}
constexpr string::const_iterator string::rfind(const CharT* s, string::const_iterator pos =  = this->cend()) const {
	string::size_type foundPos = this->string::Base::rfind(s, std::distace(&*this->cend(), &*pos));
	return string::const_iterator(&*this->cbegin() + foundPos);
}
constexpr string::const_iterator string::rfind(char ch, string::const_iterator pos = this->cend()) const noexcept{
	string::size_type foundPos = this->string::Base::rfind(ch, std::distace(&*this->cend(), &*pos));
	return string::const_iterator(&*this->cbegin() + foundPos);
}
template<class StringViewLike>
constexpr string::const_iterator string::rfind(const StringViewLike& t, string::const_iterator pos = this->cend()) const noexcept {
	string::size_type foundPos = this->string::Base::rfind(t, std::distace(&*this->cend(), &*pos));
	return string::const_iterator(&*this->cbegin() + foundPos);
}


// helper
constexpr string::iterator string::unconst(string::const_iterator itr) {return string::iterator(this->begin() + std::distance(this->begin(), itr));}

} // namespace utf8
#pragma once

#include <string>
#include <exceptions>

#include "char.hpp"
#include "char_reference.hpp"
#include "string_iterator.hpp"
#include "string_view.hpp"

namespace utf8{

class string : private std::string<char> {
public:
	using Base = std::string<char>;

	using traits_type = std::string::traits_type;
	using value_type = std::string::value_type;
	using allocator_type = std::string::allocator_type;
	using size_type = std::string::size_type;
	using difference_type = std::string::difference_type;
	using const_reference = std::string::const_reference;
	using pointer = std::string::pointer;
	using const_pointer = std::string::const_pointer;
	using iterator = utf8::iterator;
	using const_iterator = utf8::const_iterator;
	
public:
	// constructors
	constexpr string() : Base(){}
	constexpr string(size_type count, char ch) : Base(count, ch){}
	constexpr string(const string& other, size_type pos) : Base(other, pos){}
	constexpr string(const string& other, size_type pos, size_type count) : Base(other, pos, count){}
	constexpr string(const char* s, size_type count) : Base(s, count){}
	constexpr string( const char* s) : Base(s){}
	template<class InputIt> 
	constexpr string(InputIt first, InputIt last) : Base(first, last) {};
	constexpr string(const string& other) = default;
	constexpr string(string&& other) = default;
	constexpr string(std::initializer_list<char> ilist) : Base(ilist);
	template<class StringViewLike>
	constexpr explicit string(const StringViewLike& t) : Base(t){}
	template<class StringViewLike>
	constexpr string(const StringViewLike& t, size_type pos, size_type n) : Base(t, pos, n){}
	
	// assignments
	constexpr string& operator=(const string& str) = default;
	constexpr string& operator=(string&& str) = default;
	constexpr string& operator=(const char* s) {this->Base::operator=(s); return *this;}
	constexpr string& operator=(char ch) {this->Base::operator=(ch); return *this;}
	constexpr string& operator=(std::initializer_list<char> ilist) {this->Base::operator=(ilist); return *this;}
	template<class StringViewLike> 
	constexpr string& operator=(const StringViewLike& t) {this->Base::operator=(t); return *this;}
	
	constexpr string& assign(size_type count, char ch) {this->Base::assign(count, ch); return *this;}
	constexpr string& assign(const string& str) {this->Base::assign(str); return *this;}
	constexpr string& assign(const string& str, size_type pos) {this->Base::assign(str, pos); return *this;}
	constexpr string& assign(const string& str, size_type pos, size_type count) {this->Base::assign(str, pos, count); return *this;}
	constexpr string& assign(string&& str) noexcept {this->Base::assign(std::move(str)); return *this;}
	constexpr string& assign(const char* s, size_type count) {this->Base::assign(s, count); return *this;}
	constexpr string& assign(const char* s) {this->Base::assign(s); return *this;}
	template<class InputIt> 
	constexpr string& assign(InputIt first, InputIt last) {this->Base::assign(first, last); return *this;}
	constexpr string& assign(std::initializer_list<char> ilist) {this->Base::assign(ilist); return *this;}
	template<class StringViewLike> 
	constexpr string& assign(const StringViewLike& t) {this->Base::assign(t); return *this;}
	template<class StringViewLike>
	constexpr string& assign(const StringViewLike& t, size_type pos) {this->Base::assign(t, pos); return *this;}
	constexpr string& assign(const StringViewLike& t, size_type pos, size_type count)  {this->Base::assign(t, pos, count); return *this;}
	
	// allocator
	constexpr allocator_type get_allocator() const noexcept {return this->Base::get_allocator();}
	
	// Element Access
	constexpr reference front() {return *this->begin();}
	constexpr reference front() const {return *this->cbegin();}
	
	constexpr reference back() {return *this->rbegin();}
	constexpr const_reference back() {return *this->crbegin();}
	
	constexpr char* data() noexcept {return this->Base::data();}
	constexpr const char* data() const noexcept {return this->Base::data();}
	
	constexpr char* c_str() noexcept {return this->Base::c_str();}
	constexpr const char* c_str() const noexcept {return this->Base::c_str();}
	
	constexpr operator utf8::string_view() const noexcept {return utf8::string_view(this->data(), this->byte_size(), this->size())}
	constexpr operator std::string_view() const noexcept {return std::string_view(this->data(), this->byte_size())}
	
	constexpr iterator begin() noexcept {return iterator(&*this->Base::begin());}
	constexpr const_iterator begin() const noexcept {return iterator(&*this->Base::cbegin());}
	constexpr const_iterator cbegin() const noexcept {return iterator(&*this->Base::cbegin());}
	
	constexpr iterator end() noexcept {return iterator(&*this->Base::end());}
	constexpr const_iterator end() const noexcept {return const_iterator(&*this->Base::end());}
	constexpr const_iterator cend() const noexcept {return const_iterator(&*this->Base::end());}
	
	constexpr reverse_iterator rbegin() noexcept {return iterator(&*this->end());}
	constexpr const_reverse_iterator rbegin() const noexcept {return iterator(&*this->cend());}
	constexpr const_reverse_iterator crbegin() const noexcept {return iterator(&*this->cend());}
	
	constexpr iterator rend() noexcept {return iterator(&*this->begin());}
	constexpr const_reverse_iterator rend() const noexcept {return iterator(&*this->begin());}
	constexpr const_reverse_iterator crend() const noexcept {return iterator(&*this->begin());}
	
	// Capacity
	constexpr bool empty() const noexcept {return this->Base::empty();}
	constexpr size_type byte_size() const noexcept {return this->Base::size();}
	constexpr size_type size() const noexcept {return std::distance(this->begin(), this->end());}
	constexpr size_type length() const noexcept {return std::distance(this->begin(), this->end());}
	constexpr size_type max_size() const noexcept {return this->Base::max_size();}
	constexpr void reserve(size_type new_cap) {this->Base::reserve(new_cap);}
	constexpr size_type capacity() const noexcept {this->Base::capacity();}
	constexpr void shrink_to_fit() const {this->Base::shrink_to_fit();}
	
	// Operations
	constexpr void clear() noexcept {this->Base::clear();}
	constexpr iterator insert(const_iterator pos, char ascii){return iterator(&*this->Base::insert(&*pos, ascii));}
	constexpr iterator insert(const_iterator pos, utf8::Char ch){return iterator(&*this->Base::insert(&*pos, ch.begin(), ch.last()));}
	constexpr iterator insert(const_iterator pos, size_type count, char ascii){return iterator(&*this->Base::insert(&*pos, count, ch));}
	constexpr iterator insert(const_iterator pos, size_type count, utf8::Char ch){
		iterator result(&*this->Base::insert(&*pos, count * ch.size(), '\0'));
		for(size_type i = 0; i != count; (void)++i, (void)++pos){
			*pos = ch;
		}
	}
	constexpr iterator insert(const_iterator pos, const char* str){return iterator(&*this->Base::insert(std::distance(&*this->Base::begin(), &*pos), str));}
	constexpr iterator insert(const_iterator pos, const char* str, size_type count){return iterator(&*this->Base::insert(std::distance(&*this->Base::begin(), &*pos), str, count));}
	template< class InputIt >
	constexpr iterator insert(const_iterator pos, InputIt first, InputIt last){return iterator(&*this->Base::insert(&*pos, first, last));}
	constexpr iterator insert(const_iterator pos, std::initializer_list<char> ilist){iterator(&*this->Base::insert(&*pos, first, last));}
	constexpr string& erase(size_type index = 0){this->erase(this->begin() + index); return *this;}
	constexpr string& erase(size_type index = 0, size_type count);{auto first = this->begin() + index;auto last = first + count;this->erase(first, last);return *this;}
	constexpr iterator erase(const_iterator position){return iterator(&*this->erase(&*position));}
	constexpr iterator erase(const_iterator first, const_iterator last){return iterator(&*this->erase(&*first, &*last));}
	constexpr void push_back(char ascii){this->Base::push_back(ascii);}
	constexpr void push_back(utf8::Char ch){this->Base::append(ch.begin(), ch.end());}
	
	constexpr void pop_back(){auto first = this->end() - 1;auto last = this->end();this->Base::erase(&*first, &*last);}
					
	constexpr string& append(size_type count, char ascii){this->Base::append(count, ascii); return *this;}
	constexpr string& append(const string& str){this->Base::append(&*str.begin(), &*str.end()); return *this;}
	constexpr string& append(const Base& str){this->Base::append(str); return *this;}
	constexpr string& append(const Base& str, size_type pos, size_type count = this->Base::npos){this->Base::append(str, pos, count); return *this;}
	constexpr string& append(const char* str, size_type count){this->Base::append(str, count); return *this;}
	constexpr string& append(const char* str){this->Base::append(str); return *this;}
	template< class InputIt >
	constexpr string& append(InputIt first, InputIt last){this->Base::append(first, last); return *this;}
	constexpr string& append(std::initializer_list<CharT> ilist){this->Base::append(ilist); return *this;}
	template< class StringViewLike >
	constexpr string& append(const StringViewLike& t){this->Base::append(t); return *this;}
	template< class StringViewLike >
	constexpr string& append(const StringViewLike& t, size_type pos, size_type count = this->Base::npos){
		this->Base::append(t, pos, count);
	}
	
	constexpr string& operator+=(const string& str ){return this->append(str);}
	constexpr string& operator+=(const Base& str ){return this->append(str);}
	constexpr string& operator+=(char ascii){return this->append(ascii);}
	constexpr string& operator+=(utf8::Char ch){return this->append(ch);}
	constexpr string& operator+=(const char* s){return this->append(s);}
	constexpr string& operator+=(std::initializer_list<CharT> ilist){return this->append(ilist);}
	template< class StringViewLike >
	constexpr basic_string& operator+=(const StringViewLike& t){return this->append(t);}
	
	constexpr int compare(const string& str) const noexcept {
		if(this->byte_size() == str.byte_size()){
			return std::strncmp(this->data(), str.data(), this->byte_size());
		}
		return false;
	}
	constexpr int compare(const basic_string& str) const noexcept {return this->Base::compare(str);}
	constexpr int compare(const CharT* s) const {return this->Base::compare(s);}
	template< class StringViewLike >
	constexpr int compare(const StringViewLike& t) const {return this->Base::compare(t);}
	
	constexpr bool starts_with(std::string_view sv) const noexcept {return this->Base::starts_with(sv);}
	constexpr bool starts_with(utf8::string_view sv) const noexcept {return this->Base::starts_with(static_cast<std::string_view>(sv));}
	constexpr bool starts_with(char ch) const noexcept {return this->Base::starts_with(ch);}
	constexpr bool starts_with(utf8::Char ch) const noexcept {return this->starts_with(ch.to_std_string_view());}
	constexpr bool starts_with(const char* str) const {return this->Base::starts_with(str);}
	
	constexpr bool ends_with(std::string_view sv) const noexcept {return this->Base::ends_with(sv);}
	constexpr bool ends_with(utf8::string_view sv) const noexcept {return this->Base::ends_with(static_cast<std::string_view>(sv));}
	constexpr bool ends_with(char ch) const noexcept {return this->Base::ends_with(ch);}
	constexpr bool ends_with(utf8::Char ch) const noexcept {return this->ends_with(ch.to_std_string_view());}
	constexpr bool ends_with(const char* str) const {return this->Base::ends_with(str);}
	
	constexpr bool contains(std::string_view sv) const noexcept {return this->Base::contains(sv);}
	constexpr bool contains(utf8::string_view sv) const noexcept {return this->contains(static_cast<std::string_view>(sv));}
	constexpr bool contains(char ch) const noexcept {return this->Base::contains(ch);}
	constexpr bool contains(utf8::Char ch) const noexcept {return this->contains(ch.to_std_string_view());}
	constexpr bool contains(const char* str) const {return this->Base::contains(str);}
	
	constexpr string& replace(const_iterator first, const_iterator last, const string& str) {return this->replace(first, last, str.cbegin(), str.cend());}
	constexpr string& replace(const_iterator first, const_iterator last, const char* cstr, size_type count2) {
		return this->replace(first, last, const_iterator(cstr), const_iterator(cstr + count2));
	}
	constexpr string& replace(const_iterator first, const_iterator last, const char* cstr)
		auto thisItr = this->unconst(destFirst);
		auto strItr = const_iterator(cstr);
		for(; thisItr != destLast && *strItr != '\0'; (void)++thisItr, (void)++strItr){
			*thisItr = *strItr;
		}
		return *this;
	}
	constexpr string& replace(const_iterator first, const_iterator last, size_type count2, char ch) {
		auto thisItr = this->unconst(destFirst);
		size_type i = 0;
		for(; thisItr != last && i != count2; (void)++thisItr, (void)++i){
			*thisItr == ch;
		}
		return *this;
	}
	constexpr string& replace(const_iterator first, const_iterator last, size_type count2, utf8::Char ch) {
		auto thisItr = this->unconst(destFirst);
		size_type i = 0;
		for(; thisItr != last && i != count2; (void)++thisItr, (void)++i){
			*thisItr == ch;
		}
		return *this;
	}
	constexpr string& replace(const_iterator first, const_iterator last, char ch) {
		auto thisItr = this->unconst(destFirst);
		for(; thisItr != last; (void)++thisItr){
			*thisItr == ch;
		}
		return *this;
	}
	constexpr string& replace(const_iterator first, const_iterator last, utf8::Char ch) {
		auto thisItr = this->unconst(destFirst);
		for(; thisItr != last; (void)++thisItr){
			*thisItr == ch;
		}
		return *this;
	}
	template< class InputIt >
	constexpr string& replace(const_iterator destFirst, const_iterator destLast, InputIt sourceFirst, InputIt sourceLast){
		for(auto itr = this->unconst(destFirst); itr != destLast && sourceFirst != sourceLast; (void)++itr, (void)++sourceFirst){
			*itr = *sourceFirst;
		}
		return *this;
	}
	constexpr string& replace(const_iterator first, const_iterator last, std::initializer_list<CharT> ilist) {return this->replace(first, last, ilist.begin(), ilist.end());}
	template< class StringViewLike >
	constexpr string& replace(const_iterator first, const_iterator last, const StringViewLike& t) {return this->replace(first, last, t.begin(), t.end());}
	
	constexpr void resize(size_type count){this->Base::resize(count);}
	constexpr void resize(size_type count, char ch){this->Base::resize(count, ch);}
	
	constexpr void swap( string& other ) {this->Base::swap(other);}
	
	// search
	
	constexpr const_iterator find(const string& str, const_iterator pos = this->cbegin()) const noexcept {
		size_type foundPos = this->Base::find(str, std::distance(&*this->cbegin(), &*pos));
		return const_iterator(&*this->cbegin() + foundPos);
	}
	constexpr const_iterator find(const CharT* s, const_iterator pos, size_type count) const {
		size_type foundPos = this->Base::find(s, std::distace(&*this->cbegin(), &*pos), count);
		return const_iterator(&*this->cbegin() + foundPos);
	}
	constexpr const_iterator find(const CharT* s, const_iterator pos =  = this->cbegin()) const {
		size_type foundPos = this->Base::find(s, std::distace(&*this->cbegin(), &*pos));
		return const_iterator(&*this->cbegin() + foundPos);
	}
	constexpr const_iterator find(char ch, const_iterator pos = this->cbegin()) const noexcept{
		size_type foundPos = this->Base::find(ch, std::distace(&*this->cbegin(), &*pos));
		return const_iterator(&*this->cbegin() + foundPos);
	}
	template<class StringViewLike>
	constexpr const_iterator find(const StringViewLike& t, const_iterator pos = this->cbegin()) const noexcept {
		size_type foundPos = this->Base::find(t, std::distace(&*this->cbegin(), &*pos));
		return const_iterator(&*this->cbegin() + foundPos);
	}
	
	constexpr const_iterator rfind(const string& str, const_iterator pos = this->cend()) const noexcept {
		size_type foundPos = this->Base::rfind(str, std::distance(&*this->cend(), &*pos));
		return const_iterator(&*this->cbegin() + foundPos);
	}
	constexpr const_iterator rfind(const CharT* s, const_iterator pos, size_type count) const {
		size_type foundPos = this->Base::rfind(s, std::distace(&*this->cend(), &*pos), count);
		return const_iterator(&*this->cbegin() + foundPos);
	}
	constexpr const_iterator rfind(const CharT* s, const_iterator pos =  = this->cend()) const {
		size_type foundPos = this->Base::rfind(s, std::distace(&*this->cend(), &*pos));
		return const_iterator(&*this->cbegin() + foundPos);
	}
	constexpr const_iterator rfind(char ch, const_iterator pos = this->cend()) const noexcept{
		size_type foundPos = this->Base::rfind(ch, std::distace(&*this->cend(), &*pos));
		return const_iterator(&*this->cbegin() + foundPos);
	}
	template<class StringViewLike>
	constexpr const_iterator rfind(const StringViewLike& t, const_iterator pos = this->cend()) const noexcept {
		size_type foundPos = this->Base::rfind(t, std::distace(&*this->cend(), &*pos));
		return const_iterator(&*this->cbegin() + foundPos);
	}
	
	
	// helper
	constexpr iterator unconst(const_iterator itr) {return iterator(this->begin() + std::distance(this->begin(), itr));}
								
}
	
} // namespace utf8
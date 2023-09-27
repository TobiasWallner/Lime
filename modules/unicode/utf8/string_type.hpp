#pragma once

#include <string>

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
	constexpr string();
	constexpr string(size_type count, char ch);
	constexpr string(const string& other, size_type pos);
	constexpr string(const string& other, size_type pos, size_type count);
	constexpr string(const char* s, size_type count);
	constexpr string( const char* s);
	template<class InputIt> 
	constexpr string(InputIt first, InputIt last);
	constexpr string(const string& other);
	constexpr string(string&& other);
	constexpr string(std::initializer_list<char> ilist);
	template<class StringViewLike>
	constexpr explicit string(const StringViewLike& t);
	template<class StringViewLike>
	constexpr string(const StringViewLike& t, size_type pos, size_type n);
	
	// assignments
	constexpr string& operator=(const string& str);
	constexpr string& operator=(string&& str);
	constexpr string& operator=(const char* s);
	constexpr string& operator=(char ch);
	constexpr string& operator=(std::initializer_list<char> ilist);
	template<class StringViewLike> 
	constexpr string& operator=(const StringViewLike& t);
	
	constexpr string& assign(size_type count, char ch);
	constexpr string& assign(const string& str);
	constexpr string& assign(const string& str, size_type pos);
	constexpr string& assign(const string& str, size_type pos, size_type count);
	constexpr string& assign(string&& str) noexcept;
	constexpr string& assign(const char* s, size_type count);
	constexpr string& assign(const char* s);
	template<class InputIt> 
	constexpr string& assign(InputIt first, InputIt last);
	constexpr string& assign(std::initializer_list<char> ilist);
	template<class StringViewLike> 
	constexpr string& assign(const StringViewLike& t);
	template<class StringViewLike>
	constexpr string& assign(const StringViewLike& t, size_type pos);
	constexpr string& assign(const StringViewLike& t, size_type pos, size_type count);
	
	// allocator
	constexpr allocator_type get_allocator() const noexcept;
	
	// Element Access
	constexpr reference front();
	constexpr reference front();
	
	constexpr reference back();
	constexpr const_reference back();
	
	constexpr char* data() noexcept;
	constexpr const char* data() const noexcept;
	
	constexpr char* c_str() noexcept;
	constexpr const char* c_str() const noexcept;
	
	constexpr operator utf8::string_view() const noexcept;
	constexpr operator std::string_view() const noexcept;
	
	constexpr iterator begin() noexcept;
	constexpr const_iterator begin() const noexcept;
	constexpr const_iterator cbegin() const noexcept;
	
	constexpr iterator end() noexcept;
	constexpr const_iterator end() const noexcept;
	constexpr const_iterator cend() const noexcept;
	
	constexpr reverse_iterator rbegin() noexcept;
	constexpr const_reverse_iterator rbegin() const noexcept;
	constexpr const_reverse_iterator crbegin() const noexcept;
	
	constexpr iterator rend() noexcept;
	constexpr const_reverse_iterator rend() const noexcept;
	constexpr const_reverse_iterator crend() const noexcept;
	
	// Capacity
	constexpr bool empty() const noexcept;
	constexpr size_type byte_size() const noexcept;
	constexpr size_type size() const noexcept;
	constexpr size_type length() const noexcept;
	constexpr size_type max_size() const noexcept;
	constexpr void reserve(size_type new_cap);
	constexpr size_type capacity() const noexcept;
	constexpr void shrink_to_fit() const;
	
	// Operations
	constexpr void clear() noexcept;
	constexpr iterator insert(const_iterator pos, char ascii);
	constexpr iterator insert(const_iterator pos, utf8::Char ch);
	constexpr iterator insert(const_iterator pos, size_type count, char ascii);
	constexpr iterator insert(const_iterator pos, size_type count, utf8::Char ch);
	constexpr iterator insert(const_iterator pos, const char* str);
	constexpr iterator insert(const_iterator pos, const char* str, size_type count);
	template< class InputIt >
	constexpr iterator insert(const_iterator pos, InputIt first, InputIt last);
	constexpr iterator insert(const_iterator pos, std::initializer_list<char> ilist);
	constexpr string& erase(size_type index = 0);
	constexpr string& erase(size_type index = 0, size_type count);
	constexpr iterator erase(const_iterator position);
	constexpr iterator erase(const_iterator first, const_iterator last);
	constexpr void push_back(char ascii);
	constexpr void push_back(utf8::Char ch);
	
	constexpr void pop_back();
					
	constexpr string& append(size_type count, char ascii);
	constexpr string& append(const string& str);
	constexpr string& append(const Base& str);
	constexpr string& append(const Base& str, size_type pos, size_type count = this->Base::npos);
	constexpr string& append(const char* str, size_type count);
	constexpr string& append(const char* str);
	template< class InputIt >
	constexpr string& append(InputIt first, InputIt last);
	constexpr string& append(std::initializer_list<CharT> ilist);
	template< class StringViewLike >
	constexpr string& append(const StringViewLike& t);
	template< class StringViewLike >
	constexpr string& append(const StringViewLike& t, size_type pos, size_type count = this->Base::npos);
	
	constexpr string& operator+=(const string& str );
	constexpr string& operator+=(const Base& str );
	constexpr string& operator+=(char ascii);
	constexpr string& operator+=(utf8::Char ch);
	constexpr string& operator+=(const char* s);
	constexpr string& operator+=(std::initializer_list<CharT> ilist);
	template< class StringViewLike >
	constexpr basic_string& operator+=(const StringViewLike& t);
	
	constexpr int compare(const string& str) const noexcept;
	constexpr int compare(const basic_string& str) const noexcept;
	constexpr int compare(const CharT* s) const;
	template< class StringViewLike >
	constexpr int compare(const StringViewLike& t) const;
	
	constexpr bool starts_with(std::string_view sv) const noexcept;
	constexpr bool starts_with(utf8::string_view sv) const noexcept;
	constexpr bool starts_with(char ch) const noexcept;
	constexpr bool starts_with(utf8::Char ch) const noexcept;
	constexpr bool starts_with(const char* str) const;
	
	constexpr bool ends_with(std::string_view sv) const noexcept;
	constexpr bool ends_with(utf8::string_view sv) const noexcept;
	constexpr bool ends_with(char ch) const noexcept;
	constexpr bool ends_with(utf8::Char ch) const noexcept;
	constexpr bool ends_with(const char* str) const;
	
	constexpr bool contains(std::string_view sv) const noexcept;
	constexpr bool contains(utf8::string_view sv) const noexcept;
	constexpr bool contains(char ch) const noexcept;
	constexpr bool contains(utf8::Char ch) const noexcept;
	constexpr bool contains(const char* str) const;
	
	constexpr string& replace(const_iterator first, const_iterator last, const string& str);
	constexpr string& replace(const_iterator first, const_iterator last, const char* cstr, size_type count2);
	constexpr string& replace(const_iterator first, const_iterator last, const char* cstr);
	constexpr string& replace(const_iterator first, const_iterator last, size_type count2, char ch);
	constexpr string& replace(const_iterator first, const_iterator last, size_type count2, utf8::Char ch);
	constexpr string& replace(const_iterator first, const_iterator last, char ch);
	constexpr string& replace(const_iterator first, const_iterator last, utf8::Char ch);
	template< class InputIt >
	constexpr string& replace(const_iterator destFirst, const_iterator destLast, InputIt sourceFirst, InputIt sourceLast);
	constexpr string& replace(const_iterator first, const_iterator last, std::initializer_list<CharT> ilist);
	template< class StringViewLike >
	constexpr string& replace(const_iterator first, const_iterator last, const StringViewLike& t);
	
	constexpr void resize(size_type count);
	constexpr void resize(size_type count, char ch);
	
	constexpr void swap(string& other);
	
	// search
	
	constexpr const_iterator find(const string& str, const_iterator pos = this->cbegin()) const noexcept;
	constexpr const_iterator find(const CharT* s, const_iterator pos, size_type count) const;
	constexpr const_iterator find(const CharT* s, const_iterator pos =  = this->cbegin()) const;
	constexpr const_iterator find(char ch, const_iterator pos = this->cbegin()) const noexcept;
	template<class StringViewLike>
	constexpr const_iterator find(const StringViewLike& t, const_iterator pos = this->cbegin()) const noexcept;
	
	constexpr const_iterator rfind(const string& str, const_iterator pos = this->cend()) const noexcept;
	constexpr const_iterator rfind(const CharT* s, const_iterator pos, size_type count) const;
	constexpr const_iterator rfind(const CharT* s, const_iterator pos =  = this->cend()) const;
	constexpr const_iterator rfind(char ch, const_iterator pos = this->cend()) const noexcept;
	template<class StringViewLike>
	constexpr const_iterator rfind(const StringViewLike& t, const_iterator pos = this->cend()) const noexcept;
	
	
	// helper
	constexpr iterator unconst(const_iterator itr);
								
};
	
} // namespace utf8
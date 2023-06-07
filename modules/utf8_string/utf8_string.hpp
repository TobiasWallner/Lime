
#pragma once

// c++ std
#include <string>
#include <ostream>
#include <cstring>
#include <stdexcept>
#include <iostream>

//Project
#include "utf8_char.hpp"




namespace utf8{

using BaseString_view = std::basic_string_view<Char>;

/// BaseString that stores utf8::Char types 
///
/// * is allocator aware
/// * supports all constructors that a std::basic_string supports
template<class Traits = std::char_traits<Char>, class Allocator = std::allocator<Char>>
class BaseString : public std::basic_string<Char, Traits, Allocator>{
	
public:
	using CharT = Char;
	using base_class = std::basic_string<Char, Traits, Allocator>;
	using size_type = typename base_class::size_type;
	using iterator = typename base_class::iterator;
	using const_iterator = typename base_class::const_iterator;
	using reference = typename base_class::reference;
	using const_reference = typename base_class::const_reference;
	using allocator = Allocator;
	
	/// 1) default constuctor
	inline BaseString( ) = default;
	
	/// copy constructable
	inline BaseString(const BaseString&) = default;
	
	/// copy assignable
	inline BaseString& operator = (const BaseString&) = default;
	
	/// move constructable
	inline BaseString(BaseString&&) = default;
	
	/// move assignable
	inline BaseString& operator = (BaseString&&) = default;
	
	/// 2) Constructs the BaseString with count copies of character ch. 
	/// This constructor is not used for class template argument deduction 
	/// if the Allocator type that would be deduced does not qualify as an allocator. (since C++17)
	inline BaseString(size_type count, CharT ch, const Allocator& alloc = Allocator()) : base_class(count, ch, alloc){}
	
	/// 2.1) Constructs the BaseString with count copies of character ch. 
	/// This constructor is not used for class template argument deduction 
	/// if the Allocator type that would be deduced does not qualify as an allocator. (since C++17)
	inline BaseString(size_type count, char ch, const Allocator& alloc = Allocator()) : base_class(count, Char(ch), alloc){}
	
	/// 3) Constructs the BaseString with a substring [pos, pos + count) of other. 
	/// If count == npos, if count is not specified, or if the requested substring lasts past the end of the BaseString, 
	/// the resulting substring is [pos, other.size()). If other is an rvalue reference, it is left in a valid but unspecified state. (since C++23)
	inline BaseString(const BaseString& other, size_type pos, size_type count, const Allocator& alloc = Allocator()) : base_class (other, pos, count, alloc){}
	
	/// 4) Constructs the BaseString with the first count characters of character BaseString pointed to by s. 
	/// s can contain null characters. The length of the BaseString is count. 
	/// The behavior is undefined if [s, s + count) is not a valid range.
	inline BaseString(const CharT* s, size_type count, const Allocator& alloc = Allocator()) : base_class( s, count, alloc){}
	
	/// 4.1) Constructs the BaseString with the first count characters of character BaseString pointed to by s. 
	/// s can contain null characters. The length of the BaseString is count. 
	/// The behavior is undefined if [s, s + count) is not a valid range.
	inline BaseString(const char* s, size_type count, const Allocator& alloc = Allocator()) : BaseString(s, s + count, alloc){}
	
	
	/// 5) Constructs the BaseString with the contents initialized with a copy of the null-terminated character BaseString pointed to by s. 
	/// The length of the BaseString is determined by the first null character. 
	/// The behavior is undefined if [s, s + Traits::length(s)) is not a valid range (for example, if s is a null pointer). 
	/// This constructor is not used for class template argument deduction 
	/// if the Allocator type that would be deduced does not qualify as an allocator. (since C++17)
	explicit inline BaseString(const CharT* s, const Allocator& alloc = Allocator()) : base_class(s, alloc){}
	
	/// 5.1) assigns a null terminated c-string to the BaseString class
	/// **throws** an exception if the provided BaseString is not utf8 compliant
	explicit inline BaseString(const char* c_str, const Allocator& alloc = Allocator()) : BaseString(c_str, c_str + std::strlen(c_str), alloc) {}
	
	/// 6) Constructs the BaseString with the contents of the range [first, last). 
	template<class InputIt>
	inline BaseString(InputIt first, InputIt last, const Allocator& alloc = Allocator()) : BaseString() {this->append(first, last);}
	
	inline BaseString(const_iterator first, const_iterator last, const Allocator& alloc = Allocator()) : base_class(first, last, alloc){}
	inline BaseString(iterator first, iterator last, const Allocator& alloc = Allocator()) : base_class(first, last, alloc){}
	
	/// 6.1) Constructs the BaseString with the contents of the range [first, last) and assigns the c-string to the BaseString. 
	/// **throws** an exception if the provided BaseString is not utf8 compliant
	inline BaseString(const char* first, const char* last, const Allocator& alloc = Allocator()) : BaseString() {
		this->append(first, last);
	}
	
	/// 9) Constructs the BaseString with the contents of the initializer list ilist
	inline BaseString(std::initializer_list<CharT> ilist, const Allocator& alloc = Allocator()) : base_class(ilist, alloc){}
	
	/// 9.1) Constructs the BaseString with the contents of the initializer list ilist
	inline BaseString(std::initializer_list<char> ilist, const Allocator& alloc = Allocator()) : BaseString() {this->append(ilist);}
	
	/// 10.2) Construct from a std::string_view
	explicit inline BaseString(std::string_view strv, const Allocator& alloc = Allocator()) : BaseString(strv.cbegin(), strv.cend(), alloc){}
	
	explicit inline BaseString(const std::string& str, const Allocator& alloc = Allocator()) : BaseString(str.cbegin(), str.cend(), alloc){}
	
	/// 11) Implicitly converts t to a BaseString view sv as if by std::basic_string_view<CharT, Traits> sv = t;, 
	/// then initializes the BaseString with the subrange [pos, pos + n) of sv as if by basic_string(sv.substr(pos, n), alloc). 
	/// This overload participates in overload resolution only if std::is_convertible_v<const StringViewLike&, std::basic_string_view<CharT, Traits>> is true .
	template <class StringViewLike>
	inline BaseString(const StringViewLike& t, size_type pos, size_type n, const Allocator& alloc = Allocator()) : base_class(t, pos, n, alloc){}
	
	/// 10.1) Construct from a std::basic_string_view<utf8::Char, ...>
	template<class _Traits = std::char_traits<CharT>>
	explicit inline BaseString(std::basic_string_view<CharT, _Traits> strv, size_type pos, size_type n, const Allocator& alloc = Allocator()) : BaseString(strv.cbegin() + pos, strv.cend() + pos + n, alloc){}
	
	/// 10.2) Construct from a std::string_view
	explicit inline BaseString(std::string_view strv, size_type pos, size_type n, const Allocator& alloc = Allocator()) : BaseString(strv.cbegin() + pos, strv.cend() + pos + n, alloc){}
			  
	/// 12) basic_string cannot be constructed from nullptr.
	BaseString(std::nullptr_t) = delete;
	
	/// appends the provided ranged c-string to this BaseString. 
	/// only works if the character is convertible to utf8::Char
	template<class CharItr>
	 BaseString& _append(CharItr first, CharItr last){
		this->reserve(std::distance(first, last));
		while(first < last){
			Char c;
			auto newfirst = c.assign(first, last);
			this->base_class::push_back(c);
			if(newfirst == first){
				 throw std::invalid_argument( "Error in constructor utf8::BaseString(const char* first, const char* last). The provided c-string is not utf8 compliant." );
			}else{
				first = newfirst;
			}
		}  
		return *this;
	}
	
	template<class CharItr>
	BaseString& append(CharItr first, CharItr last){return this->_append(first, last);}
	
	/// appends the provided ranged c-string to this BaseString
	inline BaseString& append(const BaseString& str){this->base_class::append(str); return *this;}
	inline BaseString& append(const BaseString& str, size_type pos, size_type n){this->base_class::append(str, pos, n); return *this;}
	inline BaseString& append(Char c){this->base_class::push_back(c); return *this;}
	inline BaseString& append(char c){this->base_class::push_back(Char(c)); return *this;}
	inline BaseString& append(const char* first){return this->_append(first, first + std::strlen(first));}
	inline BaseString& append(const char* first, const char* last){return this->_append(first, last);}
	inline BaseString& append(const char* first, size_type n){return this->_append(first, first + n);}
	inline BaseString& append(const std::string& str){return this->_append(str.begin(), str.end());}
	inline BaseString& append(std::string_view str){return this->_append(str.begin(), str.end());}
	inline BaseString& append(std::string_view str, size_type pos, size_type n){return this->_append(str.begin() + pos, str.end() + pos + n);}
	inline BaseString& append(std::initializer_list<char> ilist){return this->_append(ilist.begin(), ilist.end());}
	
	/// Appends the given character ch to the end of the BaseString. 
	inline void push_back(char c){this->base_class::push_back(Char(c));}
	
	/// takes one utf8 character from the c_str and advances the iterator
	/// returns the advances iterator to the next byte after the consumend utf8 character 
	/// if no utf8 confomandt character has been detected, nothing will be pushed to the BaseString
	/// and the returned iterator is equal to the provied one. 
	inline const char* push_back(const char* c_str){
		Char c;
		const auto itr = c.assign(c_str);
		if(itr != c_str){
			this->base_class::push_back(c);
		}
		return itr;
	}
	
	/// takes one character from the c-string and pushes it into the BaseString
	/// Note that this function only appends one character at a time.
	/// returns the iterator after the consumed utf8 symbol
	/// does not push back if the symbol is not utf8 conformant
	template<class CharItr>
	inline CharItr push_back(CharItr first, CharItr last){
		Char c;
		const auto itr = c.assign(first, last);
		if(itr != first){
			this->base_class::push_back(c);
		}
		return itr;	
	}
	
	/// Takes the first 
	inline const char* push_back(const char* first, size_type n){return this->push_back(first, first + n);}
	inline std::string push_back(const std::string& str){return this->push_back(str.cbegin(), str.cend());}
	inline void push_back(std::string_view str){return this->push_back(str.cbegin(), str.cend());}
	
	
	/// appends one given character or BaseString to this BaseString
	inline BaseString& operator+=(Char c){return this->append(c);}
	inline BaseString& operator+=(const BaseString& str){return this->append(str);}
	inline BaseString& operator+=(char c){return this->append(c);}
	inline BaseString& operator+=(const char* first){return this->append(first);}
	inline BaseString& operator+=(const std::string& str){return this->append(str);}
	inline BaseString& operator+=(std::string_view str){return this->append(str);}
	inline BaseString& operator+=(std::initializer_list<char> ilist){return this->append(ilist);}
	
	/// appends one given character or BaseString to this BaseString
	friend inline BaseString& operator<<(BaseString& stream, const BaseString& str){return stream.append(str);}
	friend inline BaseString& operator<<(BaseString& stream, char c){return stream.append(c);}
	friend inline BaseString& operator<<(BaseString& stream, Char c){return stream.append(c);}
	friend inline BaseString& operator<<(BaseString& stream, const char* first){return stream.append(first);}
	friend inline BaseString& operator<<(BaseString& stream, const std::string& str){return stream.append(str);}
	friend inline BaseString& operator<<(BaseString& stream, std::string_view str){return stream.append(str);}
	friend inline BaseString& operator<<(BaseString& stream, std::initializer_list<char> ilist){return stream.append(ilist);}
	
	/// assigns a char c-string
	inline BaseString& assign(const BaseString& str){this->operator=(str); return *this;}
	inline BaseString& assign(BaseString&& str){this->operator=(std::move(str)); return *this;}
	inline BaseString& assign(char c){this->clear(); return this->append(c);}
	inline BaseString& assign(Char c){this->clear(); return this->append(c);}
	inline BaseString& assign(const char* str){this->clear(); return this->append(str);}
	inline BaseString& assign(const char* first, const char* last){this->clear(); return this->append(first, last);}
	inline BaseString& assign(const char* first, size_type n){this->clear(); return this->append(first, n);}
	inline BaseString& assign(const std::string& str){this->clear(); return this->append(str);}
	inline BaseString& assign(std::string_view str){this->clear(); return this->append(str);}
	inline BaseString& assign(std::initializer_list<char> ilist){this->clear(); return this->append(ilist);}
	
	/// assignment from std containers that use char
	inline BaseString& operator=(char c){this->clear(); return this->append(c);}
	inline BaseString& operator=(const char* str){this->clear(); return this->append(str);}
	inline BaseString& operator=(const std::string& str){this->clear(); return this->append(str);}
	inline BaseString& operator=(std::string_view str){this->clear(); return this->append(str);}
	inline BaseString& operator=(std::initializer_list<char> ilist){this->clear(); return this->append(ilist);}
	
	/// concattenate two BaseStrings
	friend inline BaseString operator+(const BaseString& lhs, const BaseString& rhs){
		BaseString result;
		result.reserve(lhs.size() + rhs.size());
		result = lhs;
		result += rhs;
		return result;
	}
	
	/// concattenate two BaseStrings and use the memory of the lhs
	friend inline BaseString operator+(BaseString&& lhs, const BaseString& rhs){lhs += rhs; return std::move(lhs);}
	
	/// concattenate two BaseStrings and use the memory of the rhs
	friend inline BaseString operator+(const BaseString& lhs, BaseString&& rhs){rhs.base_class::insert(rhs.begin(), lhs.cbegin(), lhs.cend()); return std::move(rhs);}
	
	/// concattenate two BaseStrings and use the memory of the lhs
	friend inline BaseString operator+(BaseString&& lhs, BaseString&& rhs){lhs += rhs; return std::move(lhs);}
	
	/// appends this BaseString to the provided std::string
	inline void append_to(std::string& str) const {for(auto elem : *this) str.append(elem.to_std_string_view());}
	
	/// returns the size of an c-string that would hold the same information
	inline size_type c_str_size() const {
		size_type sum = 0; 
		for(auto elem : *this) sum += elem.size(); 
		return sum;
	}
	
	/// returns an std::string with the content of this BaseString.
	/// if possible use append_to(std::string&) and reserve enouch storage beforehand to save on calls to malloc
	inline std::string to_std_string() const {
		std::string s;
		s.reserve(this->c_str_size());
		for(auto elem : *this) s.append(elem.to_std_string_view());
		return s;
	}
	
	template<class OStream>
	friend inline OStream& operator << (OStream& stream, BaseString str){
		for(const auto elem : str){
			stream << elem;
		}
		return stream;
	}
	
	/// inserts one element at the position given by the index
	inline BaseString& insert(size_type index, Char c){this->base_class::insert(index, 1, c); return *this;}
	inline BaseString& insert(size_type index, char c){this->base_class::insert(index, 1, Char(c)); return *this;}
	
	/// removes one element at the position given by the index
	inline BaseString& erase(size_type index){this->base_class::erase(index, 1); return *this;}
	
	/// removes coint many elements or until the string-end at the position given by the index
	inline BaseString& erase(size_type index, size_type count){this->base_class::erase(index, count); return *this;}

};

using string = BaseString<>;

}


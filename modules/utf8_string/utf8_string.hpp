
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

using string_view = std::basic_string_view<Char>;

/// string that stores utf8::Char types 
///
/// * is allocator aware
/// * supports all constructors that a std::basic_string supports
template<class Traits = std::char_traits<Char>, class Allocator = std::allocator<Char>>
class string : public std::basic_string<Char, Traits, Allocator>{
	
public:
	using CharT = Char;
	using base_class = std::basic_string<Char, Traits, Allocator>;
	using size_type = typename base_class::size_type;
	using iterator = typename base_class::iterator;
	using const_iterator = typename base_class::const_iterator;
	using reference = typename base_class::reference;
	using const_reference = typename base_class::const_reference;
	
	/// 1) default constuctor
	inline string() = default;
	
	/// copy constructable
	inline string(const string&) = default;
	
	/// copy assignable
	inline string& operator = (const string&) = default;
	
	/// move constructable
	inline string(string&&) = default;
	
	/// move assignable
	inline string& operator = (string&&) = default;
	
	/// 2) Constructs the string with count copies of character ch. 
	/// This constructor is not used for class template argument deduction 
	/// if the Allocator type that would be deduced does not qualify as an allocator. (since C++17)
	inline string(size_type count, CharT ch, const Allocator& alloc = Allocator()) : base_class(count, ch, alloc){}
	
	/// 2.1) Constructs the string with count copies of character ch. 
	/// This constructor is not used for class template argument deduction 
	/// if the Allocator type that would be deduced does not qualify as an allocator. (since C++17)
	inline string(size_type count, char ch, const Allocator& alloc = Allocator()) : base_class(count, Char(ch), alloc){}
	
	/// 3) Constructs the string with a substring [pos, pos + count) of other. 
	/// If count == npos, if count is not specified, or if the requested substring lasts past the end of the string, 
	/// the resulting substring is [pos, other.size()). If other is an rvalue reference, it is left in a valid but unspecified state. (since C++23)
	inline string(const string& other, size_type pos, size_type count, const Allocator& alloc = Allocator()) : base_class (other, pos, count, alloc){}
	
	/// 4) Constructs the string with the first count characters of character string pointed to by s. 
	/// s can contain null characters. The length of the string is count. 
	/// The behavior is undefined if [s, s + count) is not a valid range.
	inline string(const CharT* s, size_type count, const Allocator& alloc = Allocator()) : base_class( s, count, alloc){}
	
	/// 4.1) Constructs the string with the first count characters of character string pointed to by s. 
	/// s can contain null characters. The length of the string is count. 
	/// The behavior is undefined if [s, s + count) is not a valid range.
	inline string(const char* s, size_type count, const Allocator& alloc = Allocator()) : string(s, s + count, alloc){}
	
	
	/// 5) Constructs the string with the contents initialized with a copy of the null-terminated character string pointed to by s. 
	/// The length of the string is determined by the first null character. 
	/// The behavior is undefined if [s, s + Traits::length(s)) is not a valid range (for example, if s is a null pointer). 
	/// This constructor is not used for class template argument deduction 
	/// if the Allocator type that would be deduced does not qualify as an allocator. (since C++17)
	explicit inline string(const CharT* s, const Allocator& alloc = Allocator()) : base_class(s, alloc){}
	
	/// 5.1) assigns a null terminated c-string to the string class
	/// **throws** an exception if the provided string is not utf8 compliant
	explicit inline string(const char* c_str, const Allocator& alloc = Allocator()) : string(c_str, c_str + std::strlen(c_str), alloc) /*throws*/ {}
	
	/// 6) Constructs the string with the contents of the range [first, last). 
	template<class InputIt>
	inline string(InputIt first, InputIt last, const Allocator& alloc = Allocator()) : string() {this->append(first, last);}
	
	inline string(const_iterator first, const_iterator last, const Allocator& alloc = Allocator()) : base_class(first, last, alloc){}
	inline string(iterator first, iterator last, const Allocator& alloc = Allocator()) : base_class(first, last, alloc){}
	
	/// 6.1) Constructs the string with the contents of the range [first, last) and assigns the c-string to the string. 
	/// **throws** an exception if the provided string is not utf8 compliant
	inline string(const char* first, const char* last, const Allocator& alloc = Allocator()) : string() {
		this->append(first, last);
	}
	
	/// 9) Constructs the string with the contents of the initializer list ilist
	string(std::initializer_list<CharT> ilist, const Allocator& alloc = Allocator()) : base_class(ilist, alloc){}
	
	/// 9.1) Constructs the string with the contents of the initializer list ilist
	string(std::initializer_list<char> ilist, const Allocator& alloc = Allocator()) : string(alloc){this->append(ilist);}
	
	/// 10) Implicitly converts t to a string view sv as if by std::basic_string_view<CharT, Traits> sv = t;, 
	/// then initializes the string with the contents of sv, as if by basic_string(sv.data(), sv.size(), alloc). 
	/// This overload participates in overload resolution only if 
	/// std::is_convertible_v<const StringViewLike&, std::basic_string_view<CharT, Traits>> 
	/// is true and std::is_convertible_v<const StringViewLike&, const CharT*> is false
	template <class StringViewLike>
	explicit inline string(const StringViewLike& t, const Allocator& alloc = Allocator()) : base_class(t, alloc){}
	
	/// 10.1) Construct from a std::basic_string_view<utf8::Char, ...>
	template<class _Traits = std::char_traits<CharT>>
	explicit inline string(std::basic_string_view<CharT, _Traits> strv, const Allocator& alloc = Allocator()) : string(strv.cbegin(), strv.cend(), alloc){}
	
	/// 10.2) Construct from a std::string_view
	explicit inline string(std::string_view strv, const Allocator& alloc = Allocator()) : string(strv.cbegin(), strv.cend(), alloc){}
	
	explicit inline string(const std::string& str, const Allocator& alloc = Allocator()) : string(str.cbegin(), str.cend(), alloc){}
	
	/// 11) Implicitly converts t to a string view sv as if by std::basic_string_view<CharT, Traits> sv = t;, 
	/// then initializes the string with the subrange [pos, pos + n) of sv as if by basic_string(sv.substr(pos, n), alloc). 
	/// This overload participates in overload resolution only if std::is_convertible_v<const StringViewLike&, std::basic_string_view<CharT, Traits>> is true .
	template <class StringViewLike>
	inline string(const StringViewLike& t, size_type pos, size_type n, const Allocator& alloc = Allocator()) : base_class(t, pos, n, alloc){}
	
	/// 10.1) Construct from a std::basic_string_view<utf8::Char, ...>
	template<class _Traits = std::char_traits<CharT>>
	explicit inline string(std::basic_string_view<CharT, _Traits> strv, size_type pos, size_type n, const Allocator& alloc = Allocator()) : string(strv.cbegin() + pos, strv.cend() + pos + n, alloc){}
	
	/// 10.2) Construct from a std::string_view
	explicit inline string(std::string_view strv, size_type pos, size_type n, const Allocator& alloc = Allocator()) : string(strv.cbegin() + pos, strv.cend() + pos + n, alloc){}
			  
	/// 12) basic_string cannot be constructed from nullptr.
	string(std::nullptr_t) = delete;
	
	/// appends the provided ranged c-string to this string. 
	/// only works if the character is convertible to utf8::Char
	template<class CharItr>
	string& _append(CharItr first, CharItr last){
		this->reserve(std::distance(first, last));
		while(first < last){
			Char c;
			auto newfirst = c.assign(first, last);
			this->base_class::push_back(c);
			if(newfirst == first){
				 throw std::invalid_argument( "Error in constructor utf8::string(const char* first, const char* last). The provided c-string is not utf8 compliant." );
			}else{
				first = newfirst;
			}
		}  
		return *this;
	}
	
	template<class CharItr>
	string& append(CharItr first, CharItr last){return this->_append(first, last);}
	
	/// appends the provided ranged c-string to this string
	inline string& append(char c){this->base_class::push_back(Char(c)); return *this;}
	inline string& append(const char* first){return this->_append(first, first + std::strlen(first));}
	inline string& append(const char* first, const char* last){return this->_append(first, last);}
	inline string& append(const char* first, size_type n){return this->_append(first, first + n);}
	inline string& append(const std::string& str){return this->_append(str.begin(), str.end());}
	inline string& append(std::string_view str){return this->_append(str.begin(), str.end());}
	inline string& append(std::string_view str, size_type pos, size_type n){return this->_append(str.begin() + pos, str.end() + pos + n);}
	inline string& append(std::initializer_list<char> ilist){return this->_append(ilist.begin(), ilist.end());}
	
	/// Appends the given character ch to the end of the string. 
	inline void push_back(char c){this->base_class::push_back(Char(c));}
	
	/// takes one utf8 character from the c_str and advances the iterator
	/// returns the advances iterator to the next byte after the consumend utf8 character 
	/// if no utf8 confomandt character has been detected, nothing will be pushed to the string
	/// and the returned iterator is equal to the provied one. 
	inline const char* push_back(const char* c_str){
		Char c;
		const auto itr = c.assign(c_str);
		if(itr != c_str){
			this->base_class::push_back(c);
		}
		return itr;
	}
	
	/// takes one character from the c-string and pushes it into the string
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
	
	
	/// appends one given character or string to this string
	inline string& operator+=(const string& str){this->base_class::append(str); return *this;}
	inline string& operator+=(char c){return this->append(c);}
	inline string& operator+=(const char* first){return this->append(first);}
	inline string& operator+=(const std::string& str){return this->append(str);}
	inline string& operator+=(std::string_view str){return this->append(str);}
	inline string& operator+=(std::initializer_list<char> ilist){return this->append(ilist);}
	
	/// assigns a char c-string
	inline string& assign(char c){this->clear(); return this->append(c);}
	inline string& assign(const char* str){this->clear(); return this->append(str);}
	inline string& assign(const char* first, const char* last){this->clear(); return this->append(first, last);}
	inline string& assign(const char* first, size_type n){this->clear(); return this->append(first, n);}
	inline string& assign(const std::string& str){this->clear(); return this->append(str);}
	inline string& assign(std::string_view str){this->clear(); return this->append(str);}
	inline string& assign(std::initializer_list<char> ilist){this->clear(); return this->append(ilist);}
	
	/// assignment from std containers that use char
	inline string& operator=(char c){this->clear(); return this->append(c);}
	inline string& operator=(const char* str){this->clear(); return this->append(str);}
	inline string& operator=(const std::string& str){this->clear(); return this->append(str);}
	inline string& operator=(std::string_view str){this->clear(); return this->append(str);}
	inline string& operator=(std::initializer_list<char> ilist){this->clear(); return this->append(ilist);}
	
	/// concattenate two strings
	friend inline string operator+(const string& lhs, const string& rhs){
		string result;
		result.reserve(lhs.size() + rhs.size());
		result = lhs;
		result += rhs;
		return result;
	}
	
	/// concattenate two strings and use the memory of the lhs
	friend inline string operator+(string&& lhs, const string& rhs){lhs += rhs; return lhs;}
	
	/// concattenate two strings and use the memory of the rhs
	friend inline string operator+(const string& lhs, string&& rhs){ rhs.insert(rhs.begin(), lhs.cbegin(), lhs.cend()); return rhs;}
	
	/// concattenate two strings and use the memory of the lhs
	friend inline string operator+(string&& lhs, string&& rhs){lhs += rhs; return lhs;}
	
	
	/// appends this string to the provided std::string
	inline string& append_to(std::string& str) const { 
		for(auto elem : *this) str.append(elem.to_string_view()); 
		return *this; 
	}
	
	/// returns the size of an c-string that would hold the same information
	inline size_type c_str_size() const {
		size_type sum = 0; 
		for(auto elem : *this) sum += elem.size(); 
		return sum;
	}
	
	/// returns an std::string with the content of this string.
	/// if possible use append_to(std::string&) and reserve enouch storage beforehand to save on calls to malloc
	inline std::string to_std_string() const {
		std::string s;
		s.reserve(this->c_str_size());
		for(auto elem : *this) s.append(elem.to_std_string_view());
		return s;
	}
	
	template<class OStream>
	friend inline OStream& operator << (OStream& stream, string str){
		for(const auto elem : str){
			stream << elem;
		}
		return stream;
	}

};

}


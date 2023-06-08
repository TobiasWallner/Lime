#pragma once
// c++ std
#include <utility>

// utf8_string
#include <utf8_string.hpp>

// Project
#include "RenderTrait.hpp"
#include "FontStyle.hpp"
#include "FontStyleList.hpp"


/*
	A line is the combination of an utf8 string and a list of 
	styles that are used to format the string at specific locations
	in the string.
	
	The line is allocator aware
*/

namespace TermGui{

class ColorString : public RenderTrait{

private:
	using size_type = utf8::string::size_type;

	using string_type = utf8::string;

	string_type _string;
	iStylesList _styles;

public:
	
	/// 1) default constuctor
	inline ColorString() = default;
	
	/// copy constructable
	inline ColorString(const ColorString&) = default;
	
	/// copy assignable
	inline ColorString& operator = (const ColorString&) = default;
	
	/// move constructable
	inline ColorString(ColorString&&) = default;
	
	/// move assignable
	inline ColorString& operator = (ColorString&&) = default;
	
	/// 2) Constructs the BaseString with count copies of character ch. 
	inline ColorString(size_type count, utf8::Char ch) : _string(count, ch){}
	
	/// 2.1) Constructs the BaseString with count copies of character ch.
	inline ColorString(size_type count, char ch) : _string(count, ch){}
	
	/// 3) Constructs the BaseString with a substring [pos, pos + count) of other.
	inline ColorString(const utf8::string& other, size_type pos, size_type count) : _string(other, pos, count){}
	
	/// 4) Constructs the BaseString with the first count characters of character BaseString pointed to by s. 
	inline ColorString(const utf8::Char* s, size_type count) : _string( s, count){}
	
	/// 4.1) Constructs the BaseString with the first count characters of character BaseString pointed to by s. 
	/// s can contain null characters. The length of the BaseString is count. 
	/// The behavior is undefined if [s, s + count) is not a valid range.
	inline ColorString(const char* s, size_type count) : _string(s, count){}
	
	/// 5) Constructs the BaseString with the contents initialized with a copy of the null-terminated character BaseString
	explicit inline ColorString(const utf8::Char* s) : _string(s){}
	
	/// 5.1) assigns a null terminated c-string to the BaseString class
	/// **throws** an exception if the provided BaseString is not utf8 compliant
	explicit inline ColorString(const char* c_str) : _string(c_str) {}
	
	/// 6) Constructs the BaseString with the contents of the range [first, last). 
	template<class InputIt>
	inline ColorString(InputIt first, InputIt last) : _string(first, last) {}
	
	/// 9) Constructs the BaseString with the contents of the initializer list ilist
	inline ColorString(std::initializer_list<utf8::Char> ilist) : _string(ilist){}
	
	/// 9.1) Constructs the BaseString with the contents of the initializer list ilist
	inline ColorString(std::initializer_list<char> ilist) : _string(ilist){}
	
	/// 10) Implicitly converts t to a BaseString view sv as if by std::basic_string_view<utf8::Char, Traits> sv = t;, 
	/// then initializes the BaseString with the contents of sv, as if by basic_string(sv.data(), sv.size(), alloc). 
	/// This overload participates in overload resolution only if 
	/// std::is_convertible_v<const StringViewLike&, std::basic_string_view<utf8::Char, Traits>> 
	/// is true and std::is_convertible_v<const StringViewLike&, const utf8::Char*> is false
	template <class StringViewLike>
	explicit inline ColorString(const StringViewLike& t) : _string(t){}
	
	/// Constructs the ColorString with a substring [pos, pos + count) of other. 
	inline ColorString(const ColorString& other, size_type pos, size_type count) : 
		_string (other._string, pos, count),
		_styles(other._styles, pos, count){}
		
	/// returns true if there is nothing in the list, false otherwise
	/// the List class has to ensure that if the string is empty 
	/// that also the command list is empty too
	inline bool empty() const {return this->_string.empty();}
	
	/// returns the number of utf8 characters in the string
	inline size_type size() const {return this->_string.size();}
	
	inline void clear(){
		this->_string.clear();
		this->_styles.clear();
	}
	
	/// returns a const reference pointer to ther command list
	inline const auto& styles() const {return this->_styles;}
	
	/// appends a line and its formats (aka. command list)
	ColorString& append(const ColorString& other);
	ColorString& append(ColorString&& other);
	
	/// moves the content from the other string in range [pos, n + pos) to this string
	/// not that the other strings content in that range will be erased
	ColorString& move_append(ColorString& other, size_type pos, size_type n);
	
	
	/// append to the ColorString as you would append to the underlying string type
	template<class CharItr>
	inline ColorString& append(CharItr first, CharItr last){this->_string.append(first, last); return *this;}
	inline ColorString& append(utf8::Char c){this->_string.append(c); return *this;}
	inline ColorString& append(char c){this->_string.append(c); return *this;}
	inline ColorString& append(const char* first){this->_string.append(first); return *this;}
	inline ColorString& append(const char* first, size_type n){this->_string.append(first, first + n); return *this;}
	inline ColorString& append(const std::string& str){this->_string.append(str); return *this;}
	inline ColorString& append(std::string_view str){this->_string.append(str); return *this;}
	inline ColorString& append(std::string_view str, size_type pos, size_type n){this->_string.append(str, pos, n); return *this;}
	
	inline ColorString& operator+=(utf8::Char c){return this->append(c);}
	inline ColorString& operator+=(char c){return this->append(c);}
	inline ColorString& operator+=(const char* first){return this->append(first);}
	inline ColorString& operator+=(const std::string& str){return this->append(str);}
	inline ColorString& operator+=(std::string_view str){return this->append(str);}
	
	inline ColorString& operator<<(utf8::Char c){return this->append(c);}
	inline ColorString& operator<<(char c){return this->append(c);}
	inline ColorString& operator<<(const char* first){return this->append(first);}
	inline ColorString& operator<<(const std::string& str){return this->append(str);}
	inline ColorString& operator<<(std::string_view str){return this->append(str);}
	
	/// appends the given command to the line which will then format all string elements inserted after wards	
	inline ColorString& append(const FontStyle& fontStyle){
		//TODO: change to inser_override -> therefore implement insert_override
		this->_styles.add_override(fontStyle, this->_string.size());
		return *this;
	}
	inline ColorString& operator << (const FontStyle& fontStyle){ return this->append(fontStyle); }
	inline ColorString& operator += (const FontStyle& fontStyle){ return this->append(fontStyle); }
	
	/// push_back as you would to the underlying string 
	inline void posh_back(char c){this->_string.push_back(c);}
	inline const char* push_back(const char* c_str){return this->_string.push_back(c_str);}
	
	template<class CharItr>
	inline CharItr push_back(CharItr first, CharItr last){return this->_string.push_back(first, last);}
	
	/// assigning a new color or format to a string will only clear the previous stored styles and formats
	/// the stored string stays untouched
	inline ColorString& assign(const FontStyle& command){this->_styles.assign(command); return *this;}
	inline ColorString& operator=(const FontStyle& command){return this->assign(command);}

	
	/// assigning any unformated and uncolored string to a string will clear the previous stored formats and colores
	template<class CharItr>
	inline ColorString& assign(CharItr first, CharItr last){this->_styles.clear(); return this->_string.assign(first, last);}
	inline ColorString& assign(utf8::Char c){this->_styles.clear(); this->_string.assign(c); return *this;}
	inline ColorString& assign(char c){this->_styles.clear(); this->_string.assign(c); return *this;}
	inline ColorString& assign(const char* first){this->_styles.clear(); this->_string.assign(first); return *this;}
	inline ColorString& assign(const char* first, size_type n){this->_styles.clear(); this->_string.assign(first, first + n); return *this;}
	inline ColorString& assign(const std::string& str){this->_styles.clear(); this->_string.assign(str); return *this;}
	inline ColorString& assign(std::string_view str){this->_styles.clear(); this->_string.assign(str); return *this;}
	inline ColorString& assign(std::initializer_list<char> ilist){this->_styles.clear(); this->_string.assign(ilist); return *this;}
	
	inline ColorString& operator=(utf8::Char c){return this->assign(c);}
	inline ColorString& operator=(char c){return this->assign(c);}
	inline ColorString& operator=(const char* first){return this->assign(first);}
	inline ColorString& operator=(const std::string& str){return this->assign(str);}
	inline ColorString& operator=(std::string_view str){return this->assign(str);}
	
	// TODO: iterators
	
	
	void render(std::string& outputString) const override;
	
	/// inserts a character at the given position
	ColorString& insert(size_type index, utf8::Char c);
	inline ColorString& insert(size_type index, char c){this->insert(index, utf8::Char(c)); return *this;}
	
	// removes the character at the index position from the string.
	// styles that are at or before the erased index stay unchanged.
	// styles that are after the index will be moved one index to the front.
	// styles that are thus moved to and existing command list will override that list.
	ColorString& erase(size_type index);
	
	// as erase with one index but removes the inizes from the closed open range [index_from, index_to)
	ColorString& erase(size_type index_from, size_type index_to);
	inline bool add_override(const FontStyle& command, size_type index){return this->_styles.add_override(command, index);}

};

}//TermGui
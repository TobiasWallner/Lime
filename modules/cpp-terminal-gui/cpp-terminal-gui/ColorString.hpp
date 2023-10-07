#pragma once
// c++ std
#include <utility>
#include <string>

// utf8_string
#include <utf8/string.hpp>

// Project 
#include "FontStyleList.hpp"

/*
	A line is the combination of an utf8 string and a list of 
	styles that are used to format the string at specific locations
	in the string.
	
	The line is allocator aware
*/

namespace TermGui{

// TODO: change color string so that every element has its own color information
class ColorString{

public:
	using string_type = utf8::string;
	using size_type = string_type::size_type;
	using difference_type = string_type::difference_type;

private:
	string_type _string;
	iStylesList _styles;

public:
	
	string_type::const_iterator string_cbegin() const { return this->_string.cbegin(); }
	string_type::const_iterator string_cend() const { return this->_string.cend(); }
	
	iStylesList::const_iterator style_list_cbegin() const { return this->_styles.cbegin(); }
	iStylesList::const_iterator style_list_cend() const { return this->_styles.cend(); }
	
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
	inline ColorString(size_type count, utf8::Char ch){this->append(count, ch);}
	
	/// 2.1) Constructs the BaseString with count copies of character ch.
	inline ColorString(size_type count, char ch) {this->append(count, ch);}
	
	/// 4.1) Constructs the BaseString with the first count characters of character BaseString pointed to by s. 
	/// s can contain null characters. The length of the BaseString is count. 
	/// The behavior is undefined if [s, s + count) is not a valid range.
	inline ColorString(const char* s, size_type count) {this->append(s, count);}
	
	/// 5.1) assigns a null terminated c-string to the BaseString class
	/// **throws** an exception if the provided BaseString is not utf8 compliant
	explicit inline ColorString(const char* c_str) {this->append(c_str);}
	
	explicit inline ColorString(std::string_view str) {this->append(str);}
	explicit inline ColorString(utf8::const_string_view str) {this->append(str);}
	explicit inline ColorString(utf8::string_view str) {this->append(str);}
	explicit inline ColorString(const utf8::string& str) {this->append(str);}
	explicit inline ColorString(utf8::string&& str) {this->assign(std::move(str));}
		
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
	constexpr ColorString& append(utf8::Char c){this->_string.append(c); return *this;}
	constexpr ColorString& append(size_type count, utf8::Char c){this->_string.append(count, c); return *this;}
	constexpr ColorString& append(char c){this->_string.append(c); return *this; return *this;}
	constexpr ColorString& append(size_type count, char c){this->_string.append(count, c); return *this; return *this;}
	constexpr ColorString& append(const char* first){return this->append(first, utf8::strlen(first));}
	constexpr ColorString& append(const char* first, size_type n){return this->append(first, first + n);}
	constexpr ColorString& append(const char* first, const char* last){return this->append(utf8::const_string_view(first, last));}
	constexpr ColorString& append(const std::string& str){return this->append(utf8::const_string_view(&*str.begin(), &*str.end()));}
	constexpr ColorString& append(const std::string_view& str){return this->append(utf8::const_string_view(&*str.begin(), &*str.end()));}
	constexpr ColorString& append(const utf8::string& str){return this->append(utf8::const_string_view(&*str.begin(), &*str.end()));}
	constexpr ColorString& append(utf8::string_view str){this->_string.append(str); return *this;}
	constexpr ColorString& append(utf8::const_string_view str){this->_string.append(str); return *this;}
	
	inline ColorString& operator+=(utf8::Char c){return this->append(c);}
	inline ColorString& operator+=(char c){return this->append(c);}
	inline ColorString& operator+=(const char* first){return this->append(first);}
	inline ColorString& operator+=(const std::string& str){return this->append(str);}
	inline ColorString& operator+=(std::string_view str){return this->append(str);}
	inline ColorString& operator+=(const utf8::string& str){return this->append(str);}
	inline ColorString& operator+=(utf8::string_view str){return this->append(str);}
	inline ColorString& operator+=(utf8::const_string_view str){return this->append(str);}
	
	inline ColorString& operator<<(utf8::Char c){return this->append(c);}
	inline ColorString& operator<<(char c){return this->append(c);}
	inline ColorString& operator<<(const char* first){return this->append(first);}
	inline ColorString& operator<<(const std::string& str){return this->append(str);}
	inline ColorString& operator<<(std::string_view str){return this->append(str);}
	inline ColorString& operator<<(const utf8::string& str){return this->append(str);}
	inline ColorString& operator<<(utf8::string_view str){return this->append(str);}
	inline ColorString& operator<<(utf8::const_string_view str){return this->append(str);}
	
	/// appends the given command to the line which will then format all string elements inserted after wards	
	inline ColorString& append(const FontStyle& fontStyle){
		//TODO: change to inser_override -> therefore implement insert_override
		this->_styles.add_override(fontStyle, this->_string.size());
		return *this;
	}
	inline ColorString& operator << (const FontStyle& fontStyle){ return this->append(fontStyle); }
	inline ColorString& operator += (const FontStyle& fontStyle){ return this->append(fontStyle); }
	
	/// push_back as you would to the underlying string 
	inline void push_back(char c){this->_string.push_back(c);}
	inline void push_back(const char* c_str){this->_string.push_back(c_str);}
	
	template<class CharItr>
	inline CharItr push_back(CharItr first, CharItr last){return this->_string.push_back(first, last);}
	
	/// assigning a new color or format to a string will only clear the previous stored styles and formats
	/// the stored string stays untouched
	inline ColorString& assign(const FontStyle& command){this->_styles.assign(command); return *this;}
	inline ColorString& operator=(const FontStyle& command){return this->assign(command);}

	
	/// assigning any unformated and uncolored string to a string will clear the previous stored formats and colores
	
	/*TODO: rewrite the assign functions so that they make sense with the new stylse*/
	constexpr ColorString& assign(utf8::Char c){this->_styles.clear(); this->_string.assign(c); return *this;}
	constexpr ColorString& assign(size_type count, utf8::Char ch){this->_styles.clear(); this->_string.assign(count, ch); return *this;}
	inline ColorString& assign(char c){this->_styles.clear(); this->_string.assign(c); return *this;}
	inline ColorString& assign(size_type count, char ch){this->_styles.clear(); this->_string.assign(count, ch); return *this;}
	inline ColorString& assign(const char* first){this->_styles.clear(); this->_string.assign(first); return *this;}
	inline ColorString& assign(const char* first, size_type n){return this->assign(first, first + n);}
	inline ColorString& assign(const char* first, const char* last){this->_styles.clear(); this->_string.assign(first, last); return *this;}
	inline ColorString& assign(const std::string& str){this->_styles.clear(); this->_string.assign(str); return *this;}
	inline ColorString& assign(std::string_view str){this->_styles.clear(); this->_string.assign(str); return *this;}
	inline ColorString& assign(const utf8::string& str){this->_styles.clear(); this->_string.assign(str); return *this;}
	inline ColorString& assign(const utf8::const_string_view str){this->_styles.clear(); this->_string.assign(str); return *this;}
	
	inline ColorString& operator=(utf8::Char c){return this->assign(c);}
	inline ColorString& operator=(char c){return this->assign(c);}
	inline ColorString& operator=(const char* first){return this->assign(first);}
	inline ColorString& operator=(const std::string& str){return this->assign(str);}
	inline ColorString& operator=(std::string_view str){return this->assign(str);}
	inline ColorString& operator=(const utf8::string& str){return this->assign(str);}
	inline ColorString& operator=(const utf8::const_string_view str){return this->assign(str);}
	
	// TODO: iterators
	
	
	
	/// inserts a character at the given position
	ColorString& insert(size_type index, utf8::Char c);
	inline ColorString& insert(size_type index, char c){this->insert(index, utf8::Char(c)); return *this;}
	
	/// removes the character at the index position from the string.
	/// styles that are at or before the erased index stay unchanged.
	/// styles that are after the index will be moved one index to the front.
	/// styles that are thus moved to and existing command list will override that list.
	ColorString& erase(size_type index);
	
	/// as erase with one index but removes the inizes from the closed open range [index_from, index_to)
	ColorString& erase(size_type index_from, size_type index_to);
	inline bool add_override(const FontStyle& command, size_type index){return this->_styles.add_override(command, index);}
	
	template<class CharT, class Traits = std::char_traits<CharT>>
	inline friend std::basic_ostream<CharT, Traits>& operator << (std::basic_ostream<CharT, Traits>& stream, const ColorString& string){
		stream << string._string;
		return stream;
	}
	
	/// compares equal to the content of the string not the content of the style formats
	friend constexpr bool operator==(const ColorString& lhs, const ColorString& rhs){return lhs._string == rhs._string;}
	friend constexpr bool operator!=(const ColorString& lhs, const ColorString& rhs){return lhs._string != rhs._string;}

	void render(std::string& outputString) const;
};

}//TermGui
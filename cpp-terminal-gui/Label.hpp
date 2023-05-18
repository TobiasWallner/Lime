
#pragma once

#include "RenderTrait.hpp"

// C++ STD
#include <string>
#include <string_view>
#include <variant>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

namespace TermGUI{


/**
	A Label can store a formated and coloured string that can be rendered to the console terminal.
	The Label can contain one of the following string types:
		* a raw C-style string: `const char*`, `const char16_t*`, `const char32_t*`. 
			The Label does **not** free a `const char*` string. The Label does not own the string.
			It depends on the lifetime of the passed `const char*` 
			being longer or equally long as the Label itself.
			Use this for raw strings.
		
		* a string view
			The Label can view the string. This means that the label is not responsible for the 
			original string since it only has a view type.
			
		* a string
			It may also contain a string that is responsible for its own ressources.
			
	Note that the Label can only ever only store one line.
			
*/
class Label : public RenderTrait{
private:
	//the string that gets displayed	
	std::variant<std::string, std::string_view, const char*, const char16_t*, const char32_t*> _vstring; 
	
	//the cursour _position relative to the parent. 
	Term::Cursor _position; 
	
	//the label will only be printed up to the given length. If the length is negative then the whole label will be printed regardles.
	int _label_length = -1; 
	
public:
	
	explicit constexpr Label(const char* raw_string_view, Term::Cursor position = {0, 0}, label_length = -1) : 
			_vstring(raw_string_view), 
			_position(position)
			_label_length(label_length){}
	
	explicit constexpr Label(const char16_t* raw_string_view, Term::Cursor position = {0, 0}, label_length = -1) : 
			_vstring(raw_string_view), 
			_position(position)
			_label_length(label_length){}
			
	explicit constexpr Label(const char32_t* raw_string_view, Term::Cursor position = {0, 0}, label_length = -1) : 
			_vstring(raw_string_view), 
			_position(position)
			_label_length(label_length){}
			
	explicit constexpr Label(std::string_view string_view, Term::Cursor position = {0, 0}, label_length = -1) : 
			_vstring(string_view), 
			_position(position)
			_label_length(label_length){}
			
	explicit Label(std::string&& string, Term::Cursor position = {0, 0}, label_length = -1) : 
			_vstring(std::move(string)), 
			_position(position)
			_label_length(label_length){}
	
	inline void set_string(const char* raw_string_view){this->_vstring = raw_string_view;};
	inline void set_string(const char16_t* raw_string_view){this->_vstring = raw_string_view;};
	inline void set_string(const char32_t* raw_string_view){this->_vstring = raw_string_view;};
	inline void set_string(std::string_view string_view){this->_vstring = string_view;};
	inline void set_string(std::string&& string){this->_vstring = string;};
	
	inline void set_position(Term::Cursor position){this->_position = position;};
	
	/**
		The object renders its whole content into a new std::string object.
		This may cause new memory allocation.
		The rendered string will be a string of instructions useing ANSI codes for the 
		console.
	*/
	std::string render() const;
	
	/**
		The object renders its content into existing memory.
		The rendered string will be a string of instructions useing ANSI codes for the 
		console.		
	*/
	void render(std::string& output_string) const;
};	
	
}

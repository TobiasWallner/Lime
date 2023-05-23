
#pragma once

#include "RenderTrait.hpp"

// C++ STD
#include <cstdio>
#include <list>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>
#include <cpp-terminal/color.hpp>

// utf8_string
#include <utf8_string.hpp>

namespace TermGui{

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
public:

	using size_type = std::size_t;

private:

	// CommandIndexPair represents a command and the position at which the command should be printed into the output stream.
	

	utf8::string _string; // the string that should be printed
	
	

	Term::Cursor _position; 	// position of the label
	
	size_type _cursor_position; // poition of the cursor within the 
	
	//the label will only be printed up to the given length. If the length is negative then the whole label will be printed regardles.
	int _label_length = -1; 
	
public:
			
	Label(const utf8::string& string, Term::Cursor position = {0, 0}, int label_length = -1) : 
			_string(string), 
			_position(position),
			_label_length(label_length){}
			
	Label(utf8::string&& string, Term::Cursor position = {0, 0}, int label_length = -1) : 
			_string(std::move(string)), 
			_position(position),
			_label_length(label_length){}
	
	inline void change_foreground(Term::Color foregroundColor){
		this->_foregroundColour.push_back(
			ColorChangeIndex{
				foregroundColor, 
				static_cast<size_type>(_string.size())
			}
		);
	}
	
	inline void change_background(Term::Color backgroundColor){
		this->_backgroundColour.push_back(
			ColorChangeIndex{
				backgroundColor, 
				static_cast<size_type>(_string.size())
			}
		);
	}
	
	/*---- append ----*/
	inline Label& append(const utf8::string& str){this->_string.append(str); return *this;}
	
	
	/*---- assign ----*/
	inline Label& assign(const utf8::string& str){this->_string.assign(str); return *this;}
	inline Label& assign(utf8::string&& str){this->_string.assign(std::move(str)); return *this;}
	

	inline void set_position(Term::Cursor position){this->_position = position;};
	
	/**
		The object renders its content into existing memory.
		The rendered string will be a string of instructions useing ANSI codes for the 
		console.		
	*/
	void render(std::string& output_string) const;
};	
	
}


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
	std::string _string; 
	
	//the cursour _position relative to the parent. 
	Term::Cursor _position; 
	
	//the label will only be printed up to the given length. If the length is negative then the whole label will be printed regardles.
	int _label_length = -1; 
	
public:
			
	Label(const std::string& string, Term::Cursor position = {0, 0}, label_length = -1) : 
			_string(string), 
			_position(position)
			_label_length(label_length){}
			
	Label(std::string&& string, Term::Cursor position = {0, 0}, label_length = -1) : 
			_string(std::move(string)), 
			_position(position)
			_label_length(label_length){}
	
	inline void set_string(std::string&& string){this->_string = string;};
	
	inline void set_position(Term::Cursor position){this->_position = position;};
	
	/**
		The object renders its whole content into a new std::string object.
		This may cause new memory allocation.
		The rendered string will be a string of instructions useing ANSI codes for the 
		console.
	*/
	inline std::string render() const {
		std::string result;
		this->render(result);
		return result;
	}
	
	/**
		The object renders its content into existing memory.
		The rendered string will be a string of instructions useing ANSI codes for the 
		console.		
	*/
	void render(std::string& output_string) const;
};	
	
}

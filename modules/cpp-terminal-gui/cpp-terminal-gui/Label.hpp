
#pragma once

#include "RenderTrait.hpp"

// C++ STD
#include <string>
#include <string_view>
#include <cinttypes>
#include <size_t>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>
#include <cpp-terminal/platform/conversion.hpp>

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
private:

	struct ColorChangeIndex{
		Term::Color color; 	// the color that it is going to change to
		int index;			// the index at which it is going to change, 
							// the change will be applied before the character of that index will be printed.
	}

	std::u32string _string; 
	std::vector<ColorChangeIndex> _foregroundColour;
	std::vector<ColorChangeIndex> _backgroundColour;

	Term::Cursor _position; 
	
	//the label will only be printed up to the given length. If the length is negative then the whole label will be printed regardles.
	int _label_length = -1; 
	
public:
			
	Label(const std::string& string, Term::Cursor position = {0, 0}, int label_length = -1) : 
			_string(Term::Private::utf8_to_utf32(string)), 
			_position(position),
			_label_length(label_length){}
			
	Label(std::string_view string, Term::Cursor position = {0, 0}, int label_length = -1) : 
			_string(Term::Private::utf8_to_utf32(string)), 
			_position(position),
			_label_length(label_length){}
			
	Label(const std::u32string& string, Term::Cursor position = {0, 0}, int label_length = -1) : 
			_string(string), 
			_position(position),
			_label_length(label_length){}
			
	Label(std::u32string&& string, Term::Cursor position = {0, 0}, int label_length = -1) : 
			_string(std::move(string)), 
			_position(position),
			_label_length(label_length){}
	
	/*---- append ----*/
	inline void append(const std::string& string){
		Term::Private::append_utf8_to_utf32(this->_string, string);
	}
	
	inline void append(const std::string_view& string){
		Term::Private::append_utf8_to_utf32(this->_string, string);
	}
	
	inline void append(const char* string, std::size_t size){
		Term::Private::append_utf8_to_utf32(this->_string, string, size);
	}
	
	inline void append(const std::u32string& string){
		this->_string.append(string);
	}
	
	/*---- assign ----*/
	inline void assign(const std::string& string){
		this->_string.clear();
		this->append(string);
	}
	
	inline void assign(const std::string_view& string){
		this->_string.clear();
		this->append(string);
	}
	
	inline void assign(const char* string, std::size_t size){
		this->_string.clear();
		this->append(string);
	}
	
	inline void assign(const std::u32string& string){
		this->_string = string;
	}
	
	inline void assign(std::u32string&& string){
		this->_string = std::move(string);
	}
	
	/**/
	
	inline void set_position(Term::Cursor position){this->_position = position;};
	
	/**
		The object renders its whole content into a new std::string object.
		This may cause new memory allocation.
		The rendered string will be a string of instructions useing ANSI codes for the 
		console.
	*/
	inline std::string render() const {std::string result;this->render(result);return result;}
	
	/**
		The object renders its content into existing memory.
		The rendered string will be a string of instructions useing ANSI codes for the 
		console.		
	*/
	void render(std::string& output_string) const;
};	
	
}

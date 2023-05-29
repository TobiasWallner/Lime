#pragma once

// C++ std
#include <cstddef>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

// utf8 string 
#include <utf8_char.hpp>
#include <utf8_string.hpp>

#include "ColorString.hpp"

namespace TermGui{
	
class TextEditor{
private:
	using Line = ColorString;		// will be managed by this class in a way so that there are no linebraks in a line
	using Text = std::list<Line>;	// list of lines

public:

	using size_type = std::size_t;
	using iterator = Text::iterator;
	using const_iterator = Text::const_iterator;
	using reverse_iterator = Text::reverse_iterator;
	using const_reverse_iterator = Text::const_reverse_iterator;
	using reference = Text::const_reference;
	using const_reference = Text::const_reference;
	using value_type = Text::value_type;
	using pointer = Text::pointer;
	using const_pointer = Text::const_pointer;
	using allocator_type = Text::allocator_type;
	
	struct Cursor{
		size_type lineNumber;
		size_type columnNumber;
		Text::iterator lineIterator;
	};


private:
	
	Text _text; // stores the text data
	Cursor _cursor;

public:

	/// constructs a text with one empty line + sets the cursor position and iterators
	TextEditor();
	
	iterator begin(){return this->_text.begin();}
	const_iterator begin() const {return this->_text.cbegin();}
	const_iterator cbegin() const {return this->_text.cbegin();}
	
	iterator lineItr(){return this->_cursor.lineIterator;}
	const_iterator lineItr() const {return this->_cursor.lineIterator;}
	const_iterator clineItr() const {return this->_cursor.lineIterator;}
	
	iterator end(){return this->_text.end();}
	const_iterator end() const {return this->_text.cend();}
	const_iterator cend() const {return this->_text.cend();}
	
	iterator last(){return this->rbegin().base();}
	const_iterator last() const {return this->crbegin().base();}
	const_iterator clast() const {return this->crbegin().base();}
	
	reverse_iterator rbegin(){return this->_text.rbegin();}
	const_reverse_iterator rbegin() const {return this->_text.crbegin();}
	const_reverse_iterator crbegin() const {return this->_text.crbegin();}
	
	reverse_iterator rend(){return this->_text.rend();}
	const_reverse_iterator rend() const {return this->_text.crend();}
	const_reverse_iterator crend() const {return this->_text.crend();}
	
	/// inserts a character at the current cursor position into the string
	void insert(utf8::Char c);
	
	/// converts the char into a utf8::Char and inserts it at the current cursor position and advances the cursor
	inline void insert(char c){this->insert(utf8::Char(c));}
	
	/// inserts the null terminated string into the stream
	/// returns true is the string could be read as utf8, returns false otherwise
	bool insert(const char*);
	
	
	/// returns the line position of the cursor
	inline size_type get_line() const {return this->_cursor.lineNumber;}
	
	/// returns the column position of the cursor in the line
	inline size_type get_column() const {return this->_cursor.columnNumber;}
	
	
	/// moves the cursor to the rigth, aka. advances the column by one.
	/// if at the end of line perform a jump to the next line, if it exists
	void move_right();

	inline bool is_start_of_line() const {return this->get_column() == 0;}

	inline bool is_end_of_line() const {return this->get_column() == this->lineItr()->size();}
	
	inline bool is_last_line() const {return this->lineItr() == this->last();}

	inline bool is_end_of_file() const { return this->is_last_line() && this->is_end_of_line(); }
	
	
private:

	/// returns an iterator to the line at the given absolute position or the last line of the file
	iterator find_line(size_type line);
	const_iterator find_line(size_type line) const;
	
	

};
	
}
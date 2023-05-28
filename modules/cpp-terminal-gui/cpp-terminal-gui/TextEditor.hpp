#pragma once

// C++ std
#include <cstddef>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

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
	
	/// returns the line position of the cursor
	inline size_type get_line() const {return this->_cursor.lineNumber;}
	
	/// returns the column position of the cursor in the line
	inline size_type get_column() const {return this->_cursor.columnNumber;}
	
	
	
	
	
private:

	/// returns an iterator to the line at the given absolute position or the last line of the file
	iterator find_line(size_type line);
	const_iterator find_line(size_type line) const;
	
	

};
	
}
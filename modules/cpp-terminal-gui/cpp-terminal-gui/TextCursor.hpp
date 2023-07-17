#pragma once

// c++ std
#include <list>

// project

// project
#include "FontStyle.hpp"
#include "ColorString.hpp"

namespace TermGui{
	
class TextEditor;

class TextCursor{
	
public:
	using Line = ColorString;		// will be managed by this class in a way so that there are no linebraks in a line
	using Text = std::list<Line>;	// list of lines
	
	const TextEditor* pEditor;
	Text::const_iterator lineIterator;
	std::int32_t lineIndex;
	std::int32_t columnIndex;
	std::int32_t screenColumn;
public:
	
	TextCursor(const TextEditor* pEditor,
				Text::const_iterator lineIterator,
				std::int32_t lineIndex,
				std::int32_t columnIndex = 0,
				std::int32_t screenColumn = 0);
	
	explicit TextCursor(const TextEditor* pEditor);
	
	
	TextCursor(const TextCursor& other) = default;
	
	TextCursor& operator=(const TextCursor& other) = default;

	inline Text::const_iterator line_iterator() const {return this->lineIterator;}
	inline std::int32_t line_index() const {return this->lineIndex;}
	inline std::int32_t column_index() const {return this->columnIndex;}
	inline std::int32_t screen_column() const {return this->screenColumn;}
	
	inline bool is_start_of_line() const {return this->columnIndex == 0;}
	inline bool is_end_of_line() const {return this->columnIndex == lineIterator->size();}
	bool is_first_line() const;
	bool is_last_line() const;
	bool is_start_of_file() const;
	bool is_end_of_file() const;

	void move_forward();
	void move_back();
	
	inline void move_up_to_start_of_line(){--this->lineIterator; --this->lineIndex; this->columnIndex = 0; this->screenColumn = 0;}
	inline void move_down_to_start_of_line(){++this->lineIterator; ++this->lineIndex; this->columnIndex = 0; this->screenColumn = 0;}
	
	void move_up(std::int32_t n = 1);
	void move_down(std::int32_t n = 1);
	
	inline void move_to_start_of_line(){this->lineIndex = 0; this->columnIndex = 0; this->screenColumn = 0;}
	void move_to_end_of_line();
	void move_to_start_of_file();
	void move_to_end_of_file();
	
	void move_to_screen_column_before(std::int32_t wantedScreenColumn);
	void move_to_screen_column_after(std::int32_t wantedScreenColumn);
	
	inline utf8::Char get_char(){return lineIterator->empty() ? utf8::Char('\0') : this->lineIterator->at(this->columnIndex);}
	
	void update_tab_size();

	friend inline bool operator==(const TextCursor& l, const TextCursor& r){
		return l.lineIterator == r.lineIterator && l.columnIndex == r.columnIndex;
	}
	friend inline bool operator!=(const TextCursor& l, const TextCursor& r){return !(l == r);}

	friend inline bool operator<(const TextCursor& l, const TextCursor& r){
		return l.lineIndex < r.lineIndex || (l.lineIndex == r.lineIndex && l.columnIndex < r.columnIndex);
	}
	friend inline bool operator<=(const TextCursor& l, const TextCursor& r){return !(l > r);}
	friend inline bool operator>(const TextCursor& l, const TextCursor& r){return (r < l);}
	friend inline bool operator>=(const TextCursor& l, const TextCursor& r){return !(l < r);}
};

}
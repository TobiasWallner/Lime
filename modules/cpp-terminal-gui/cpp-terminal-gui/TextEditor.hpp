#pragma once

// C++ std
#include <cstddef>
#include <filesystem>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

// utf8 string 
#include <utf8_char.hpp>
#include <utf8_string.hpp>

#include "ColorString.hpp"
#include "RenderTrait.hpp"
#include "EditTrait.hpp"
#include "TextCursor.hpp"

namespace TermGui{
	
	
class TextEditor; // forward declaration
bool operator==(const TermGui::TextEditor& lhs, const TermGui::TextEditor& rhs);
	
class TextEditor : public RenderTrait, public EditTrait{
private:
	using Line = ColorString;		// will be managed by this class in a way so that there are no linebraks in a line
	using Text = std::list<Line>;	// list of lines

public:

	using size_type = Text::size_type;
	using iterator = Text::iterator;
	using const_iterator = Text::const_iterator;
	using reference = Text::reference;
	using const_reference = Text::const_reference;
	using value_type = Text::value_type;
	using pointer = Text::pointer;
	using const_pointer = Text::const_pointer;

private:
	
	Text _text; // stores the text data
	TextCursor _cursor;
	
	std::filesystem::path _filename;
	
	ScreenPosition screenPosition;
	ScreenWidth screenWidth;
	
	size_type renderLineStart;
	const_iterator renderLineStartItr;
	size_type renderColumnStart;
	
	TextCursor topScreenLine;		// is placed at the first column of the first line that is visible on the screen
	TextCursor topMarginLine;	// is placed at the first column of the upper line margin. If an insertion cursor is above this line the screen will get moved up
	TextCursor bottomMarginLine; // is placed at the end of the bottom margin line. If an insertion cursor is below that the screen will get moved down
	TextCursor bottomScreenLine;		// is placed at the end of the last visible line on the screen.
	std::int32_t screenColumn = 0;	// marks the screen column at which characters on the screen will be displayed
	
	inline static constexpr std::int32_t margin = 4;
	
	std::int32_t tabSize = 4;
	
	//TODO: bool lineWrapping = false;
	bool showCursor = false;
	
	bool saved = true;

public:

	TextEditor(std::filesystem::path filename = "", ScreenPosition screenPosition=ScreenPosition{0,0}, ScreenWidth screenWidth=ScreenWidth{0,0});
	
	inline std::int32_t tab_size() const {return this->tabSize;}
	void tab_size(std::int32_t newTabSize);
	
	inline bool show_cursor() const {return this->showCursor;}
	void show_cursor(bool logic) override;
	
	/// returns true if there are no lines in the text or if there is one and it is empty
	bool empty() const;
	void clear() override;
	
	inline iterator un_const(const_iterator constIterator) {return this->_text.erase(constIterator, constIterator);}
	
	inline iterator begin(){return this->_text.begin();}
	inline const_iterator begin() const {return this->_text.cbegin();}
	inline const_iterator cbegin() const {return this->_text.cbegin();}
	
	inline iterator end(){return this->_text.end();}
	inline const_iterator end() const {return this->_text.cend();}
	inline const_iterator cend() const {return this->_text.cend();}
	
	inline reference front(){return this->_text.front();}
	inline const_reference front() const {return this->_text.front();}
	
	inline reference back(){return this->_text.back();}
	inline const_reference back() const {return this->_text.back();}
	
	inline bool is_screen_at_top() const {return this->topScreenLine.line_iterator() == this->_text.begin();}
	inline bool is_screen_at_bottom() const {return this->topScreenLine.line_iterator() == --this->_text.end();}
	inline bool is_above_margin(const TextCursor& cursor) const {return cursor.line_index() > this->topMarginLine.line_index();}
	inline bool is_below_margin(const TextCursor& cursor) const {return cursor.line_index() < this->bottomMarginLine.line_index();}
	
	inline ScreenWidth::size_type text_width() const {return this->screenWidth.x;}
	inline ScreenWidth::size_type text_height() const {return this->screenWidth.y - 2;}
	inline ScreenWidth::size_type header_height() const {return 1;}
	inline ScreenWidth::size_type footer_height() const {return 1;}
	
	inline ScreenPosition header_position() const {return this->screenPosition;}
	inline ScreenPosition text_position() const {
		const ScreenPosition::size_type x = this->screenPosition.x;
		const ScreenPosition::size_type y = this->screenPosition.y + this->header_height();
		return ScreenPosition{.x = x, .y = y};
	}
	inline ScreenPosition footer_position() const {
		const ScreenPosition::size_type x = this->screenPosition.x;
		const ScreenPosition::size_type y = this->screenPosition.y + this->header_height() + this->text_height();
		return ScreenPosition{.x = x, .y = y};
	}
	
	/// moves the screen down by one screen line
	void move_screen_up();
	
	/// moves the screen up by one screen line
	void move_screen_down();
	
	/// moves the screen to the right by one column
	void move_screen_right(std::int32_t n = 1);
	
	/// moves the screen to the left by one column
	void move_screen_left(std::int32_t n = 1);
	
	/// moves the screen to the cursor by iterating over lines
	/// reccomended usage: when the cursor is close to the screen - line distance is smaller than the screen visible screen lines
	void move_screen_to_cursor(const TextCursor& cursor);
	void move_screen_lines_to_cursor(const TextCursor& cursor);
	void move_screen_column_to_cursor(const TextCursor& cursor);
	
	/// jumps the screen to the cursor by placing all screen edges and margins at the cursor position and then itteratively increasing the screen size.
	/// reccomended usage: when the cursor is far away from the screen - line distance is greater that then the visible screen lines
	//void jump_screen_to_cursor(const TextCursor& cursor);
	
	/// returns the number of all lines in the file. corresponds to the number of line breaks + 1
	inline size_type size() const {return this->_text.size();}
	
	/// inserts a character at the current cursor position into the string
	void insert(utf8::Char c) override;
	inline void insert(char c){return this->insert(utf8::Char(c));}
	inline bool insert(const char* str){return this->EditTrait::insert(str);}
	inline bool insert(const char* str, size_t size){return this->EditTrait::insert(str, size);}
	
	/// inserts a new line after the current cursor position
	inline void insert_line_after(){
		auto itr = this->_cursor.line_iterator();
		++itr;
		this->_text.insert(itr, Line());
	}
	
	/// inserts a new line after the current one and moves the cursor to the beginning
	/// of the inserted line. copies the remaining string to the beginning of the new line
	void insert_new_line();
	
	/// expects that the character is a number, letter, bracket, operator, or the like.
	/// character cannot be a special character like a line break or a backspace
	/// simply inserts the character at the current cursor position without any prozessing.
	void insert_naive(utf8::Char character);
	
	/// inserts a new line before the current cursor position
	inline void insert_line_before(){this->_text.insert(this->_cursor.line_iterator(), Line());}
	
	/// moves the cursor to the rigth, aka. advances the column by one.
	/// if at the end of line perform a jump to the next line, if it exists
	void move_forward() override;
	
	/// moves the cursor to the left, aka. decreases the column by one.
	/// if the cursot is at the beginning of the file -> moves the cursor to the end of the previous line
	void move_back() override;
	
	/// moves the cursor up one line, aka. decreases the line number and iterator
	/// if at the beginning of the file-> does nothing
	/// if at the first line -> moves to the beginning of the file
	/// if the column is larger than the line size of the line above -> places cursor at the end of the line
	void move_up() override;
	
	/// moves the cursor down a line, aka. advances the line number and line iterator by one
	/// if the cursor is already at the last line -> moves the cursor to the end of the line / end of file
	/// if the column is greater than the column of the next line -> moves the cursor to the end of that line
	void move_down() override;
	
	/// moves the cursor to the start of the line
	void move_to_start_of_line() override;
	
	void move_to_start_of_file() override;
	
	/// moves the cursor to the end of the line;
	void move_to_end_of_line() override;
	
	void move_to_end_of_file() override;
	
	/// renders the content of the editor that is visible into an ANSII string format that can be printed onto the screen.
	/// does not expect a clear screen
	void render(std::string& outputString) const override;
	
	bool save() const;
	bool save_as(std::filesystem::path newFilename);
	bool open(std::filesystem::path filename);
	
	friend bool operator==(const TermGui::TextEditor& lhs, const TermGui::TextEditor& rhs);
	friend inline bool operator!=(const TextEditor& lhs, const TextEditor& rhs){return !(lhs == rhs);}
	
	/// erases the character at the cursor position
	void erase() override;
	
	/// deletes the character befor the cursor position
	void Delete() override;
	
	/// inserts a new line
	void enter() override;
	
	/// sets the position of the object on the screen
	void set_screen_position(ScreenPosition position) override;
	
	/// get the position of the object on the screen
	ScreenPosition get_screen_position() const override;
	
	/// sets the width of the object on the screen
	void set_screen_width(ScreenWidth width) override;
	
	/// get the render width of the object
	ScreenWidth get_screen_width() const override;

};

}
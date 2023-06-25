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

namespace TermGui{
	
	
class TextEditor; // forward declaration
bool operator==(const TermGui::TextEditor& lhs, const TermGui::TextEditor& rhs);
	
class TextEditor : public RenderTrait, public EditTrait{
private:
	using Line = ColorString;		// will be managed by this class in a way so that there are no linebraks in a line
	using Text = std::list<Line>;	// list of lines

public:

	using size_type = Line::size_type;
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
		long lineNumber;
		long columnNumber;
		Text::iterator lineIterator;
	};


private:
	
	Text _text; // stores the text data
	Cursor _cursor;
	
	ScreenPosition screenPosition;
	ScreenWidth screenWidth;
	
	size_type renderLineStart;
	const_iterator renderLineStartItr;
	size_type renderColumnStart;
	
	static constexpr utf8::Char lineWrapSymbol = "→";
	
	//TODO: bool lineWrapping = false;
	bool showCursor = false;

public:

	inline TextEditor(){this->init({0,0}, {0,0});}
	/// constructs a text with one empty line + sets the cursor position and iterators
	inline TextEditor(ScreenPosition screenPosition, ScreenWidth screenWidth){this->init(screenPosition, screenWidth);}
	
	/// returns true if there are no lines in the text or if there is one and it is empty
	bool empty() const;

	void init(ScreenPosition screenPosition, ScreenWidth screenWidth);
	
	inline void clear() override { this->_text.clear(); this->init(this->screenPosition, this->screenWidth);}
	
	iterator begin(){return this->_text.begin();}
	const_iterator begin() const {return this->_text.cbegin();}
	const_iterator cbegin() const {return this->_text.cbegin();}
	
	iterator lineItr(){return this->_cursor.lineIterator;}
	const_iterator lineItr() const {return this->_cursor.lineIterator;}
	const_iterator clineItr() const {return this->_cursor.lineIterator;}
	
	iterator end(){return this->_text.end();}
	const_iterator end() const {return this->_text.cend();}
	const_iterator cend() const {return this->_text.cend();}
	
	iterator last(){return --this->end();}
	const_iterator last() const {return --this->cend();}
	const_iterator clast() const {return --this->cend();}
	
	reverse_iterator rbegin(){return this->_text.rbegin();}
	const_reverse_iterator rbegin() const {return this->_text.crbegin();}
	const_reverse_iterator crbegin() const {return this->_text.crbegin();}
	
	reverse_iterator rend(){return this->_text.rend();}
	const_reverse_iterator rend() const {return this->_text.crend();}
	const_reverse_iterator crend() const {return this->_text.crend();}
	
	inline reference front(){return this->_text.front();}
	inline const_reference front() const {return this->_text.front();}
	inline const_reference cfront() const {return this->_text.front();}
	
	inline reference back(){return this->_text.back();}
	inline const_reference back() const {return this->_text.back();}
	inline const_reference cback() const {return this->_text.back();}
	
	/// returns the number of all lines in the file. corresponds to the number of line breaks + 1
	inline size_type number_of_lines() const {return this->_text.size();}
	
	/// returns the size of the current line
	inline size_type line_size() const {return this->lineItr()->size();}
	
	inline size_type cursor_line() const {return this->_cursor.lineNumber;}
	inline size_type cursor_column() const {return this->_cursor.columnNumber;}
	
	/// inserts a character at the current cursor position into the string
	void insert(utf8::Char c) override;
	inline void insert(char c){return this->insert(utf8::Char(c));}
	inline bool insert(const char* str){return this->EditTrait::insert(str);}
	inline bool insert(const char* str, size_t size){return this->EditTrait::insert(str, size);}
	
	/// inserts a new line after the current cursor position
	inline void insert_line_after(){
		auto itr = this->lineItr();
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
	
	inline void show_cursor(bool on_off) override { this->showCursor = on_off; }
	
	/// inserts a new line before the current cursor position
	inline void insert_line_before(){this->_text.insert(this->lineItr(), Line());}
	
	
	/// returns the line position of the cursor
	inline size_type line_number() const {return this->_cursor.lineNumber;}
	
	/// returns the column position of the cursor in the line
	inline size_type column_number() const {return this->_cursor.columnNumber;}
	
	
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
	
	/// returns true if the cursor is at the start of the current line and false otherwise
	inline bool is_start_of_line() const {return this->column_number() == 0;}
	
	/// returns true if the cusor is located somewhere in the first line and false otherwise
	inline bool is_first_line() const {return this->line_number() == 0;}
	
	/// returs true if the cursor is located at the very start of the file before the first caracter
	inline bool is_start_of_file() const {return this->is_first_line() && this->is_start_of_line();}

	/// returns true if the cursor is located at the very end of the current line
	inline bool is_end_of_line() const {return this->column_number() == this->lineItr()->size();}
	
	/// return true if the cursor is located somewhere in the last line
	inline bool is_last_line() const { return this->lineItr() == this->last(); }
	
	/// returns true if the cursor is located at the very end of the last line 
	inline bool is_end_of_file() const { return this->is_last_line() && this->is_end_of_line(); }
	
	/// renders the content of the editor that is visible into an ANSII string format that can be printed onto the screen.
	void render(std::string& outputString) const override;
	
	/// reads the content of the file into the current cursor position
	bool append_file(std::ifstream& stream);
	bool append_file(const std::filesystem::path& path);

	/// reads the content of a file into the Editor
	bool read_file(std::ifstream& file);
	bool read_file(const std::filesystem::path& path);
	
	
	/// writes the content of the Editor into a file
	bool write_file(const std::filesystem::path& path);
	bool write_file(std::ofstream& stream);
	
	template<class Stream>
	friend Stream& operator << (Stream& stream, TextEditor& self){
		self.write_file(stream);
		return stream;
	}
	
	template<class Stream>
	friend Stream& operator >> (Stream& stream, TextEditor& self){
		self.append_file(stream);
		return stream;
	}
	
	friend bool TermGui::operator==(const TermGui::TextEditor& lhs, const TermGui::TextEditor& rhs);
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
private:

	/// returns an iterator to the line at the given absolute position or the last line of the file
	iterator find_line(size_type line);
	const_iterator find_line(size_type line) const;
	
	inline size_type line_width() const {return static_cast<size_type>(this->screenWidth.x);}
	inline size_type line_height() const {return static_cast<size_type>(this->screenWidth.y);}
	
	void scrowl_forward();
	void scrowl_back();
	void scrowl_up();
	void scrowl_down();

};

}
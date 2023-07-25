#pragma once

//utf8 string
#include <utf8_string.hpp>

// c++ std
#include <algorithm>
#include <list>

//project
#include "RenderTrait.hpp"
#include "EditTrait.hpp"

#include <cpp-terminal/cursor.hpp>

/*
	TODO: save command history
*/

namespace TermGui{

/// offers an editable string with a callback function that gets executed on enter.
/// On construction offer a pointer to the object that should be notified and a 
/// will move the command string into the provided method
class CommandLine : public RenderTrait, public EditTrait{
public:
	using string_type = utf8::string;
	using value_type = string_type::value_type;
	using size_type = string_type::size_type;
	using object_pointer = void*;
	using method_type = void(*)(object_pointer self, utf8::string_view);
	
private:
	string_type commandString;
	object_pointer objectPtr; // pointer cannot be changed after construction
	method_type method; // method cannot be changed after construction
	
	ScreenPosition position;
	ScreenWidth width;
	
	std::int32_t cursorIndex = 0;
	std::int32_t cursorColumn = 0;
	std::int32_t screenColumn = 0;
	std::int32_t margin = 4;
	bool showCursor = false;
	
	std::list<string_type> commandHistory;
	std::list<string_type>::const_iterator historyItr = commandHistory.cend();
	
	std::string startSymbol = ":";
	
public:
	
	CommandLine(object_pointer objectPtr, method_type method, ScreenPosition position = {0,0}, ScreenWidth width = {0,0});
		
	
	CommandLine(const CommandLine&) = default;
	CommandLine(CommandLine&&) = default;
	
	CommandLine& operator=(const CommandLine&) = default;
	CommandLine& operator=(CommandLine&&) = default;
	
	inline void start_symbol(std::string_view startSymbol) {this->startSymbol = startSymbol;}
	
	void show_cursor(bool on_off) override;
	
	void clear() override;
	
	void insert(utf8::Char c) override;
	inline bool insert(const char* str){return this->EditTrait::insert(str);}
	inline bool insert(const char* str, size_t size){return this->EditTrait::insert(str, size);}
	
	/// calls the provided method, passes the command string and clears the command line
	void enter() override;
	void erase() override;
	void Delete() override;
	
	void move_screen_to_cursor();
	
	/// moves the cursor to the rigth, aka. advances the column by one.
	/// if at the end of line perform a jump to the next line, if it exists
	void move_forward() override;
	
	/// moves the cursor to the left, aka. decreases the column by one.
	/// if the cursot is at the beginning of the file -> moves the cursor to the end of the previous line
	void move_back() override;

	/// loads the previous command into the command line
	void move_up() override;
	
	/// loads the earlier command into the command line
	void move_down() override;
	
	/// moves the cursor to the start of the line
	void move_to_start_of_line() override;
	
	/// moves the cursor the the start of the line
	void move_to_start_of_file() override;
	
	/// moves the cursor to the end of the line;
	void move_to_end_of_line() override;
	
	void move_to_end_of_file() override;
	
	/// returns true if the cursor is at the start of the current line and false otherwise
	bool is_start_of_line() const;

	/// returns true if the cursor is located at the very end of the current line
	bool is_end_of_line() const;
	
	/// sets the position of the object on the screen
	void set_screen_position(ScreenPosition position) override;
	
	/// get the position of the object on the screen
	ScreenPosition get_screen_position() const override;
	
	/// sets the width of the object on the screen
	void set_screen_width(ScreenWidth width) override;
	
	/// get the render width of the object
	ScreenWidth get_screen_width() const override;
	
	void render(std::string& outputString) const override;

	inline size_type text_width() const {return (this->width.x - this->startSymbol.size());}

};

}
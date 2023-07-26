#pragma once

//utf8 string
#include <utf8_string.hpp>

// c++ std
#include <algorithm>
#include <list>
#include <vector>

//project
#include "RenderTrait.hpp"
#include "EditTrait.hpp"
#include "ColorString.hpp"

#include <cpp-terminal/cursor.hpp>

/*
	TODO: save command history
*/

namespace TermGui{

struct Command;

/// sorted list of commands
class CommandList{
	using list_type = std::vector<Command>;
	list_type commandList;
	
public:
	
	using value_type = list_type::value_type;
	using allocator_type = list_type::allocator_type;
	using size_type = list_type::size_type;
	using difference_type = list_type::difference_type;
	using reference = list_type::reference;
	using const_reference = list_type::const_reference;
	using pointer = list_type::pointer;
	using const_pointer = list_type::const_pointer;
	using iterator = list_type::iterator;
	using const_iterator = list_type::const_iterator;
	
	struct range{iterator first; iterator last;};
	struct const_range{const_iterator first; const_iterator last;};
	
	inline iterator begin() {return this->commandList.begin();}
	inline const_iterator begin() const {return this->commandList.begin();}
	inline const_iterator cbegin() const {return this->commandList.cbegin();}
	
	inline iterator end() {return this->commandList.end();}
	inline const_iterator end() const {return this->commandList.end();}
	inline const_iterator cend() const {return this->commandList.cend();}
	
	bool empty() const;
	size_type size() const;
	
	void reserve(size_type new_cap);
	
	void insert(const Command& command);
	void insert(Command&& command);
	
	const_range find(const utf8::string_view& command) const;
	const_range find(const std::vector<utf8::string_view>& commands) const;
	
};

/// command element
struct Command{
	using object_pointer = void*;
	using callback_type = void(*)(object_pointer self, const std::vector<utf8::string_view>& command);
	
	struct Flag{
		utf8::string name;
		ColorString info;
	};
	
	utf8::string name;
	ColorString info;
	std::vector<Flag> Flags;
	CommandList subCommands;
	
	object_pointer objectPtr;
	callback_type callbackFn;
	
	inline void call(const std::vector<utf8::string_view>& command) const { 
		if(callbackFn != nullptr) callbackFn(objectPtr, command); 
	}
};


bool less(const Command &a, const utf8::string_view &b);
bool greater(const utf8::string_view &a, const Command &b);

/// offers an editable string with a callback function that gets executed on enter.
/// On construction offer a pointer to the object that should be notified and a 
/// will move the command string into the provided enterCallback
class CommandLine : public RenderTrait, public EditTrait{
public:
	using string_type = utf8::string;
	using value_type = string_type::value_type;
	using size_type = string_type::size_type;
	using object_pointer = void*;
	using callback_type = void(*)(object_pointer self, const std::vector<utf8::string_view>& commands);

private:
	string_type commandString;
	object_pointer objectPtr; // pointer cannot be changed after construction
	callback_type enterCallback; // enterCallback cannot be changed after construction
	
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
	
	CommandLine(object_pointer objectPtr, callback_type enterCallback, ScreenPosition position = {0,0}, ScreenWidth width = {0,0});
		
	
	CommandLine(const CommandLine&) = default;
	CommandLine(CommandLine&&) = default;
	
	CommandLine& operator=(const CommandLine&) = default;
	CommandLine& operator=(CommandLine&&) = default;
	
	inline void start_symbol(std::string_view startSymbol) {this->startSymbol = startSymbol;}
	
	void show_cursor(bool on_off) override;
	
	void clear() override;
	
	void naive_insert(utf8::Char c);
	void insert(utf8::Char c) override;
	
	/// calls the provided enterCallback, passes the command string and clears the command line
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
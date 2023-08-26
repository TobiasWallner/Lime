#pragma once

//utf8 string
#include <utf8_string.hpp>

// c++ std
#include <algorithm>
#include <list>
#include <vector>

//project
#include "GridCell.hpp"
#include "EditTrait.hpp"
#include "ColorString.hpp"

#include <cpp-terminal/cursor.hpp>

/*
	TODO: save command history
*/

namespace TermGui{

/// command element
struct Command{
	using object_pointer = void*;
	using callback_type = void(*)(object_pointer self, const std::vector<utf8::string_view>& command);
	struct Flag{ const utf8::string_view name; const utf8::string_view info; };
	struct ConstFlagView(){ const Flag* begin; size_t count; };
	
	const utf8::string_view name;
	const utf8::string_view info;
	const FlagView flags;
	object_pointer objectPtr;
	callback_type callbackFn;
	
	inline void call(const std::vector<utf8::string_view>& command) const { 
		if(callbackFn != nullptr) callbackFn(objectPtr, command); 
	}
};

struct const_command_range { const Command* first, const Command* last};

/// searches a range of commands that starts with the provided command name in the sorted range of commands
const_command_range find(const utf8::string_view& commandName, const_command_range sortedCommandRange);

/// offers an editable string with a callback function that gets executed on enter.
/// On construction offer a pointer to the object that should be notified and a 
/// will move the command string into the provided method
class CommandLine : public GridCell, public EditTrait{
public:
	using string_type = utf8::string;
	using value_type = string_type::value_type;
	using size_type = string_type::size_type;
	using object_pointer = Command::object_pointer;
	using callback_type = Command::callback_type;

private:
	const_command_range commands;
	ScreenPosition position;
	ScreenWidth width;
	string_type commandString;
	GridTrait* grid = nullptr;

	const_range possibleCommandsForInfo = commands;
	string_type inputString;
	
	std::list<string_type> commandHistory;
	std::list<string_type>::const_iterator historyItr = commandHistory.cend();
	
	std::string startSymbol = ":";
	
	std::int32_t cursorIndex = 0;
	std::int32_t cursorColumn = 0;
	std::int32_t screenColumn = 0;
	std::int32_t margin = 4;
	
	bool showCursor = false;
	
public:
	CommandLine(CallbackObjectType* objectPtr, method_type method) : 
		GridCell(ScreenWidth{100, 1}),
		commandString(), 
		objectPtr(objectPtr),
		method(method)
		{}
	
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
	
	void render(std::string& outputString) const override;

	inline size_type text_width() const {return (this->width.x - this->startSymbol.size());}

};

}
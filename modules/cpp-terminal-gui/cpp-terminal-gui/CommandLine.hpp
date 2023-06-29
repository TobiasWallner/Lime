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
template<class CallbackObjectType>
class CommandLine : public RenderTrait, public EditTrait{
public:
	using string_type = utf8::string;
	using value_type = string_type::value_type;
	using size_type = string_type::size_type;
	using method_type = void(CallbackObjectType::*)(utf8::string_view);
	
private:
	string_type commandString;
	CallbackObjectType* objectPtr; // pointer cannot be changed after construction
	method_type method; // method cannot be changed after construction
	
	ScreenPosition position;
	ScreenWidth width;
	
	size_type cursorIndex = 0;
	size_type renderStart = 0;
	bool showCursor = false;
	
	std::list<string_type> commandHistory;
	std::list<string_type>::const_iterator historyItr = commandHistory.cend();
	
public:
	
	CommandLine(CallbackObjectType* objectPtr, method_type method, ScreenPosition position = {0,0}, ScreenWidth width = {0,0}) : 
		commandString(), 
		objectPtr(objectPtr),
		method(method),
		position(position),
		width(width)
		{}
		
	
	CommandLine(const CommandLine&) = default;
	CommandLine(CommandLine&&) = default;
	
	CommandLine& operator=(const CommandLine&) = default;
	CommandLine& operator=(CommandLine&&) = default;
	
	inline void show_cursor(bool on_off) override { this->showCursor = on_off; }
	
	inline void clear() override {
		this->move_to_start_of_line();
		this->commandString.clear();
		historyItr = commandHistory.cend();
	}
	
	inline void insert(utf8::Char c) override {
		this->historyItr = this->commandHistory.cend();
		if(c == '\n'){
			this->enter();
		}else if(c == '\b'){
			this->Delete();
		}else{
			this->commandString.insert(cursorIndex, c);
			this->move_forward();
		}
	}
	
	inline bool insert(const char* str){return this->EditTrait::insert(str);}
	inline bool insert(const char* str, size_t size){return this->EditTrait::insert(str, size);}
	
	/// calls the provided method, passes the command string and clears the command line
	inline void enter() override {
		(objectPtr->*(method))(this->commandString);
		if(this->commandHistory.empty()){
			this->commandHistory.push_front(std::move(this->commandString));	
		}else if(this->commandHistory.front() != this->commandString){
			this->commandHistory.push_front(std::move(this->commandString));	
		}
		this->clear();
	}
	
	inline void erase() override { 
		if(!this->is_end_of_line()){
			this->commandString.erase(this->cursorIndex); 
		}
	}
	
	inline void Delete() override {
		if(!this->is_start_of_line()){
			this->move_back();
			this->commandString.erase(this->cursorIndex);
		}
	}
	
private:
	
	inline void scrowl_forward(){
		const bool over_scrowl_protection = this->renderStart + this->line_width() < this->commandString.size();
		const bool cursor_near_end = this->cursorIndex + 3 > this->line_width() + this->renderStart;
		this->renderStart += over_scrowl_protection && cursor_near_end;
	}
	
	inline void scrowl_back(){
		const bool underscrowl_protection =  this->renderStart > 0;
		const bool cursor_near_start = this->renderStart + 3 > this->cursorIndex;
		this->renderStart -= underscrowl_protection && cursor_near_start;
	}
	
public:
	
	
	/// moves the cursor to the rigth, aka. advances the column by one.
	/// if at the end of line perform a jump to the next line, if it exists
	inline void move_forward() override { 
		this->cursorIndex += !this->is_end_of_line();
		this->scrowl_forward();
	}
	
	/// moves the cursor to the left, aka. decreases the column by one.
	/// if the cursot is at the beginning of the file -> moves the cursor to the end of the previous line
	inline void move_back() override { 
		this->cursorIndex -= !this->is_start_of_line();
		this->scrowl_back();
	}


	/// loads the previous command into the command line
	inline void move_up() override {
		// note that the cend() is the invalid state in the invalid state the command line will be empty
		if(!this->commandHistory.empty()){
			if(this->historyItr == this->commandHistory.cend()){
				this->historyItr = this->commandHistory.cbegin();
			}else{
				++this->historyItr;
				if(this->historyItr == this->commandHistory.cend()){
					--this->historyItr;
				}
			}
			if(this->historyItr != this->commandHistory.cend()){
				this->commandString = *(this->historyItr);
				this->move_to_end_of_line();
			}
		}
	};
	
	/// loads the earlier command into the command line
	inline void move_down() override {
		// note that the cend() is the invalid state in the invalid state the command line will be empty
		if(this->historyItr != this->commandHistory.cend()){
			if(this->historyItr == this->commandHistory.cbegin()){
				this->clear();
				this->historyItr = this->commandHistory.cend();
			}else{
				--this->historyItr;
			}
			if(this->historyItr != this->commandHistory.cend()){
				this->commandString = *(this->historyItr);
				this->move_to_end_of_line();
			}	
		}		
	};
	
	/// moves the cursor to the start of the line
	inline void move_to_start_of_line() override { this->cursorIndex = 0; }
	
	/// moves the cursor the the start of the line
	inline void move_to_start_of_file() override { this->move_to_start_of_line(); }
	
	/// moves the cursor to the end of the line;
	inline void move_to_end_of_line() override { this->cursorIndex = this->commandString.size(); }
	
	inline void move_to_end_of_file() override { this->move_to_end_of_line(); }
	
	/// returns true if the cursor is at the start of the current line and false otherwise
	inline bool is_start_of_line() const { return this->cursorIndex == 0; }

	/// returns true if the cursor is located at the very end of the current line
	inline bool is_end_of_line() const { return this->cursorIndex == this->commandString.size(); }
	
	/// sets the position of the object on the screen
	inline void set_screen_position(ScreenPosition position) override {this->position = position;}
	
	/// get the position of the object on the screen
	ScreenPosition get_screen_position() const override {return this->position;}
	
	/// sets the width of the object on the screen
	void set_screen_width(ScreenWidth width) override {this->width = width;}
	
	/// get the render width of the object
	ScreenWidth get_screen_width() const override{return this->width;}
	
	void render(std::string& outputString) const override {
		if(this->width.y > 0){
			outputString += Term::cursor_move(this->position.y, this->position.x);
			outputString += ": ";
			const auto end = std::min(this->line_width() + this->renderStart - this->is_end_of_line(), this->commandString.size());
			for(auto i = this->renderStart; i != end; ++i){ 
				if(this->showCursor && i == cursorIndex){
						outputString += to_string(FontStyle::Reversed::ON);
						outputString += this->commandString[i].to_std_string_view();
						outputString += to_string(FontStyle::Reversed::OFF);				
				}else{
					outputString += this->commandString[i].to_std_string_view();
				}
				
			}
			if (this->showCursor && this->is_end_of_line()) {
				outputString += to_string(FontStyle::Reversed::ON);
				outputString += ' ';
				outputString += to_string(FontStyle::Reversed::OFF);
			}
		}
	}

private:

	inline size_type line_width() const {
		return (this->width.x - (sizeof(": ")-1));
	}

};

}
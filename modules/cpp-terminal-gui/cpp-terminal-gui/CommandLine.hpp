#pragma once

//utf8 string
#include <utf8_string.hpp>

//project
#include "RenderTrait.hpp"
#include "EditTrait.hpp"

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
	using method_type = void(CallbackObjectType::*)(utf8::string&&);
	
private:
	string_type commandString;
	CallbackObjectType* objectPtr; // pointer cannot be changed after construction
	method_type method; // method cannot be changed after construction
	size_type cursorIndex = 0;
	
	bool showCursor = false;
	
public:

	/// constructs a command line with a pointer to and object
	/// that should be called with the provided method uppon pressing enter
	CommandLine(CallbackObjectType* objectPtr, method_type method) : 
		commandString(), 
		objectPtr(objectPtr),
		method(method),
		cursorIndex(0)
		{}
		
	CommandLine(const CommandLine&) = default;
	CommandLine(CommandLine&&) = default;
	
	CommandLine& operator=(const CommandLine&) = default;
	CommandLine& operator=(CommandLine&&) = default;
	
	inline void show_cursor(bool on_off) override { this->showCursor = on_off; }
	
	inline void clear() override {
		this->move_to_start_of_line();
		this->commandString.clear();
	}
	
	inline void insert(utf8::Char c) override {
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
		(objectPtr->*(method))(std::move(this->commandString));
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
	
	/// moves the cursor to the rigth, aka. advances the column by one.
	/// if at the end of line perform a jump to the next line, if it exists
	inline void move_forward() override { if(!this->is_end_of_line()){ ++this->cursorIndex; } }
	
	/// moves the cursor to the left, aka. decreases the column by one.
	/// if the cursot is at the beginning of the file -> moves the cursor to the end of the previous line
	inline void move_back() override { if(!this->is_start_of_line()){ --this->cursorIndex; } }

	/// moves the cursor to the beginning of the line
	inline void move_up() override { this->move_to_start_of_line(); };
	
	/// moves the cursor the the end of the line
	inline void move_down() override { this->move_to_end_of_line(); };
	
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
	
	
	void render(std::string& outputString) const override {
		outputString += ": ";
		
		if(this->showCursor){
			if(this->is_end_of_line()){
				this->commandString.append_to(outputString);
				outputString += to_string(FontStyle::Reversed::ON);
				outputString +=	' ';
				outputString +=	to_string(FontStyle::Reversed::OFF);
			}else{
				auto itr = this->commandString.cbegin();
				const auto cursorItr = this->commandString.cbegin() + this->cursorIndex;
				const auto endItr = this->commandString.cend();
				
				for(; itr != cursorItr; ++itr){
					outputString += itr->to_std_string_view();
				}
				
				outputString += to_string(FontStyle::Reversed::ON);
				outputString += itr->to_std_string_view();
				++itr;
				outputString += to_string(FontStyle::Reversed::OFF);
				
				for(; itr != endItr; ++itr){
					outputString += itr->to_std_string_view();
				}
			}
		}else{
			this->commandString.append_to(outputString);
		}
	}
	

};

}
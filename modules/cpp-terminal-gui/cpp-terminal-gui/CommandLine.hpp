#pragma once

#include <utf8_string.hpp>

#include "ColorString.hpp"
#include "RenderTrait.hpp"

namespace TermGui{

/// offers an editable string with a callback function that gets executed on enter.
/// On construction offer a pointer to the object that should be notified and a 
template<class CallbackObject, class CallbackMethod>
class CommandLine : public RenderTrait, public EditTrait{
public:
	using size_type = ColorString::size_type;
	
private:
	utf8::string commandString;
	CallbackObject* const objectPtr; // pointer cannot be changed after construction
	CallbackMethod const method; // method cannot be changed after construction
	size_type cursorIndex = 0;
	
	bool showCursor = false;
	
public:

	/// constructs a command line with a pointer to and object
	/// that should be called with the provided method uppon pressing enter
	CommandLine(CallbackObject* objectPtr, CallbackMethod method) : 
		commandString(), 
		objectPtr(objectPtr),
		method(method),
		cursorIndex(0)
		{}
	
	inline void show_cursor(bool on_off){ this->showCursor = on_off; }
	
	inline clear(){
		this->move_to_start_of_line();
		this->commandString.clear();
	}
	
	inline ColorString& insert(utf8::Char c){
		if(c != '\n'){
			this->commandString.insert(cursorIndex, c);
			this->move_forward()
		}else{
			objectPtr->method();
			this->clear();
		}
	}
	
	inline EditTrait& insert(char c) override {this->insert(utf8::Char(c)); return *this;}
	
	
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
	
	
	void render(std::string& outputString) const override;
	

};

}
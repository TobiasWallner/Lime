#pragma once

namespace TermGui{

/**
	This is an Abstract class that provides functions that are 
	useful for editing a piece of text.
	
	offers functions to move the cursor (aka. insert) positions
	
	offers functions to insert into the 
*/
class EditTrait{
	public:
	
	EditTrait(){};
	
	virtual ~EditTrait(){}
	
	// ----- edit functions -----
	
	virtual clear() = 0
	
	virtual EditTrait& insert(utf8::Char c) = 0;
	
	
	// ----- cursor functions -----
	
	virtual void show_cursor(bool on_off) = 0;
	
	virtual void move_forward() = 0;
	
	virtual void move_back() = 0;

	virtual void move_up() = 0;
	
	virtual void move_down() = 0;
	
	virtual void move_to_start_of_line() = 0;
	
	virtual void move_to_start_of_file() = 0;
	
	virtual void move_to_end_of_line() = 0;
	
	virtual void move_to_end_of_file() = 0;

};

}
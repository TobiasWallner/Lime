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
	
	virtual void clear() = 0;
	
	virtual void insert(utf8::Char c) = 0;
	
	inline void insert(char c){this->insert(utf8::Char(c));}
	
	inline bool insert(const char* str){
		while(*str != '\0'){
			utf8::Char c;
			const char* next = c.assign(str);
			if(next == str){
				// no characters have been read -> error
				return false;
			}
			this->insert(c);
			str = next;
		}
		return true;
	}

	inline bool insert(const char* str, size_t size){
		size_t pos = 0;
		while(pos < size){
			utf8::Char c;
			const char* next = c.assign(str, size - pos);
			pos += std::distance(str, next);
			if(next == str){
				// no characters have been read -> error
				return false;
			}
			this->insert(c);
			str = next;
		}
		return true;
	}
	
	virtual void enter() = 0;
	
	/// deletes the character at or after the cursor position
	virtual void erase() = 0;
	
	/// deletes the character before the cursor position
	virtual void Delete() = 0;
	
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
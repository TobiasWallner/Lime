
#include "CommandLine.hpp"
#include "FontStyle.hpp"

TermGui::CommandLine::CommandLine(object_pointer objectPtr, method_type method, ScreenPosition position, ScreenWidth width) : 
		commandString(), 
		objectPtr(objectPtr),
		method(method),
		position(position),
		width(width)
		{}

void TermGui::CommandLine::show_cursor(bool on_off) { this->showCursor = on_off; }

void TermGui::CommandLine::clear() {
	this->move_to_start_of_line();
	this->commandString.clear();
	historyItr = commandHistory.cend();
}

void TermGui::CommandLine::insert(utf8::Char c) {
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

void TermGui::CommandLine::enter() {
	method(objectPtr, this->commandString);
	if(this->commandHistory.empty()){
		this->commandHistory.push_front(std::move(this->commandString));	
	}else if(this->commandHistory.front() != this->commandString){
		this->commandHistory.push_front(std::move(this->commandString));	
	}
	this->clear();
}

void TermGui::CommandLine::erase() { 
	if(!this->is_end_of_line()){
		this->commandString.erase(this->cursorIndex); 
	}
}

void TermGui::CommandLine::Delete() {
	if(!this->is_start_of_line()){
		this->move_back();
		this->commandString.erase(this->cursorIndex);
	}
}

void TermGui::CommandLine::move_screen_to_cursor(){
	if(this->cursorColumn < this->screenColumn + this->margin){
		this->screenColumn = (this->cursorColumn < this->margin) ? 0 : this->cursorColumn - this->margin;
	}else if(this->cursorColumn > this->screenColumn + this->text_width() - this->margin){
		this->screenColumn = this->cursorColumn - this->text_width() + this->margin;
	}
}

void TermGui::CommandLine::move_forward() { 
	this->cursorIndex += !this->is_end_of_line();
	this->move_screen_to_cursor();
}

void TermGui::CommandLine::move_back() { 
	this->cursorIndex -= !this->is_start_of_line();
	this->move_screen_to_cursor();
}

/// loads the previous command into the command line
void TermGui::CommandLine::move_up() {
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
}

/// loads the earlier command into the command line
void TermGui::CommandLine::move_down() {
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
}

void TermGui::CommandLine::move_to_start_of_line() { 
	this->cursorIndex = 0; 
	this->screenColumn = 0;
}

void TermGui::CommandLine::move_to_start_of_file() { 
	this->move_to_start_of_line(); 
}

void TermGui::CommandLine::move_to_end_of_line() {
	this->cursorIndex = this->commandString.size(); 
	this->screenColumn = (this->commandString.size() + 3 > this->text_width()) ? this->commandString.size() - (this->text_width() - 3): 0;
}

void TermGui::CommandLine::move_to_end_of_file() { 
	this->move_to_end_of_line(); 
}

bool TermGui::CommandLine::is_start_of_line() const { 
	return this->cursorIndex == 0; 
}

bool TermGui::CommandLine::is_end_of_line() const { 
	return this->cursorIndex == this->commandString.size(); 
}

void TermGui::CommandLine::set_screen_position(TermGui::ScreenPosition position) {
	this->position = position;
}


TermGui::ScreenPosition TermGui::CommandLine::get_screen_position() const {
	return this->position;
}


void TermGui::CommandLine::set_screen_width(TermGui::ScreenWidth width) {
	this->width = width;
}


TermGui::ScreenWidth TermGui::CommandLine::get_screen_width() const {
	return this->width;
}

void TermGui::CommandLine::render(std::string& outputString) const {
	if(this->width.y > 0){
		outputString += Term::cursor_move(this->position.y, this->position.x);
		outputString += this->startSymbol;
		size_type column = this->screenColumn;
		const size_type columnEnd = this->commandString.size();
		size_type screenColumn = 0;
		const size_type screenColumnEnd = this->text_width();
		for(; column < columnEnd && screenColumn < (screenColumnEnd - this->is_end_of_line()); ++column, (void)++screenColumn){
			const size_type show_cursor = this->showCursor && column == this->cursorIndex;
			if(this->commandString[column] == '\t' && show_cursor){
				outputString += to_string(TermGui::FontStyle::Reversed::ON);
				outputString += ' ';
				outputString += to_string(TermGui::FontStyle::Reversed::OFF);
				const size_type tabs_to_print = std::min(3ULL, (screenColumnEnd - this->is_end_of_line()) - screenColumn);
				outputString.append(tabs_to_print, ' ');
				screenColumn += tabs_to_print;
			}else if(this->commandString[column] == '\t'){
				const size_type tabs_to_print = std::min(4ULL, (screenColumnEnd - this->is_end_of_line()) - screenColumn);
				outputString.append(tabs_to_print, ' ');
				screenColumn += tabs_to_print-1;
			}else if(show_cursor && column == this->cursorIndex){
				outputString += to_string(TermGui::FontStyle::Reversed::ON);
				outputString += this->commandString[column].to_std_string_view();
				outputString += to_string(TermGui::FontStyle::Reversed::OFF);				
			}else{
				outputString += this->commandString[column].to_std_string_view();
			}
		}
		
		if (this->showCursor && this->is_end_of_line()) {
			outputString += to_string(TermGui::FontStyle::Reversed::ON);
			outputString += ' ';
			outputString += to_string(TermGui::FontStyle::Reversed::OFF);
			++screenColumn;
		}
		outputString.append(screenColumnEnd - screenColumn, ' ');
	}
}
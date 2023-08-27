
// std c++
#include <algorithm>


// project
#include "CommandLine.hpp"
#include "FontStyle.hpp"

TermGui::CommandLine::CommandLine(const_command_range commands, string_type inintMessage) : 
		GridCell(ScreenWidth{100, 1}),
		commands(commands),
		possibleCommands(commands),
		inputString(),
		message(std::move(inintMessage))
		{}

/// parses a utf8 view of a command string into a vector of string views
/// returns a vector where each element holds one word or string ("..") of the command
static std::vector<utf8::string_view> parse_command_string(utf8::string_view inputString){
	std::vector<utf8::string_view> commandList;
	auto itr = inputString.cbegin();
	const auto end = inputString.cend();
	while(itr != end){
		//skip whitespaces
		if(utf8::is_whitespace(*itr) || utf8::is_control(*itr)){
			// go to next
			++itr;
		}else if(*itr == '"'){
			// find next " or end
			auto next = itr+1;
			for (; next != end; ++next) {
				if (*next == '"') break;
			}
			const bool has_quotation_marks = next != end;
			const auto distance = std::distance(itr + 1, next);
			const utf8::string_view::const_pointer pChar = &(*(itr + 1));
			commandList.emplace_back(pChar, distance);
			itr = next + has_quotation_marks;
		}else{
			// find next whitespaces
			auto next = itr+1;
			for (; next != end; ++next) {
				if (utf8::is_whitespace(*next)) break;
			}
			const auto distance = std::distance(itr, next);
			const utf8::string_view::const_pointer pChar = &(*itr);
			commandList.emplace_back(pChar, distance);
			itr = next;
		}
	}
	return commandList;
}



/// returns true if the common string length of the left name is greater thatn the right one
static bool common_lesser_command_name(const TermGui::Command& com, const utf8::string_view& str){
	auto strItr = str.begin();
	const auto strEnd = str.end();
	auto comItr = com.name.begin();
	const auto comEnd = com.name.end();
	
	for(; (comItr != comEnd) && (strItr != strEnd); (void)++comItr, (void)++strItr){
		if(!(*comItr <= *strItr)){
			return false;
		}
	}
	
	return true; 
}

/// returns true if the common string length of the left name is lesser than the right one
static bool greater_command_name(const utf8::string_view& str, const TermGui::Command& com){
	auto strItr = str.begin();
	const auto strEnd = str.end();
	auto comItr = com.name.begin();
	const auto comEnd = com.name.end();
	
	for(; (comItr != comEnd) && (strItr != strEnd); (void)++comItr, (void)++strItr){
		if(!(*comItr > *strItr)){
			return false;
		}
	}
	
	return true; 
}

TermGui::const_command_range find(const utf8::string_view& command, const TermGui::Command* first, const TermGui::Command* last) {
	const auto lowerBound = std::lower_bound(first, last, command, common_lesser_command_name);
	const auto upperBound = std::upper_bound(lowerBound, last, command, greater_command_name);
	return TermGui::const_command_range{lowerBound, upperBound};
}

void TermGui::CommandLine::show_cursor(bool logic) { 
	if(logic == true) this->message.clear();
	this->showCursor = logic; 
}


void TermGui::CommandLine::clear() {
	this->move_to_start_of_line();
	this->inputString.clear();
	historyItr = commandHistory.cend();
}

void TermGui::CommandLine::naive_insert(utf8::Char c){
	this->inputString.insert(cursorIndex, c);
	this->move_forward();
}

void TermGui::CommandLine::insert(utf8::Char c) {
	this->message.clear();
	this->historyItr = this->commandHistory.cend();
	if(c == '\n'){
		this->enter();
	}else if(c == '\b'){
		this->Delete();
	}else{
		this->naive_insert(c);
	}
}

void TermGui::CommandLine::enter() {
	const Command* command = possibleCommands.first;
	
	if(possibleCommands.first == nullptr || possibleCommands.last == nullptr){
		this->message = "command list is empty";
	}else if(possibleCommands.first != possibleCommands.last - 1){
		this->message = "command not found";
	}else if(command->callbackFn == nullptr){
		this->message = "command does not have a callback function";
	}else{
		auto args = parse_command_string (this->inputString);
		command->callbackFn(command->objectPtr, args);
	}
	
	if(this->commandHistory.empty()){
		this->commandHistory.push_front(this->inputString);	
	}else if(this->commandHistory.front() != this->inputString){
		this->commandHistory.push_front(this->inputString);	
	}
	
	this->clear();
}

void TermGui::CommandLine::erase() { 
	if(!this->is_end_of_line()){
		this->inputString.erase(this->cursorIndex); 
	}
}

void TermGui::CommandLine::Delete() {
	if(!this->is_start_of_line()){
		this->move_back();
		this->inputString.erase(this->cursorIndex);
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
	this->message.clear();
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
			this->inputString = *(this->historyItr);
			this->move_to_end_of_line();
		}
	}
}

/// loads the earlier command into the command line
void TermGui::CommandLine::move_down() {
	this->message.clear();
	// note that the cend() is the invalid state in the invalid state the command line will be empty
	if(this->historyItr != this->commandHistory.cend()){
		if(this->historyItr == this->commandHistory.cbegin()){
			this->clear();
			this->historyItr = this->commandHistory.cend();
		}else{
			--this->historyItr;
		}
		if(this->historyItr != this->commandHistory.cend()){
			this->inputString = *(this->historyItr);
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
	this->cursorIndex = this->inputString.size(); 
	this->screenColumn = (this->inputString.size() + 3 > this->text_width()) ? this->inputString.size() - (this->text_width() - 3): 0;
}

void TermGui::CommandLine::move_to_end_of_file() { 
	this->move_to_end_of_line(); 
}

bool TermGui::CommandLine::is_start_of_line() const { 
	return this->cursorIndex == 0; 
}

bool TermGui::CommandLine::is_end_of_line() const { 
	return this->cursorIndex == this->inputString.size(); 
}

void TermGui::CommandLine::render(std::string& outputString) const {
	if(this->GridCell::get_screen_width().y > 0){
		outputString += Term::cursor_move(this->GridCell::get_screen_position().y, this->GridCell::get_screen_position().x);
		if(this->display_message()){
			this->render_message(outputString);
		}else{
			this->render_command(outputString);
		}
	}
}

void TermGui::CommandLine::render_command(std::string& outputString) const {
	outputString += ':';
	size_type column = this->screenColumn;
	const size_type columnEnd = this->inputString.size();
	size_type screenColumn = 0;
	const size_type screenColumnEnd = this->text_width();
	
	for(; column < columnEnd && screenColumn < (screenColumnEnd - this->is_end_of_line()); ++column, (void)++screenColumn){
		const size_type show_cursor = this->showCursor && column == this->cursorIndex;
		if(this->inputString[column] == '\t' && show_cursor){
			outputString += to_string(TermGui::FontStyle::Reversed::ON);
			outputString += ' ';
			outputString += to_string(TermGui::FontStyle::Reversed::OFF);
			const size_type tabs_to_print = std::min(3ULL, (screenColumnEnd - this->is_end_of_line()) - screenColumn);
			outputString.append(tabs_to_print, ' ');
			screenColumn += tabs_to_print;
		}else if(this->inputString[column] == '\t'){
			const size_type tabs_to_print = std::min(4ULL, (screenColumnEnd - this->is_end_of_line()) - screenColumn);
			outputString.append(tabs_to_print, ' ');
			screenColumn += tabs_to_print-1;
		}else if(show_cursor && column == this->cursorIndex){
			outputString += to_string(TermGui::FontStyle::Reversed::ON);
			outputString += this->inputString[column].to_std_string_view();
			outputString += to_string(TermGui::FontStyle::Reversed::OFF);				
		}else{
			outputString += this->inputString[column].to_std_string_view();
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

void TermGui::CommandLine::render_message(std::string& outputString) const {
	outputString += '>';
	size_type column = 0;
	const size_type columnEnd = this->message.size();
	size_type screenColumn = 0;
	const size_type screenColumnEnd = this->text_width();
	
	if(column < columnEnd && screenColumn < screenColumnEnd && this->showCursor){
		// show cursor is true
		if(this->message[column] == '\t'){
			outputString += to_string(TermGui::FontStyle::Reversed::ON);
			outputString += ' ';
			outputString += to_string(TermGui::FontStyle::Reversed::OFF);
			const size_type tabs_to_print = std::min(3ULL, (screenColumnEnd - this->is_end_of_line()) - screenColumn);
			outputString.append(tabs_to_print, ' ');
			screenColumn += tabs_to_print;
		}else{
			outputString += to_string(TermGui::FontStyle::Reversed::ON);
			outputString += this->message[column].to_std_string_view();
			outputString += to_string(TermGui::FontStyle::Reversed::OFF);
		}
		++column, (void)++screenColumn;
	}
	
	for(; column < columnEnd && screenColumn < screenColumnEnd; ++column, (void)++screenColumn){
		// show cursor is false
		if(this->message[column] == '\t'){
			const size_type tabs_to_print = std::min(4ULL, screenColumnEnd - screenColumn);
			outputString.append(tabs_to_print, ' ');
			screenColumn += tabs_to_print-1;
		}else{
			outputString += this->message[column].to_std_string_view();
		}
	}
	
	outputString.append(screenColumnEnd - screenColumn, ' ');
}

TermGui::CommandLine::size_type TermGui::CommandLine::text_width() const {return (this->GridCell::get_screen_width().x - 1 /* 1 is the size of the start symbol */);}
bool TermGui::CommandLine::display_message() const {return !message.empty();}
char TermGui::CommandLine::start_symbol() const { return (this->display_message()) ? '>' : ':';}
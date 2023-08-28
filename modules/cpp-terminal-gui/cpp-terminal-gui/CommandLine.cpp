
// std c++
#include <algorithm>


// project
#include "CommandLine.hpp"
#include "FontStyle.hpp"

TermGui::CommandLine::CommandLine(object_pointer objectPtr, const_command_range commands, string_type inintMessage) : 
		objectPtr(objectPtr),
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
	this->showCursor = logic; 
	if(logic == true){
		this->message.clear();
		this->GridCell::set_target_width(TermGui::ScreenWidth{.x = this->get_screen_width().x, .y = 2});
	}else{
		this->GridCell::set_target_width(TermGui::ScreenWidth{.x = this->get_screen_width().x, .y = 1});
	}
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
		command->callbackFn(objectPtr, args);
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
	}else if(this->cursorColumn > this->screenColumn + this->input_line_width() - this->margin){
		this->screenColumn = this->cursorColumn - this->input_line_width() + this->margin;
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
	this->screenColumn = (this->inputString.size() + 3 > this->input_line_width()) ? this->inputString.size() - (this->input_line_width() - 3): 0;
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

TermGui::ScreenPosition TermGui::CommandLine::info_position() const{
	TermGui::ScreenPosition result = this->GridCell::get_screen_position();
	return result;
}

TermGui::ScreenWidth TermGui::CommandLine::info_width() const{
	const TermGui::ScreenWidth totalWidth = this->GridCell::get_screen_width();
	const TermGui::ScreenWidth commandLineWidth = this->command_line_width();
	const TermGui::ScreenWidth result{.x = totalWidth.x, .y = totalWidth.y - commandLineWidth.y};
	return result;
}

TermGui::ScreenPosition TermGui::CommandLine::command_line_position() const{
	TermGui::ScreenPosition infoPosition = this->info_position();
	TermGui::ScreenWidth infoWidth = this->info_width();
	TermGui::ScreenPosition result{.x = infoPosition.x, .y = infoPosition.y + infoWidth.y};
	return result;
}

TermGui::ScreenWidth TermGui::CommandLine::command_line_width() const{
	const TermGui::ScreenWidth result{.x = this->GridCell::get_screen_width().x, .y = std::min(this->GridCell::get_screen_width().y, 1)};
	return result;
}

void TermGui::CommandLine::render_command(std::string& outputString) const {
	render_command_info(outputString);
	render_command_line(outputString);
}

void TermGui::CommandLine::render(std::string& outputString) const {
	if(this->GridCell::get_screen_width().y > 0){
		
		if(this->display_message()){
			this->render_message(outputString);
		}else{
			this->render_command(outputString);
		}
	}
}

void TermGui::CommandLine::render_command_info(std::string& outputString) const {
	const auto infoWidth = this->info_width();
	const auto infoPosition = this->info_position();
	
	if(infoWidth.y == 0) return;
	
	outputString += Term::cursor_move(infoPosition.y, infoPosition.x);
	size_type screenColumn = 0;
	size_type screenEnd = infoWidth.x;
	
	for(auto comItr = this->possibleCommands.first; comItr != this->possibleCommands.last && screenColumn != screenEnd; (void)++comItr){
		for(auto nameItr = comItr->name.begin(); nameItr != comItr->name.end() && screenColumn != screenEnd; (void)++nameItr, (void)++screenColumn){
			outputString += nameItr->to_std_string_view();
		}
		if(screenColumn != screenEnd){
			outputString += ' ';
			++screenColumn;
		}
	}
	
	outputString.append(screenEnd - screenColumn, ' ');
}

void TermGui::CommandLine::render_command_line(std::string& outputString) const {
	outputString += Term::cursor_move(this->command_line_position().y, this->command_line_position().x);
	outputString += ':';
	size_type column = this->screenColumn;
	const size_type columnEnd = this->inputString.size();
	size_type screenColumn = 0;
	const size_type screenColumnEnd = this->input_line_width();
	
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
	outputString += Term::cursor_move(this->GridCell::get_screen_position().y, this->GridCell::get_screen_position().x);
	outputString += '>';
	size_type column = 0;
	const size_type columnEnd = this->message.size();
	size_type screenColumn = 0;
	const size_type screenColumnEnd = this->input_line_width();
	
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

TermGui::CommandLine::size_type TermGui::CommandLine::input_line_width() const {return (this->GridCell::get_screen_width().x - 1 /* 1 is the size of the start symbol */);}
bool TermGui::CommandLine::display_message() const {return !message.empty();}
char TermGui::CommandLine::start_symbol() const { return (this->display_message()) ? '>' : ':';}
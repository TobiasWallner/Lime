
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
static std::vector<utf8::const_string_view> parse_command_string(utf8::const_string_view inputString){
	std::vector<utf8::const_string_view> commandList;
	auto itr = inputString.cbegin();
	const auto end = inputString.cend();
	while(itr != end){
		//skip whitespaces
		if(utf8::is_whitespace(*itr) || utf8::is_control(*itr)){
			// go to next
			++itr;
		}else if(*itr == '"'){
			// find next " or end
			const auto first = ++itr;
			while(true){
				if(itr == end){
					commandList.emplace_back(first, itr);
				}else if(*itr++ == '"'){
					commandList.emplace_back(first, itr);
					++itr;
				}
			}
		}else{
			const auto first = itr;
			// find next whitespaces
			while(itr != end){
				if(utf8::is_whitespace(*itr++)) break;
			}
			commandList.emplace_back(first, itr);
		}
	}
	return commandList;
}

static bool common_less(const utf8::const_string_view& lhs, const utf8::const_string_view& rhs) {
	// TODO: put in utf8 string
	auto rhsItr = rhs.begin();
	const auto rhsEnd = rhs.end();
	auto lhsItr = lhs.begin();
	const auto lhsEnd = lhs.end();

	for (; (rhsItr != rhsEnd) && (lhsItr != lhsEnd); (void)++rhsItr, (void)++lhsItr) {
		if (*lhsItr < *rhsItr) {
			return true;
		}
		else if (*lhsItr > *rhsItr) {
			return false;
		}
	}

	return false;
}

static bool strict_less(const utf8::const_string_view& lhs, const utf8::const_string_view& rhs) {
	// TODO: put in utf8 string
	auto rhsItr = rhs.begin();
	const auto rhsEnd = rhs.end();
	auto lhsItr = lhs.begin();
	const auto lhsEnd = lhs.end();

	for (; (rhsItr != rhsEnd) && (lhsItr != lhsEnd); (void)++rhsItr, (void)++lhsItr) {
		if (*lhsItr < *rhsItr) {
			return true;
		}
		else if (*lhsItr > *rhsItr) {
			return false;
		}
	}

	return lhsItr == lhsEnd && rhsItr != rhsEnd;
}

static bool lower_bound_fn(const TermGui::Command& com, const utf8::const_string_view& str){
	return strict_less(com.name, str);
}

/// returns true if the common string length of the left name is lesser than the right one
static bool upper_bound_fn(const utf8::const_string_view& str, const TermGui::Command& com){
	return common_less(str, com.name);
}

TermGui::const_command_range TermGui::find(const utf8::const_string_view& command, const TermGui::const_command_range& range) {
	const auto lowerBound = std::lower_bound(range.first, range.last, command, lower_bound_fn);
	const auto upperBound = std::upper_bound(lowerBound, range.last, command, upper_bound_fn);
	return TermGui::const_command_range{lowerBound, upperBound};
}

void TermGui::CommandLine::show_cursor(bool logic) { 
	this->showCursor = logic; 
	if(logic == true) this->message.clear();
	this->update_height();
}

TermGui::ScreenWidth::size_type TermGui::CommandLine::get_target_info_height(){
	if(this->showCursor){
			if(this->possibleCommands.first == nullptr || this->possibleCommands.last == nullptr){
				return 0;
			}else if(this->possibleCommands.last - this->possibleCommands.first <= 0){
				return 0;
			}else if(this->possibleCommands.last - this->possibleCommands.first == 1){
				const TermGui::ScreenWidth::size_type nameHeight = 1;
				const TermGui::ScreenWidth::size_type infoHeight = 1;
				const TermGui::ScreenWidth::size_type flagsHeight = this->possibleCommands.first->flags.size();
				return nameHeight + infoHeight + flagsHeight;
			}else{
				return 1;
			}
	}else{
		return 0;
	}
}

void TermGui::CommandLine::update_height(){
	const TermGui::ScreenWidth::size_type commandLineHeight = 1;
	const TermGui::ScreenWidth targetWidth = this->GridCell::get_target_width();
	if(this->showCursor == false || !this->message.empty()){
		if(commandLineHeight != targetWidth.y){
			this->GridCell::set_target_width(TermGui::ScreenWidth{.x = targetWidth.x, .y = commandLineHeight});
		}
	}else{
		const TermGui::ScreenWidth::size_type infoHeight = get_target_info_height();
		const TermGui::ScreenWidth::size_type totalHeight = commandLineHeight + infoHeight;
		if(totalHeight != targetWidth.y){
			this->GridCell::set_target_width(TermGui::ScreenWidth{.x = targetWidth.x, .y = totalHeight});
		}
	}
}

void TermGui::CommandLine::update_on_input() {
	this->select_possible_commands();
	this->update_height();
}

// does not call the update function
// does not clear the message string
void TermGui::CommandLine::clear_internal() {
	this->move_to_start_of_line_internal();
	this->inputString.clear();
	historyItr = commandHistory.cend();
}

void TermGui::CommandLine::clear() {
	this->move_to_start_of_line();
	this->inputString.clear();
	historyItr = commandHistory.cend();
	this->update_on_input();
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
	const Command* command = this->possibleCommands.first;
	
	if(this->possibleCommands.first == nullptr || this->possibleCommands.last == nullptr){
		this->message = "command list is empty";
	}else if(this->possibleCommands.first != this->possibleCommands.last - 1){
		this->message = "command not found";
	}else if(command->callbackFn == nullptr){
		this->message = "command does not have a callback function";
	}else{
		const auto args = parse_command_string(this->inputString);
		command->callbackFn(objectPtr, args);
	}
	
	if(this->commandHistory.empty()){
		this->commandHistory.push_front(this->inputString);	
	}else if(this->commandHistory.front() != this->inputString){
		this->commandHistory.push_front(this->inputString);	
	}
	
	this->clear_internal();
	this->update_on_input();
}

void TermGui::CommandLine::erase() { 
	if(!this->is_end_of_line()){
		auto itr = this->inputString.begin();
		std::advance(itr, this->cursorIndex);
		this->inputString.erase(itr); 
		this->update_on_input();
	}
}

void TermGui::CommandLine::Delete() {
	if(!this->is_start_of_line()){
		this->move_back();
		this->erase();
	}
}

void TermGui::CommandLine::naive_insert(utf8::Char c) {
	auto itr = this->inputString.begin();
	std::advance(itr, this->cursorIndex);
	this->inputString.insert(itr, c);
	this->move_forward();
	this->update_on_input();
}


utf8::const_string_view TermGui::CommandLine::view_first_word() const{
	const auto first = this->inputString.begin();
	const auto last = this->inputString.end();
	// skip whitespaces
	const auto commandBegin = std::find_if_not(first, last, utf8::is_whitespace);
	// find end of word
	const auto commandEnd = std::find_if(commandBegin, last, utf8::is_whitespace);
	utf8::const_string_view result(commandBegin, commandEnd);
	return result;
}

void TermGui::CommandLine::select_possible_commands(){
	if (this->inputString.empty()) {
		this->possibleCommands = this->commands;
	}else {
		this->possibleCommands = find(this->view_first_word(), this->commands);
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
	this->message.clear();
	this->cursorIndex += !this->is_end_of_line();
	this->move_screen_to_cursor();
	this->update_on_input();
}

void TermGui::CommandLine::move_back() { 
	this->message.clear();
	this->cursorIndex -= !this->is_start_of_line();
	this->move_screen_to_cursor();
	this->update_on_input();
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
			this->move_to_end_of_line_internal();
		}
	}
	this->update_on_input();
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
			this->move_to_end_of_line_internal();
		}	
	}
	this->update_on_input();
}


void TermGui::CommandLine::move_to_start_of_line_internal() {
	this->cursorIndex = 0;
	this->screenColumn = 0;
}


void TermGui::CommandLine::move_to_start_of_line() { 
	this->message.clear();
	this->move_to_start_of_line_internal();
	this->update_on_input();
}



void TermGui::CommandLine::move_to_start_of_file() {
	this->message.clear();
	this->move_to_start_of_line();
	this->update_on_input();
}

void TermGui::CommandLine::move_to_end_of_line_internal() {
	this->cursorIndex = this->inputString.size();
	this->screenColumn = (this->inputString.size() + 3 > this->input_line_width()) ? this->inputString.size() - (this->input_line_width() - 3) : 0;
}

void TermGui::CommandLine::move_to_end_of_line() {
	this->message.clear();
	this->move_to_end_of_line_internal();
	this->update_on_input();
}

void TermGui::CommandLine::move_to_end_of_file() {
	this->message.clear();
	this->move_to_end_of_line();
	this->update_on_input();
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
	const TermGui::ScreenWidth result{.x = this->GridCell::get_screen_width().x, .y = std::min(static_cast<TermGui::ScreenWidth::size_type>(this->GridCell::get_screen_width().y), static_cast<TermGui::ScreenWidth::size_type>(1))};
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
	if(this->possibleCommands.first == nullptr || this->possibleCommands.last == nullptr){
		// do nothing
	}else if(this->possibleCommands.last - this->possibleCommands.first < 1){
		// do nothing
	}else if(this->possibleCommands.last - this->possibleCommands.first == 1){
		this->render_single_command_info(outputString);
	}else{
		this->render_multiple_command_info(outputString);
	}
}

long TermGui::max_flag_size(const std::vector<TermGui::Command::Flag>& flags){
	long max = 0;
	for(const auto& flag : flags){
		if(flag.name.size() > max){
			max = flag.name.size();
		}
	}
	return max;
}

void TermGui::CommandLine::render_single_command_info(std::string& outputString) const {
	if(this->possibleCommands.first == nullptr || this->possibleCommands.last == nullptr){
		return;
	}
	
	const TermGui::ScreenWidth infoWidth = this->info_width();
	const TermGui::ScreenPosition infoPosition = this->info_position();
	
	const TermGui::Command* command = this->possibleCommands.first;
	
	TermGui::ScreenWidth::size_type lineItr = 0;
	TermGui::ScreenWidth::size_type lineEnd = infoWidth.y;
	
	if(lineItr != lineEnd){// print name
		outputString += Term::cursor_move(static_cast<size_t>(infoPosition.y + lineItr), static_cast<size_t>(infoPosition.x));

		auto itr = command->name.begin();
		const auto end = command->name.end();
		TermGui::ScreenWidth::size_type columnItr = 0;
		const TermGui::ScreenWidth::size_type columnEnd = infoWidth.x;
		
		for(; itr != end && columnItr != columnEnd; (void)++itr, (void)++columnItr){
			outputString.append(*itr);
		}
		
		outputString.append(columnEnd - columnItr, ' ');
		
		++lineItr;
	}
	if(lineItr != lineEnd){// print info
		outputString += Term::cursor_move(static_cast<size_t>(infoPosition.y + lineItr), static_cast<size_t>(infoPosition.x));
		
		auto itr = command->info.begin();
		const auto end = command->info.end();
		TermGui::ScreenWidth::size_type columnItr = 0;
		const TermGui::ScreenWidth::size_type columnEnd = infoWidth.x;
		
		for(; itr != end && columnItr != columnEnd; (void)++itr, (void)++columnItr){
			outputString.append(*itr);
		}
		
		outputString.append(columnEnd - columnItr, ' ');
		
		++lineItr;
	}
	if(lineItr != lineEnd){// print flags
		const std::vector<TermGui::Command::Flag>& flags = command->flags;
		auto flagItr = flags.begin();
		const auto flagEnd = flags.end();
		
		const ScreenWidth::size_type maxFlagWidth = static_cast<ScreenWidth::size_type>(max_flag_size(flags));
		
		for(; flagItr != flagEnd && lineItr != lineEnd; (void)++flagItr, (void)++lineItr){
			outputString += Term::cursor_move(static_cast<size_t>(infoPosition.y + lineItr), static_cast<size_t>(infoPosition.x));
	
			TermGui::ScreenWidth::size_type columnItr = 0;
			const ScreenWidth::size_type columnEnd = infoWidth.x;
			
			{// print flag name
				auto itr = flagItr->name.begin();
				const auto end = flagItr->name.end();
				
				for(; itr != end && columnItr != columnEnd; (void)++itr, (void)++columnItr){
					outputString.append(*itr);
				}
				
				const ScreenWidth::size_type spaceing = 2;
				outputString.append(std::min(spaceing + maxFlagWidth - columnItr, columnEnd - columnItr), ' ');
			}
			{// print flag info
				auto itr = flagItr->info.begin();
				const auto end = flagItr->info.end();
				
				for(; itr != end && columnItr != columnEnd; (void)++itr, (void)++columnItr){
					outputString.append(*itr);
				}
				
				outputString.append(columnEnd - columnItr, ' ');
			}
		}
	}
}

void TermGui::CommandLine::render_multiple_command_info(std::string& outputString) const {
	if(this->possibleCommands.first == nullptr || this->possibleCommands.last == nullptr){
		return;
	}
	
	const auto infoWidth = this->info_width();
	const auto infoPosition = this->info_position();
	
	if(infoWidth.y == 0) return;
	
	outputString += Term::cursor_move(infoPosition.y, infoPosition.x);
	size_type screenColumn = 0;
	size_type screenEnd = infoWidth.x;
	
	for(auto comItr = this->possibleCommands.first; comItr != this->possibleCommands.last && screenColumn != screenEnd; (void)++comItr){
		for(auto nameItr = comItr->name.begin(); nameItr != comItr->name.end() && screenColumn != screenEnd; (void)++nameItr, (void)++screenColumn){
			outputString += *nameItr;
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
	auto stringItr = this->inputString.begin();
	const auto stringEnd = this->inputString.end();
	size_type screenColumn = 0;
	const size_type screenColumnEnd = this->input_line_width();
	
	for(; stringItr != stringEnd && screenColumn < (screenColumnEnd - this->is_end_of_line()); ++column, (void)++screenColumn, (void)++stringItr){
		const size_type show_cursor = this->showCursor && column == this->cursorIndex;
		if(*stringItr == '\t' && show_cursor){
			outputString += to_string(TermGui::FontStyle::Reversed::On);
			outputString += ' ';
			outputString += to_string(TermGui::FontStyle::Reversed::Off);
			const size_type tabs_to_print = std::min(static_cast<size_type>(3), static_cast<size_type>((screenColumnEnd - this->is_end_of_line()) - screenColumn));
			outputString.append(tabs_to_print, ' ');
			screenColumn += tabs_to_print;
		}else if(*stringItr == '\t'){
			const size_type tabs_to_print = std::min(static_cast<size_type>(4), static_cast<size_type>((screenColumnEnd - this->is_end_of_line()) - screenColumn));
			outputString.append(tabs_to_print, ' ');
			screenColumn += tabs_to_print-1;
		}else if(show_cursor && column == this->cursorIndex){
			outputString += to_string(TermGui::FontStyle::Reversed::On);
			outputString += *stringItr;
			outputString += to_string(TermGui::FontStyle::Reversed::Off);				
		}else{
			outputString += *stringItr;
		}
	}
	
	if (this->showCursor && this->is_end_of_line()) {
		outputString += to_string(TermGui::FontStyle::Reversed::On);
		outputString += ' ';
		outputString += to_string(TermGui::FontStyle::Reversed::Off);
		++screenColumn;
	}
	outputString.append(screenColumnEnd - screenColumn, ' ');
}

void TermGui::CommandLine::render_message(std::string& outputString) const {
	outputString += Term::cursor_move(this->GridCell::get_screen_position().y, this->GridCell::get_screen_position().x);
	outputString += '>';
	size_type column = 0;
	auto messageItr = this->message.begin();
	const auto messageEnd = this->message.end();
	size_type screenColumn = 0;
	const size_type screenColumnEnd = this->input_line_width();
	
	if(messageItr != messageEnd && screenColumn < screenColumnEnd && this->showCursor){
		// show cursor is true
		if(*messageItr == '\t'){
			outputString += to_string(TermGui::FontStyle::Reversed::On);
			outputString += ' ';
			outputString += to_string(TermGui::FontStyle::Reversed::Off);
			const size_type tabs_to_print = std::min(static_cast<size_type>(3), static_cast<size_type>((screenColumnEnd - this->is_end_of_line()) - screenColumn));
			outputString.append(tabs_to_print, ' ');
			screenColumn += tabs_to_print;
		}else{
			outputString += to_string(TermGui::FontStyle::Reversed::On);
			outputString += *messageItr;
			outputString += to_string(TermGui::FontStyle::Reversed::Off);
		}
		++column, (void)++screenColumn, (void)++messageItr;
	}
	
	for(; messageItr != messageEnd && screenColumn < screenColumnEnd; ++column, (void)++screenColumn, (void)++messageItr){
		// show cursor is false
		if(*messageItr == '\t'){
			const size_type tabs_to_print = std::min(static_cast<size_type>(4), static_cast<size_type>(screenColumnEnd - screenColumn));
			outputString.append(tabs_to_print, ' ');
			screenColumn += tabs_to_print-1;
		}else{
			outputString += *messageItr;
		}
	}
	
	outputString.append(screenColumnEnd - screenColumn, ' ');
}

TermGui::CommandLine::size_type TermGui::CommandLine::input_line_width() const {return (this->GridCell::get_screen_width().x - 1 /* 1 is the size of the start symbol */);}
bool TermGui::CommandLine::display_message() const {return !message.empty();}
char TermGui::CommandLine::start_symbol() const { return (this->display_message()) ? '>' : ':';}
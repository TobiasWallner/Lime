
// C++ std
#include <fstream>
#include <iostream>
#include <algorithm>

// utf8 string 
#include <utf8_char.hpp>
#include <utf8_string.hpp>

// project
#include "TextEditor.hpp"

void TermGui::TextEditor::init(ScreenPosition screenPosition, ScreenWidth screenWidth){
	// init screen
	this->screenPosition = screenPosition;
	this->screenWidth = screenWidth;
	
	// init list
	this->_text.emplace_back();

	// init cursor
	this->_cursor.lineNumber = 0;
	this->_cursor.columnNumber = 0;
	this->_cursor.lineIterator = this->begin();

	// init render
	this->renderLineStart = 0;
	this->renderLineStartItr = this->begin();
	this->renderColumnStart = 0;
}

bool TermGui::TextEditor::empty() const{
	if(this->_text.empty()){
		return true;
	}else{
		const_reference front = this->front();
		const_reference back = this->back();
		const auto* pfront = &front;
		const auto* pback = &back;
		const bool is_only_one_line = pfront == pback; // equality by pointer comparison
		const bool is_line_empty = front.empty();
		const bool result = is_only_one_line && is_line_empty;
		return result;
	}
}

void TermGui::TextEditor::insert_naive(utf8::Char character){
	this->_cursor.lineIterator->insert(this->_cursor.columnNumber, character);
	this->move_forward();
}

void TermGui::TextEditor::insert_new_line(){
	Cursor prevCursor = this->_cursor;
	this->insert_line_after();
	this->move_down();
	this->lineItr()->move_append(*(prevCursor.lineIterator), prevCursor.columnNumber, prevCursor.lineIterator->size() - prevCursor.columnNumber);
}

void TermGui::TextEditor::insert(utf8::Char c){
	if(c == '\n'){
		this->insert_new_line();
	}else if(c == '\b'){
		this->Delete();
	}else if(c == '\r' || c == '\v' || c == '\f'){
		return;
	}else{
		this->insert_naive(c);
	}	
}

TermGui::TextEditor::iterator TermGui::TextEditor::find_line(size_type line){
	//TODO: possible optimisation: decide between going from the start, current position or the end
	auto itr = this->_text.begin();
	const auto end = this->_text.end();
	for(; itr != end && line != 0; ++itr, (void)--line);
	return itr;
}

TermGui::TextEditor::const_iterator TermGui::TextEditor::find_line(size_type line) const {
	//TODO: possible optimisation: decide between going from the start, current position or the end
	auto itr = this->_text.cbegin();
	const auto end = this->_text.cend();
	for(; itr != end && line != 0; ++itr, (void)--line);
	return itr;
}

void TermGui::TextEditor::move_forward(){
	if(this->is_end_of_file()){
		return;
	}else if(this->is_end_of_line()){
		this->move_down();
		this->move_to_start_of_line();
	}else{
		++this->_cursor.columnNumber;
		
		// scrowl forward
		const bool over_scrowl_protection = this->renderColumnStart + this->line_width() <= this->lineItr()->size();
		const bool cursor_near_end = this->cursor_column() + 3 > this->line_width() + this->renderColumnStart;
		const bool scrowlForward = over_scrowl_protection && cursor_near_end;
		this->renderColumnStart += scrowlForward;
	}
}

void TermGui::TextEditor::move_back(){
	if(this->is_start_of_file()){
		return;
	}else if(this->is_start_of_line()){
		this->move_up();
		this->move_to_end_of_line();
	}else{
		--this->_cursor.columnNumber;
		
		// scrowl back
		const bool underscrowl_protection =  this->renderColumnStart > 0;
		const bool cursor_near_start = this->renderColumnStart + 3 > this->cursor_column();
		const bool scrowlBack = underscrowl_protection && cursor_near_start;
		this->renderColumnStart -= scrowlBack;
	}
}

void TermGui::TextEditor::move_up(){
	if(this->is_start_of_file()){
		return;
	}else if(this->is_first_line()){
		this->move_to_start_of_line();
	}else{
		const bool eol = this->is_end_of_line();
		--this->_cursor.lineNumber;
		--this->_cursor.lineIterator;
		this->_cursor.columnNumber = eol ? this->_cursor.lineIterator->size() : std::min(static_cast<TermGui::ColorString::size_type>(this->_cursor.columnNumber), this->_cursor.lineIterator->size());
		if (this->_cursor.columnNumber < this->renderColumnStart || (this->renderColumnStart + this->screenWidth.x) < this->_cursor.columnNumber) {
			this->renderColumnStart = (this->_cursor.columnNumber > (this->screenWidth.x + 3)) ?  this->_cursor.columnNumber - this->screenWidth.x + 3 : 0L;
		}
		
		// scrowl up
		const bool underscrowl_protection =  this->renderLineStart > 0;
		const bool cursor_near_start = this->renderLineStart + 3 > this->cursor_line();
		const bool scrowl_up = underscrowl_protection && cursor_near_start;
		if(scrowl_up){
			--this->renderLineStart;
			--this->renderLineStartItr;
		}
	}
}

void TermGui::TextEditor::move_down(){
	if(this->is_end_of_file()){
		return;
	}else if(this->is_last_line()){
		this->move_to_end_of_line();
	}else{
		const bool eol = this->is_end_of_line();
		++this->_cursor.lineNumber;
		++this->_cursor.lineIterator;
		this->_cursor.columnNumber = eol ? this->_cursor.lineIterator->size() : std::min(static_cast<TermGui::ColorString::size_type>(this->_cursor.columnNumber), this->_cursor.lineIterator->size());
		if (this->_cursor.columnNumber < this->renderColumnStart || (this->renderColumnStart + this->screenWidth.x) < this->_cursor.columnNumber) {
			this->renderColumnStart = (this->_cursor.columnNumber > (this->screenWidth.x + 3)) ? this->_cursor.columnNumber - this->screenWidth.x + 3 : 0L;
		}
		
		// scrowl down 
		const bool over_scrowl_protection = this->renderLineStart + this->line_height() <= this->number_of_lines();
		const bool cursor_near_end = this->cursor_line() + 3 > this->line_height() + this->renderLineStart;
		const bool scrowlDown = over_scrowl_protection && cursor_near_end;
		if(scrowlDown){
			++this->renderLineStart;
			++this->renderLineStartItr;
		}
	}
}

void TermGui::TextEditor::move_to_start_of_line() { 
	this->_cursor.columnNumber = 0;
        
	// scrowl
	this->renderColumnStart = 0;
}

void TermGui::TextEditor::move_to_start_of_file() {
	this->_cursor.columnNumber = 0;
	this->_cursor.lineNumber = 0;
	this->_cursor.lineIterator = this->begin();

	// scrowl
	this->renderColumnStart = 0;
	this->renderLineStart = 0;
	this->renderLineStartItr = this->begin();
}

void TermGui::TextEditor::move_to_end_of_line() {
	this->_cursor.columnNumber = this->lineItr()->size();

    // scrowl
	if ((this->renderColumnStart + this->screenWidth.x) < this->_cursor.columnNumber) {
		this->renderColumnStart = (this->_cursor.columnNumber > (this->screenWidth.x + 3)) ? this->_cursor.columnNumber - this->screenWidth.x + 3 : 0L;
	}
}

void TermGui::TextEditor::move_to_end_of_file() {
	this->_cursor.lineNumber = this->_text.empty() ? 0 : this->_text.size() - 1;
	this->_cursor.lineIterator = this->last();
	this->_cursor.columnNumber = this->last()->size();

    // scrowl 
    this->renderColumnStart = (this->_cursor.columnNumber > (this->screenWidth.x + 3)) ? this->_cursor.columnNumber - this->screenWidth.x + 3 : 0L;
	if(this->number_of_lines() <= this->screenWidth.y * 3 / 4){
		this->renderLineStart = 0;
		this->renderLineStartItr = this->begin();
	}else{
		this->renderLineStart = this->number_of_lines() - this->screenWidth.y * 3 / 4;
		this->renderLineStartItr = this->_cursor.lineIterator;
		for(long i = 1; i < this->screenWidth.y * 3 / 4; ++i) --this->renderLineStartItr;
	}
}

void TermGui::TextEditor::render(std::string& outputString) const {
	TextEditor::const_iterator lineItr = this->renderLineStartItr;
	size_type lineNumber = this->renderLineStart;
	size_type screenLineNumber = 0;
	for(; screenLineNumber < this->screenWidth.y && lineItr != this->cend(); ++lineItr, (void)++lineNumber, (void)++screenLineNumber){
		outputString += Term::cursor_move(this->screenPosition.y + screenLineNumber, this->screenPosition.x);
		
		// print styles until start of render
		auto stylesItr = lineItr->style_list_cbegin();
		for(; (stylesItr != lineItr->style_list_cend()) && (stylesItr->index < this->renderColumnStart); ++stylesItr){
			stylesItr->render(outputString);
		}
		
		// print characters until the end of the screen has been reached
		size_type column = this->renderColumnStart;
		const size_type columnEnd = this->line_width();
		const auto string = lineItr->string_cbegin();
		auto screenColumn = 0;
		const auto screenColumnEnd = this->screenWidth.x;
		for(; screenColumn < (screenColumnEnd - this->is_end_of_line()) && column < lineItr->size(); ++column, ++screenColumn){
			if (stylesItr != lineItr->style_list_cend()) {
				if (stylesItr->index == column) {
					stylesItr->render(outputString);
					++stylesItr;
				}
			}
			const auto show_cursor = this->showCursor && lineNumber == this->cursor_line() && column == this->cursor_column();
			if(string[column] == '\t' && show_cursor){
				outputString += to_string(FontStyle::Reversed::ON);
				outputString += ' ';
				outputString += to_string(FontStyle::Reversed::OFF);
				
				const auto tabs_to_print = std::min(this->tab_width-1, (screenColumnEnd - this->is_end_of_line()) - screenColumn);
				outputString.append(tabs_to_print, ' ');
				screenColumn += tabs_to_print;
			}else if(string[column] == '\t'){
				const auto tabs_to_print = std::min(4, (screenColumnEnd - this->is_end_of_line()) - screenColumn);
				outputString.append(tabs_to_print, ' ');
				screenColumn += tabs_to_print-1;
			}else if(show_cursor) {
				// print cursor in text
				outputString += to_string(TermGui::FontStyle::Reversed::ON);
				outputString += string[column].to_std_string_view();
				outputString += to_string(TermGui::FontStyle::Reversed::OFF);
			}else{
				outputString += string[column].to_std_string_view();
			}
		}
		
		// print cursor past the end of the line
		if (this->showCursor && lineNumber == this->cursor_line() && this->is_end_of_line()) {
			outputString += to_string(FontStyle::Reversed::ON);
			outputString += ' ';
			outputString += to_string(FontStyle::Reversed::OFF);
			++screenColumn;
		}
		
		// print styles until the end of the string
		for(; (stylesItr != lineItr->style_list_cend()); ++stylesItr){
			stylesItr->render(outputString);
		}
		
		// override characters until the end of the provided screen
		outputString.append(this->screenWidth.x - screenColumn, ' ');
	}
	//override lines until the end of the provided screen
	for(;screenLineNumber < this->screenWidth.y; ++screenLineNumber){
		outputString += Term::cursor_move(this->screenPosition.y + screenLineNumber, this->screenPosition.x);
		outputString += '~';
		outputString.append(this->screenWidth.x - 1, ' ');
	}
}


bool TermGui::TextEditor::append_file(std::ifstream& file) {
	char buffer[4 * 1024];
	if (file.is_open()) {
		while (!file.eof()) {
			file.read(buffer, 4 * 1024);
			this->insert(buffer, file.gcount());
		}
		return true;
	}
	return false;
}

bool TermGui::TextEditor::append_file(const std::filesystem::path& path){
	if(!std::filesystem::is_regular_file(path)) {return false;}
	std::ifstream file(path);
	return append_file(file);
}



bool TermGui::TextEditor::read_file(std::ifstream& file) {
	this->clear();
	const auto result = append_file(file);
	this->move_to_start_of_file();
	return result;
}

bool TermGui::TextEditor::read_file(const std::filesystem::path& path) {
	if (!std::filesystem::is_regular_file(path)) { return false; }
	std::ifstream file(path);
	return read_file(file);
}

bool TermGui::TextEditor::write_file(const std::filesystem::path& path){
	std::ofstream file(path);
	return write_file(file);
}

bool TermGui::TextEditor::write_file(std::ofstream& file){
	if(file.is_open()){
		auto iterator = _text.cbegin();
		file << *(iterator++);
        for(; iterator != _text.cend(); ++iterator){
            file << "\n" << *iterator;
        }
		return true;
    }
	return false;
}

void TermGui::TextEditor::Delete(){
	if(this->is_start_of_file()){
		// nothing to do
	}else if(this->is_start_of_line()){
		// remove the line break
		this->move_up();
		this->move_to_end_of_line();
		iterator nextLineIterator = this->_cursor.lineIterator;
		++nextLineIterator;
		this->_cursor.lineIterator->append(*nextLineIterator);
		this->_text.erase(nextLineIterator);
	}else{
		// erase element inside string
		this->move_back();
		this->_cursor.lineIterator->erase(this->_cursor.columnNumber);
	}
}

void TermGui::TextEditor::erase(){
	if(this->is_end_of_file()){
		// nothing to do
	}else if(this->is_end_of_line()){
		// erase line break
		iterator nextLineIterator = this->_cursor.lineIterator;
		++nextLineIterator;
		this->_cursor.lineIterator->append(*nextLineIterator);
		this->_text.erase(nextLineIterator);
	}else{
		// erase element inside string
		this->_cursor.lineIterator->erase(this->_cursor.columnNumber);
	}
}

void TermGui::TextEditor::enter() {this->insert_new_line();}

void TermGui::TextEditor::set_screen_position(TermGui::ScreenPosition position) {
	this->screenPosition = position;
}

TermGui::ScreenPosition TermGui::TextEditor::get_screen_position() const {
	return this->screenPosition;
}

void TermGui::TextEditor::set_screen_width(TermGui::ScreenWidth width) {
	this->screenWidth = width;
}

TermGui::ScreenWidth TermGui::TextEditor::get_screen_width() const {
	return this->screenWidth;
}

bool TermGui::operator==(const TermGui::TextEditor& lhs, const TermGui::TextEditor& rhs){
	if(lhs.number_of_lines() == rhs.number_of_lines()){
		auto lhsItr = lhs.cbegin();
		auto rhsItr = rhs.cbegin();
		for(; lhsItr != lhs.cend() && rhsItr != rhs.cend(); ++lhsItr, ++rhsItr){
			if(*lhsItr != *rhsItr){
				return false;
			}
		}
		return true;
	}
	return false;
}



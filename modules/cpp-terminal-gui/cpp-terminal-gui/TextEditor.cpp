
// C++ std
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

// utf8 string 
#include <utf8/char.hpp>
#include <utf8/string.hpp>

// project
#include "TextEditor.hpp"
#include "ColorThemes.hpp"

// +++++++++++++++++++++++ TextEditor ++++++++++++++++++++++++

TermGui::TextEditor::TextEditor(std::filesystem::path filename) :
		GridCell(1.f, 0, 100),
		_text(1),
		_cursor(this),
		_filename(filename),
		screenColumn(0),
		topScreenLine(this){}


void TermGui::TextEditor::move_screen_up(){
	if(!this->topScreenLine.is_first_line())
		this->topScreenLine.move_up_to_start_of_line();		
}

void TermGui::TextEditor::move_screen_down(){
	if(!this->topScreenLine.is_last_line())
		this->topScreenLine.move_down_to_start_of_line();
}

void TermGui::TextEditor::move_screen_right(std::int32_t n){
	this->screenColumn = (this->screenColumn > n) ? this->screenColumn - n : 0;
}

void TermGui::TextEditor::move_screen_left(std::int32_t n){
	this->screenColumn += n;
}

void TermGui::TextEditor::move_screen_to_cursor(const TextCursor& cursor){
	this->move_screen_lines_to_cursor(cursor);
	this->move_screen_column_to_cursor(cursor);
}

void TermGui::TextEditor::move_screen_lines_to_cursor(const TextCursor& cursor){
	while(this->is_above_margin(cursor) && !this->is_screen_at_top()) this->move_screen_up();
	while(this->is_below_margin(cursor) && !this->is_screen_at_bottom()) this->move_screen_down();
}

void TermGui::TextEditor::move_screen_column_to_cursor(const TextCursor& cursor){
	if(cursor.screen_column() < this->screenColumn + this->margin){
		this->screenColumn = (cursor.screen_column() < this->margin) ? 0 : cursor.screen_column() - this->margin;
	}else if(cursor.screen_column() > this->screenColumn + this->text_width() - this->margin){
		this->screenColumn = cursor.screen_column() - this->text_width() + this->margin;
	}
}

void TermGui::TextEditor::tab_size(std::int32_t newTabSize){
	this->tabSize = newTabSize;
	this->_cursor.update_tab_size();
}

void TermGui::TextEditor::show_cursor(bool logic) {
	this->showCursor = logic;
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
	Text::iterator lineIterator = this->un_const(this->_cursor.line_iterator());
	lineIterator->insert(this->_cursor.column_index(), character);
	this->move_forward();
	this->saved = false;
}

void TermGui::TextEditor::insert_new_line(){
	const TextCursor prevTextCursor = this->_cursor;
	Line& prevLine = *(this->un_const(prevTextCursor.line_iterator()));
	const size_type from = prevTextCursor.column_index();
	const size_type to = prevTextCursor.line_iterator()->size() - prevTextCursor.column_index();
	this->insert_line_after();
	this->move_down();
	Text::iterator lineIterator = this->un_const(this->_cursor.line_iterator());
	lineIterator->move_append(prevLine, from, to);
	this->saved = false;
}

void TermGui::TextEditor::insert(utf8::Char c){
	if(c == '\n'){
		this->insert_new_line();
	}else if(c == '\b'){
		this->Delete();
	}else if(c == '\t' || !utf8::is_control(c)){
		this->insert_naive(c);
	}
}

void TermGui::TextEditor::move_forward(){
	this->_cursor.move_forward();
	this->move_screen_to_cursor(this->_cursor);
}

void TermGui::TextEditor::move_back(){
	this->_cursor.move_back();
	this->move_screen_to_cursor(this->_cursor);
}

void TermGui::TextEditor::move_up(){
	this->_cursor.move_up();
	this->move_screen_to_cursor(this->_cursor);
}

void TermGui::TextEditor::move_down(){
	this->_cursor.move_down();
	this->move_screen_to_cursor(this->_cursor);
}

void TermGui::TextEditor::move_to_start_of_line() { 
	this->_cursor.move_to_start_of_line();
	this->move_screen_to_cursor(this->_cursor);
}

void TermGui::TextEditor::move_to_start_of_file() {
	this->_cursor.move_to_start_of_file();
	this->move_screen_to_cursor(this->_cursor);
}

void TermGui::TextEditor::move_to_end_of_line() {
	this->_cursor.move_to_end_of_line();
	this->move_screen_to_cursor(this->_cursor);
}

void TermGui::TextEditor::move_to_end_of_file() {
	this->_cursor.move_to_end_of_file();
	this->move_screen_to_cursor(this->_cursor);
}

void TermGui::TextEditor::render(std::string& outputString) const {
	// render file name and saved status
	outputString += Term::cursor_move(this->header_position().y, this->header_position().x);
	outputString += to_string(TermGui::FontStyle::Reversed::On);
	outputString += (this->saved) ? ' ' : '*';
	const std::string filename = this->_filename.filename().empty() ? std::string("%Unnamed") : this->_filename.filename().string();
	if(filename.size() + 1 <= (this->text_width())){
		outputString += filename;
		outputString.append(this->text_width() - (1 + filename.size()), ' ');
	}else{
		outputString.append(filename.begin(), filename.begin() + this->text_width() - 1);
	}
	outputString += to_string(TermGui::FontStyle::Reversed::Off);
	
	// render text field
	size_type screenLineNumber = 0;
	TextCursor renderCursor = this->topScreenLine;
	for(; screenLineNumber < this->text_height() && renderCursor.line_iterator() != this->cend(); (void)++screenLineNumber, (void)renderCursor.move_down_to_start_of_line()){
		outputString += Term::cursor_move(this->text_position().y + screenLineNumber, this->text_position().x);
		
		renderCursor.move_to_screen_column_after(this->screenColumn);
		
		// print styles until start of render
		auto stylesItr = renderCursor.line_iterator()->style_list_cbegin();
		for(; (stylesItr != renderCursor.line_iterator()->style_list_cend()) && (stylesItr->index < renderCursor.column_index()); ++stylesItr){
			stylesItr->render(outputString);
		}
		
		{// append spaces to the start of the line
			const auto spaces = std::max(renderCursor.screen_column() - this->screenColumn, 0);
			outputString.append(spaces, ' ');
		}

		// print characters until the end of the screen has been reached
		const size_type screenColumnEnd = this->screenColumn + this->text_width();
		for(; renderCursor.screen_column() < screenColumnEnd && !renderCursor.is_end_of_line(); renderCursor.move_forward()){
			if (stylesItr != renderCursor.line_iterator()->style_list_cend()) {
				if (stylesItr->index == renderCursor.column_index()) {
					stylesItr->render(outputString);
					++stylesItr;
				}
			}
			
			const auto print_cursor = this->showCursor && renderCursor == this->_cursor;
			if(renderCursor.get_char() == '\t' && print_cursor){
				outputString += to_string(FontStyle::Reversed::On);
				outputString += ' ';
				outputString += to_string(FontStyle::Reversed::Off);
				const auto tabs_to_print = std::min(static_cast<size_type>(this->tabSize - 1), 
													static_cast<size_type>(screenColumnEnd - renderCursor.screen_column()));
				outputString.append(tabs_to_print, ' ');
			}else if(renderCursor.get_char() == '\t'){
				const auto tabs_to_print = std::min(static_cast<size_type>(this->tabSize),
													static_cast<size_type>(screenColumnEnd - renderCursor.screen_column()));
				outputString.append(tabs_to_print, ' ');
			}else if(print_cursor) {
				// print cursor in text
				outputString += to_string(TermGui::FontStyle::Reversed::On);
				outputString += renderCursor.get_char().to_std_string_view();
				outputString += to_string(TermGui::FontStyle::Reversed::Off);
			}else{
				outputString += renderCursor.get_char().to_std_string_view();
			}

			// print styles until the end of the string
			for (; (stylesItr != renderCursor.line_iterator()->style_list_cend()); ++stylesItr) {
				stylesItr->render(outputString);
			}
		}
		
		// print cursor past the end of the line
		if (this->showCursor && renderCursor == this->_cursor) {
			outputString += to_string(FontStyle::Reversed::On);
			outputString += ' ';
			outputString += to_string(FontStyle::Reversed::Off);
			outputString.append(screenColumnEnd - renderCursor.screen_column(), ' ');
		}else{
			// override characters until the end of the provided screen
			outputString.append(screenColumnEnd - renderCursor.screen_column(), ' ');
		}
	}
	
	//override lines until the end of the provided screen
	if(screenLineNumber < this->text_height()){
		outputString += TermGui::to_string(TermGui::fg_color(LimeTheme::green[0], LimeTheme::green[1], LimeTheme::green[2]));
		for(;screenLineNumber < this->text_height(); ++screenLineNumber){
			outputString += Term::cursor_move(this->text_position().y + screenLineNumber, this->text_position().x);
			outputString += '~';
			outputString.append(this->text_width() - 1, ' ');
		}
		outputString += TermGui::to_string(TermGui::default_fg_color());
	}
	
	// render text footer
	std::int32_t column = 0;
	outputString += Term::cursor_move(this->footer_position().y, this->footer_position().x);
	outputString += to_string(TermGui::FontStyle::Reversed::On);
	{	
		if(column + sizeof("Line: ")-1 > this->text_width()) goto BREAK;
		outputString += "Line: ";
		column += sizeof("Line: ")-1;

		{
			const auto number = std::to_string(this->_cursor.line_index());
			if(column + number.size() > this->text_width()) goto BREAK;
			outputString += number;
			column += number.size();
		}

		if(column + sizeof("  Column: ")-1 > this->text_width())  goto BREAK;
		outputString += "  Column: ";
		column += sizeof("  Column: ")-1;
		
		{
			const auto number = std::to_string(this->_cursor.column_index());
			if(column + number.size() > this->text_width()) goto BREAK;
			outputString += number;
			column += number.size();
		}
		
		if(column + sizeof("  UTF-8")-1 > this->text_width()) goto BREAK;
		outputString += "  UTF-8";
		column += sizeof("  UTF-8")-1;
		
		if(column + sizeof(" screen lines:")-1 > this->text_width()) goto BREAK;
		outputString += " screen lines:";
		column += sizeof(" screen lines:")-1;
		
		{
			const auto number = std::to_string(this->topScreenLine.line_index());
			if(column + number.size() > this->text_width()) goto BREAK;
			outputString += number;
			column += number.size();
		}
	}BREAK:

	
	outputString.append(this->text_width() - column, ' ');
	outputString += to_string(TermGui::FontStyle::Reversed::Off);
	
}

bool TermGui::TextEditor::save() {
	if(this->_filename.empty()){
		return false;
	}
	
	std::ofstream file(this->_filename);
	
	if(!file.is_open()) {
		return false;
	}
	
	if(this->empty()){
		this->saved = true;
		return true;
	}
	
	const_iterator iterator = _text.cbegin();
	file << *(iterator++);
	for(; iterator != _text.cend(); ++iterator){
		file << "\n" << *iterator;
	}
	
	this->saved = true;
	return true;
}

bool TermGui::TextEditor::save_as(std::filesystem::path newFilename){
	this->_filename = newFilename;
	return this->save();
}

bool TermGui::TextEditor::open(std::filesystem::path filename){
	std::ifstream file(filename);

	if(!file.is_open()) {
		return false;
	}

	this->clear();
	this->_filename = filename;
	
	while (!file.eof()) {
		char buffer[4 * 1024];
		file.read(buffer, 4 * 1024);
		
		for(size_type i = 0; i != file.gcount(); ++i){
			if(buffer[i] == '\n'){
				this->_text.emplace_back();
			}else if(buffer[i] == '\t' || !std::iscntrl(buffer[i])){
				this->back().append(utf8::Char(buffer[i]));
			}
		}
	}
	this->saved = true;
	return true;
}

void TermGui::TextEditor::Delete(){
	if(!this->_cursor.is_start_of_file()){
		if(this->_cursor.is_start_of_line()){
			// remove the line break
			this->move_up();
			this->move_to_end_of_line();
			const_iterator nextLineIterator = this->_cursor.line_iterator();
			++nextLineIterator;
			this->un_const(this->_cursor.line_iterator())->append(*nextLineIterator);
			this->_text.erase(nextLineIterator);
		}else{
			// erase element inside string
			this->move_back();
			this->un_const(this->_cursor.line_iterator())->erase(this->_cursor.column_index());
		}
		this->saved = false;
	} 
}

void TermGui::TextEditor::erase(){
	if(!this->_cursor.is_end_of_file()){
		if(this->_cursor.is_end_of_line()){
			// erase line break
			const_iterator nextLineIterator = this->_cursor.line_iterator();
			++nextLineIterator;
			this->un_const(this->_cursor.line_iterator())->append(*nextLineIterator);
			this->_text.erase(nextLineIterator);
		}else{
			// erase element inside string
			this->un_const(this->_cursor.line_iterator())->erase(this->_cursor.column_index());
		}
		this->saved = false;
	} 
}

void TermGui::TextEditor::enter() {this->insert_new_line();}

void TermGui::TextEditor::set_screen_width(TermGui::ScreenWidth width) {
	GridCell::set_screen_width(width);
	this->move_screen_to_cursor(this->_cursor);
}

void TermGui::TextEditor::clear() { 
	if(!this->empty()){
		this->_text.clear();
		this->_text.emplace_back();
		this->_cursor = TextCursor(this);
		this->topScreenLine.move_to_start_of_file();
		this->screenColumn = 0;
		this->saved = false;
	}
}

bool TermGui::operator==(const TermGui::TextEditor& lhs, const TermGui::TextEditor& rhs){
	if(lhs.size() == rhs.size()){
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



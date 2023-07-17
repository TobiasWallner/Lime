
// C++ std
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

// utf8 string 
#include <utf8_char.hpp>
#include <utf8_string.hpp>

// project
#include "TextEditor.hpp"
#include "ColorThemes.hpp"

TermGui::TextEditor::TextEditor(std::filesystem::path filename, ScreenPosition screenPosition, ScreenWidth screenWidth) :
		_text(1),
		_cursor(this),
		_filename(filename),
		screenPosition(screenPosition),
		screenWidth(screenWidth),
		renderLineStart(0),
		renderLineStartItr(this->_text.begin()),
		renderColumnStart(0){}

void TermGui::TextEditor::tab_size(std::int32_t newTabSize){
	this->tabSize = newTabSize;
	this->_cursor.update_tab_size();
}

void TermGui::TextEditor::show_cursor(bool logic) {this->showCursor = logic;}

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
}

void TermGui::TextEditor::move_back(){
	this->_cursor.move_back();
}

void TermGui::TextEditor::move_up(){
	this->_cursor.move_up();
}

void TermGui::TextEditor::move_down(){
	this->_cursor.move_down();
}

void TermGui::TextEditor::move_to_start_of_line() { 
	this->_cursor.move_to_start_of_line();
}

void TermGui::TextEditor::move_to_start_of_file() {
	this->_cursor.move_to_start_of_file();
}

void TermGui::TextEditor::move_to_end_of_line() {
	this->_cursor.move_to_end_of_line();
}

void TermGui::TextEditor::move_to_end_of_file() {
	this->_cursor.move_to_end_of_file();
}

void TermGui::TextEditor::render(std::string& outputString) const {
	TextEditor::const_iterator lineItr = this->renderLineStartItr;
	size_type lineNumber = this->renderLineStart;
	size_type screenLineNumber = 0;
	for(; screenLineNumber < this->screenWidth.y && lineItr != this->cend(); ++lineItr, (void)++lineNumber, (void)++screenLineNumber){
		outputString += Term::cursor_move(this->screenPosition.y + screenLineNumber, this->screenPosition.x);
		
		TextCursor renderCursor(this, lineItr, lineNumber);
		renderCursor.move_to_screen_column_after(this->renderColumnStart);
		
		// print styles until start of render
		auto stylesItr = lineItr->style_list_cbegin();
		for(; (stylesItr != lineItr->style_list_cend()) && (stylesItr->index < renderCursor.column_index()); ++stylesItr){
			stylesItr->render(outputString);
		}
		
		outputString.append(renderCursor.screen_column() - this->renderColumnStart, ' ');
		
		// print characters until the end of the screen has been reached
		const size_type screenColumnEnd = this->renderColumnStart + this->screenWidth.x;
		for(; renderCursor.screen_column() < screenColumnEnd && !renderCursor.is_end_of_line(); renderCursor.move_forward()){
			if (stylesItr != lineItr->style_list_cend()) {
				if (stylesItr->index == renderCursor.column_index()) {
					stylesItr->render(outputString);
					++stylesItr;
				}
			}
			
			const auto print_cursor = this->showCursor && renderCursor == this->_cursor;
			if(renderCursor.get_char() == '\t' && print_cursor){
				outputString += to_string(FontStyle::Reversed::ON);
				outputString += ' ';
				outputString += to_string(FontStyle::Reversed::OFF);
				const auto tabs_to_print = std::min(static_cast<size_type>(this->tabSize - 1), 
													static_cast<size_type>(screenColumnEnd - renderCursor.screen_column()));
				outputString.append(tabs_to_print, ' ');
			}else if(renderCursor.get_char() == '\t'){
				const auto tabs_to_print = std::min(static_cast<size_type>(this->tabSize),
													static_cast<size_type>(screenColumnEnd - renderCursor.screen_column()));
				outputString.append(tabs_to_print, ' ');
			}else if(print_cursor) {
				// print cursor in text
				outputString += to_string(TermGui::FontStyle::Reversed::ON);
				outputString += renderCursor.get_char().to_std_string_view();
				outputString += to_string(TermGui::FontStyle::Reversed::OFF);
			}else{
				outputString += renderCursor.get_char().to_std_string_view();
			}
		}
		
		// print cursor past the end of the line
		const auto print_cursor = this->showCursor && renderCursor == this->_cursor;
		if (print_cursor && this->_cursor.is_end_of_line()) {
			outputString += to_string(FontStyle::Reversed::ON);
			outputString += ' ';
			outputString += to_string(FontStyle::Reversed::OFF);
			renderCursor.move_forward();
		}
		
		// print styles until the end of the string
		for(; (stylesItr != lineItr->style_list_cend()); ++stylesItr){
			stylesItr->render(outputString);
		}
		
		// override characters until the end of the provided screen
		outputString.append(screenColumnEnd - renderCursor.screen_column(), ' ');
	}
	//override lines until the end of the provided screen
	if(screenLineNumber < this->screenWidth.y){
		outputString += TermGui::to_string(TermGui::fg_color(LimeTheme::green[0], LimeTheme::green[1], LimeTheme::green[2]));
		for(;screenLineNumber < this->screenWidth.y; ++screenLineNumber){
			outputString += Term::cursor_move(this->screenPosition.y + screenLineNumber, this->screenPosition.x);
			outputString += '~';
			outputString.append(this->screenWidth.x - 1, ' ');
		}
		outputString += TermGui::to_string(TermGui::default_fg_color());
	}
}

bool TermGui::TextEditor::save() const{
	if(this->_filename.empty()){
		return false;
	}
	
	std::ofstream file(this->_filename);
	
	if(!file.is_open()) {
		return false;
	}
	
	if(this->empty()){
		return true;
	}
	
	const_iterator iterator = _text.cbegin();
	file << *(iterator++);
	for(; iterator != _text.cend(); ++iterator){
		file << "\n" << *iterator;
	}
	
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

void TermGui::TextEditor::clear() { 
	if(!this->empty()){
		this->_text.clear();
		this->_text.emplace_back();
		this->_cursor = TextCursor(this);
		this->renderLineStart = 0;
		this->renderLineStartItr = this->_text.begin();
		this->renderColumnStart = 0;
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



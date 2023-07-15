
// C++ std
#include <fstream>
#include <iostream>
#include <algorithm>

// utf8 string 
#include <utf8_char.hpp>
#include <utf8_string.hpp>

// project
#include "TextEditor.hpp"
#include "ColorThemes.hpp"

TermGui::TextEditor::TextEditor(ScreenPosition screenPosition, ScreenWidth screenWidth) :
		_text(1),
		_cursor(&this->_text),
		screenPosition(screenPosition),
		screenWidth(screenWidth),
		renderLineStart(0),
		renderLineStartItr(this->_text.begin()),
		renderColumnStart(0){}


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
		
		TextCursor renderCursor(&this->_text, lineItr, lineNumber, 0, 0, this->tabSize);
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
				const auto tabs_to_print = std::min(static_cast<size_type>(4), 
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
	if(this->_cursor.is_start_of_file()){
		// nothing to do
	}else if(this->_cursor.is_start_of_line()){
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
}

void TermGui::TextEditor::erase(){
	if(this->_cursor.is_end_of_file()){
		// nothing to do
	}else if(this->_cursor.is_end_of_line()){
		// erase line break
		const_iterator nextLineIterator = this->_cursor.line_iterator();
		++nextLineIterator;
		this->un_const(this->_cursor.line_iterator())->append(*nextLineIterator);
		this->_text.erase(nextLineIterator);
	}else{
		// erase element inside string
		this->un_const(this->_cursor.line_iterator())->erase(this->_cursor.column_index());
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
	this->_text.clear();
	this->_text.emplace_back();
	this->_cursor = TextCursor(&this->_text);
	this->renderLineStart = 0;
	this->renderLineStartItr = this->_text.begin();
	this->renderColumnStart = 0;
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



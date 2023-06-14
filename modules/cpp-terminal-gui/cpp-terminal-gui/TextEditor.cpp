

// utf8 string 
#include <utf8_char.hpp>
#include <utf8_string.hpp>

#include "TextEditor.hpp"

#include <fstream>
#include <iostream>

TermGui::TextEditor::TextEditor(){
	init();
}

void TermGui::TextEditor::init(){
	this->_text.emplace_back();
	this->_cursor.lineNumber = 0;
	this->_cursor.columnNumber = 0;
	this->_cursor.lineIterator = this->_text.begin();
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
	auto prevCursor = this->_cursor;
	this->insert_line_after();
	++this->_cursor.lineNumber;
	++this->_cursor.lineIterator;
	this->_cursor.columnNumber = 0;
	this->lineItr()->move_append(*(prevCursor.lineIterator), prevCursor.columnNumber, prevCursor.lineIterator->size() - prevCursor.columnNumber);
}

void TermGui::TextEditor::insert(utf8::Char c){
	if(c == '\n'){
		this->insert_new_line();
	}else if(c == '\b'){
		this->Delete();
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
		++this->_cursor.lineNumber;
		++this->_cursor.lineIterator;
		this->_cursor.columnNumber = 0;
	}else{
		++this->_cursor.columnNumber;
	}
}

void TermGui::TextEditor::move_back(){
	if(this->is_start_of_file()){
		return;
	}else if(this->is_start_of_line()){
		--this->_cursor.lineNumber;
		--this->_cursor.lineIterator;
		this->_cursor.columnNumber = this->_cursor.lineIterator->size();
	}else{
		--this->_cursor.columnNumber;
	}
}

void TermGui::TextEditor::move_up(){
	if(this->is_start_of_file()){
		return;
	}else if(this->is_first_line()){
		this->move_to_start_of_line();
	}else{
		--this->_cursor.lineNumber;
		--this->_cursor.lineIterator;
		this->_cursor.columnNumber = std::min(this->_cursor.columnNumber, this->_cursor.lineIterator->size());
	}
}

void TermGui::TextEditor::move_down(){
	if(this->is_end_of_file()){
		return;
	}else if(this->is_last_line()){
		this->move_to_end_of_line();
	}else{
		++this->_cursor.lineNumber;
		++this->_cursor.lineIterator;
		this->_cursor.columnNumber = std::min(this->_cursor.columnNumber, this->_cursor.lineIterator->size());
	}
}

void TermGui::TextEditor::render(std::string& outputString) const {
	for(auto itr = this->begin(); itr != this->end(); ++itr){
		if(itr != this->begin()){
			outputString += '\n';	
		}
		if(this->showCursor && itr == this->_cursor.lineIterator){
			TermGui::TextEditor::Line lineCopy = *itr;
			if(this->is_end_of_line()){
				lineCopy += ' ';
			}
			lineCopy.add_override(TermGui::FontStyle::Reversed::ON, this->cursor_column());
			lineCopy.add_override(TermGui::FontStyle::Reversed::OFF, this->cursor_column() + 1);
			lineCopy.render(outputString);
		}else{
			itr->render(outputString);
		}
	}
}

bool TermGui::TextEditor::append_file(const std::filesystem::path& path){
	if(!std::filesystem::is_regular_file(path)) {return false;}
	std::ifstream file(path);
	return append_file(file);
}

bool TermGui::TextEditor::read_file(const std::filesystem::path& path){
	this->clear();
	return append_file(path);
}

bool TermGui::TextEditor::append_file(std::ifstream& file){
	char buffer[4*1024];

	if(file.is_open()){
		while(!file.eof()){
			file.read (buffer, 4*1024);
			this->insert(buffer, file.gcount());
		}
		this->move_to_start_of_file();
		return true;
	}
	return false;
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
		auto nextLineIterator = this->_cursor.lineIterator;
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
		auto nextLineIterator = this->_cursor.lineIterator;
		++nextLineIterator;
		this->_cursor.lineIterator->append(*nextLineIterator);
		this->_text.erase(nextLineIterator);
	}else{
		// erase element inside string
		this->_cursor.lineIterator->erase(this->_cursor.columnNumber);
	}
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
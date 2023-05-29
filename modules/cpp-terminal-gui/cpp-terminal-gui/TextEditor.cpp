

// utf8 string 
#include <utf8_char.hpp>
#include <utf8_string.hpp>

#include "TextEditor.hpp"

TermGui::TextEditor::TextEditor(){
	this->_text.emplace_back();
	this->_cursor.lineNumber = 0;
	this->_cursor.columnNumber = 0;
	this->_cursor.lineIterator = this->_text.begin();
}

void TermGui::TextEditor::insert(utf8::Char c){
	auto& line = *this->_cursor.lineIterator;
	const auto& index = this->_cursor.columnNumber;
	line.insert(index, c);
	this->move_right();
}

bool TermGui::TextEditor::insert(const char* str){
	utf8::Char c;
	while(*str != '\0'){
		auto next_str = c.assign(str);
		if(str == next_str){
			//error: encountered a non-utf8 character 
			return false;
		}else{
			this->insert(c);
			str = next_str;
		}
	}
	return true;
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


void TermGui::TextEditor::move_right(){
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

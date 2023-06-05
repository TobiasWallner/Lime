

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

void TermGui::TextEditor::insert(utf8::Char c){
	if(c == '\n'){
		auto prevCursor = this->_cursor;
		this->insert_new_line();
		this->lineItr()->move_append(*(prevCursor.lineIterator), prevCursor.columnNumber, prevCursor.lineIterator->size() - prevCursor.columnNumber);
	}else{
		auto& line = *this->_cursor.lineIterator;
		const auto& index = this->_cursor.columnNumber;
		line.insert(index, c);
		this->move_forward();	
	}	
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
	if(this->empty()){
		return;
	}
	auto itr = this->begin();
	const auto end = this->end();
	
	// do first iteration without newline outside of loop
	
	itr->render(outputString);
	++itr;
	for(; itr != end; ++itr){
		outputString += '\n';
		itr->render(outputString);
	}
}
	

		
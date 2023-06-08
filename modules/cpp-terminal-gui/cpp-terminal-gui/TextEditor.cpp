

// utf8 string 
#include <utf8_char.hpp>
#include <utf8_string.hpp>

#include "TextEditor.hpp"

#include <fstream>
#include <iostream>

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

TermGui::TextEditor& TermGui::TextEditor::insert_naive(utf8::Char character){
	this->_cursor.lineIterator->insert(this->_cursor.columnNumber, character);
	this->move_forward();
	return *this;
}

TermGui::TextEditor& TermGui::TextEditor::insert_new_line(){
	auto prevCursor = this->_cursor;
	this->insert_line_after();
	++this->_cursor.lineNumber;
	++this->_cursor.lineIterator;
	this->_cursor.columnNumber = 0;
	this->lineItr()->move_append(*(prevCursor.lineIterator), prevCursor.columnNumber, prevCursor.lineIterator->size() - prevCursor.columnNumber);
	return *this;
}

TermGui::TextEditor& TermGui::TextEditor::insert(utf8::Char c){
	if(c == '\n'){
		return this->insert_new_line();
	}else if(c == '\b'){
		return this->erase();
	}else{
		return this->insert_naive(c);
	}	
}

bool TermGui::TextEditor::insert(const char* str){
	while(*str != '\0'){
		utf8::Char c;
		const char* next = c.assign(str);
		if(next == str){
			// no characters have been read -> error
			return false;
		}
		this->insert(c);
		str = next;
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
	

bool TermGui::TextEditor::read_file(const std::filesystem::path& path){
	std::streampos size;
	char *buffer;

	if(!std::filesystem::is_regular_file(path)) {return false;}
	std::ifstream file(path, std::ios::in|std::ios::binary|std::ios::ate);

	if(file.is_open()){
		size = file.tellg();
		buffer = new char[size];
		file.seekg (0, std::ios::beg);
    	file.read (buffer, size);
		_text.insert(buffer);
    	file.close();
		this->move_to_start_of_file();
		delete [] buffer;
		return true;
	}
	return false;
}

bool TermGui::TextEditor::read_file(std::ifstream& stream){
	std::streampos size;
	char *buffer;

	if(stream.is_open()){
		stream.seekg(0, std::ios::end);
		size = stream.tellg();
		buffer = new char[size];
		stream.seekg (0, std::ios::beg);
		stream.read (buffer, size);
		_text.insert(buffer);
		stream.close();
		this->move_to_start_of_file();
		delete [] buffer;
		return true;
	}
	return false;
}

bool TermGui::TextEditor::write_file(const std::filesystem::path& path){
	if(std::filesystem::is_regular_file(path) == 0) {return false;}
	std::ofstream file(path);
	if(file.is_open()){
        for(Line line : _text){
            file << line << "\n";
        }
        file.close();
		return true;
    }
	return false;
}

bool TermGui::TextEditor::write_file(std::ifstream& stream){
	if(stream.is_open()){
        for(Line line : _text){
            stream << line << "\n";
        }
        stream.close();
		return true;
    }
	return false;
}

TermGui::TextEditor& TermGui::TextEditor::erase(){
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
	return *this;
}
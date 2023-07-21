// C++ std
#include <fstream>
#include <algorithm>

// utf8 string 
#include <utf8_char.hpp>
#include <utf8_string.hpp>

// project
#include "ColorString.hpp"
#include "TextCursor.hpp"
#include "TextEditor.hpp"

TermGui::TextCursor::TextCursor(const TextEditor* pEditor,
				Text::const_iterator lineIterator,
				std::int32_t lineIndex,
				std::int32_t columnIndex,
				std::int32_t screenColumn) :
	pEditor(pEditor),
	lineIterator(lineIterator),
	lineIndex(lineIndex),
	columnIndex(columnIndex),
	screenColumn(screenColumn)
	{}

TermGui::TextCursor::TextCursor(const TextEditor* pEditor) : TextCursor(pEditor, pEditor->begin(), 0){}

bool TermGui::TextCursor::is_first_line() const {return this->lineIterator == this->pEditor->begin();}
bool TermGui::TextCursor::is_last_line() const {return this->lineIterator == --this->pEditor->end();}
bool TermGui::TextCursor::is_start_of_file() const {return this->is_first_line() && this->is_start_of_line();}
bool TermGui::TextCursor::is_end_of_file() const {return this->is_last_line() && this->is_end_of_line();}

TermGui::TextCursor& TermGui::TextCursor::move_forward(){
	if(this->is_end_of_file()){
		return *this;
	}else if(this->is_end_of_line()){
		this->move_down_to_start_of_line();
	}else{
		const auto tabSize = this->pEditor->tab_size();
		this->screenColumn += (this->get_char() == '\t') ? tabSize : 1;
		++this->columnIndex;
	}
	return *this;
}

TermGui::TextCursor& TermGui::TextCursor::move_back(){
	if(this->is_start_of_file()){
		return *this;
	}else if(this->is_start_of_line()){
		this->move_up_to_start_of_line().move_to_end_of_line();
	}else{
		--this->columnIndex;
		const auto tabSize = this->pEditor->tab_size();
		this->screenColumn -= (this->get_char() == '\t') ? tabSize : 1;
	}
	return *this;
}

TermGui::TextCursor& TermGui::TextCursor::move_up(std::int32_t n){
	if(this->is_start_of_file()){
		return *this;
	}else if(this->is_first_line()){
		this->move_to_start_of_line();
	}else{
		const auto prevScreenColumn = this->screenColumn;
		for(std::int32_t i = 0; i < n && !this->is_first_line(); ++i){
			move_up_to_start_of_line();
		}
		move_to_screen_column_before(prevScreenColumn);
	}
	return *this;
}

TermGui::TextCursor& TermGui::TextCursor::move_down(std::int32_t n){
	if(this->is_end_of_file()){
		return *this;
	}else if(this->is_last_line()){
		this->move_to_end_of_line();
	}else{
		const auto prevScreenColumn = this->screenColumn;
		for(std::int32_t i = 0; (i < n) && !this->is_last_line(); ++i){
			move_down_to_start_of_line();
		}
		move_to_screen_column_before(prevScreenColumn);
	}
	return *this;
}

TermGui::TextCursor& TermGui::TextCursor::move_to_screen_column_before(std::int32_t wantedScreenColumn){
	// move foreward
	const auto tabSize = this->pEditor->tab_size();
	while(!this->is_end_of_line() && this->screenColumn < wantedScreenColumn){	
		this->screenColumn += (this->get_char() == '\t') ? tabSize : 1;
		++this->columnIndex;
	}
	
	// move backward
	while(!this->is_start_of_line() && this->screenColumn > wantedScreenColumn){	
		--this->columnIndex;
		this->screenColumn -= (this->get_char() == '\t') ? tabSize : 1;
	}
	return *this;
}

TermGui::TextCursor& TermGui::TextCursor::move_to_screen_column_after(std::int32_t wantedScreenColumn){
	// move backward
	const auto tabSize = this->pEditor->tab_size();
	while(!this->is_start_of_line() && this->screenColumn > wantedScreenColumn){	
		--this->columnIndex;
		this->screenColumn -= (this->get_char() == '\t') ? tabSize : 1;
	}
	
	// move foreward
	while(!this->is_end_of_line() && this->screenColumn < wantedScreenColumn){	
		this->screenColumn += (this->get_char() == '\t') ? tabSize : 1;
		++this->columnIndex;
	}
	return *this;
}

TermGui::TextCursor& TermGui::TextCursor::move_to_end_of_line(){
	const auto tabSize = this->pEditor->tab_size();
	while(!this->is_end_of_line()){
		this->screenColumn += (this->get_char() == '\t') ? tabSize : 1;
		++this->columnIndex;
	}
	return *this;
}

TermGui::TextCursor& TermGui::TextCursor::move_to_start_of_file(){
	this->lineIterator = this->pEditor->begin(); 
	this->lineIndex = 0; 
	this->columnIndex = 0; 
	this->screenColumn = 0;
	return *this;
}

TermGui::TextCursor& TermGui::TextCursor::move_to_end_of_file(){
	// TODO: change once line wrapping becomes a thing
	this->lineIterator = --this->pEditor->begin();
	this->lineIndex = this->pEditor->size() - 1;
	this->columnIndex = 0;
	this->screenColumn = 0;
	this->move_to_end_of_line();
	return *this;
}

TermGui::TextCursor& TermGui::TextCursor::update_tab_size(){
	this->screenColumn = 0;
	const auto tabSize = this->pEditor->tab_size();
	for(std::int32_t i = 0; i < this->columnIndex; ++i){
		this->screenColumn += (this->lineIterator->at(i) == '\t') ? tabSize : 1;
	}
	return *this;
}
// C++ std
#include <fstream>
#include <algorithm>

// utf8 string 
#include <utf8_char.hpp>
#include <utf8_string.hpp>

// project
#include "ColorString.hpp"
#include "TextCursor.hpp"

TermGui::TextCursor::TextCursor(const Text* pText,
				Text::const_iterator lineIterator,
				std::int32_t lineIndex,
				std::int32_t columnIndex,
				std::int32_t screenColumn,
				std::int32_t tabSize) :
	pText(pText),
	lineIterator(lineIterator),
	lineIndex(lineIndex),
	columnIndex(columnIndex),
	screenColumn(screenColumn),
	tabSize(tabSize)
	{}



void TermGui::TextCursor::move_forward(){
	if(this->is_end_of_file()){
		return;
	}else if(this->is_end_of_line()){
		this->move_down_to_start_of_line();
	}else{
		this->screenColumn += (this->get_char() == '\t') ? this->tabSize : 1;
		++this->columnIndex;
	}
}

void TermGui::TextCursor::move_back(){
	if(this->is_start_of_file()){
		return;
	}else if(this->is_start_of_line()){
		this->move_up_to_start_of_line();
	}else{
		--this->columnIndex;
		this->screenColumn -= (this->get_char() == '\t') ? this->tabSize : 1;
	}
}

void TermGui::TextCursor::move_up_to_start_of_line(){
	--this->lineIterator;
	--this->lineIndex;
	this->columnIndex = 0;
	this->screenColumn = 0;
}

void TermGui::TextCursor::move_down_to_start_of_line(){
	++this->lineIterator;
	++this->lineIndex;
	this->columnIndex = 0;
	this->screenColumn = 0;
}

void TermGui::TextCursor::move_up(std::int32_t n){
	if(this->is_start_of_file()){
		return;
	}else if(this->is_first_line()){
		this->move_to_start_of_line();
	}else{
		const auto prevScreenColumn = this->screenColumn;
		for(std::int32_t i = 0; i < n && !this->is_first_line(); ++i){
			move_up_to_start_of_line();
		}
		move_to_screen_column_before(prevScreenColumn);
	}
}

void TermGui::TextCursor::move_down(std::int32_t n){
	if(this->is_end_of_file()){
		return;
	}else if(this->is_last_line()){
		this->move_to_end_of_line();
	}else{
		const auto prevScreenColumn = this->screenColumn;
		for(std::int32_t i = 0; (i < n) && !this->is_last_line(); ++i){
			move_down_to_start_of_line();
		}
		move_to_screen_column_before(prevScreenColumn);
	}
}

void TermGui::TextCursor::move_to_screen_column_before(std::int32_t wantedScreenColumn){
	// move foreward
	while(!this->is_end_of_line() && this->screenColumn < wantedScreenColumn){	
		this->screenColumn += (this->get_char() == '\t') ? this->tabSize : 1;
		++this->columnIndex;
	}
	
	// move backward
	while(!this->is_start_of_line() && this->screenColumn > wantedScreenColumn){	
		--this->columnIndex;
		this->screenColumn -= (this->get_char() == '\t') ? this->tabSize : 1;
	}
}

void TermGui::TextCursor::move_to_screen_column_after(std::int32_t wantedScreenColumn){
	// move backward
	while(!this->is_start_of_line() && this->screenColumn > wantedScreenColumn){	
		--this->columnIndex;
		this->screenColumn -= (this->get_char() == '\t') ? this->tabSize : 1;
	}
	
	// move foreward
	while(!this->is_end_of_line() && this->screenColumn < wantedScreenColumn){	
		this->screenColumn += (this->get_char() == '\t') ? this->tabSize : 1;
		++this->columnIndex;
	}
}



void TermGui::TextCursor::move_to_start_of_line(){
	this->lineIndex = 0;
	this->columnIndex = 0;
	this->screenColumn = 0;
}

void TermGui::TextCursor::move_to_end_of_line(){
	while(!this->is_end_of_line()){
		this->screenColumn += (this->get_char() == '\t') ? this->tabSize : 1;
		++this->columnIndex;
	}
}

void TermGui::TextCursor::move_to_start_of_file(){
	this->lineIterator = this->pText->begin();
	this->lineIndex = 0;
	this->columnIndex = 0;
	this->screenColumn = 0;
}

void TermGui::TextCursor::move_to_end_of_file(){
	// TODO: change once line wrapping becomes a thing
	this->lineIterator = --this->pText->begin();
	this->lineIndex = this->pText->size();
	this->columnIndex = 0;
	this->screenColumn = 0;
	this->move_to_end_of_line();
}



#pragma once

#include "Label.hpp"

// cpp-terminal
#include <cpp-terminal/cursor.hpp>


void Label::render(std::string& output_string) const{
	Term::cursor_right(output_string, this->_position->column());
	Term::cursor_down(output_string, this->_position->row());
	output_string.append(this->_string);
}
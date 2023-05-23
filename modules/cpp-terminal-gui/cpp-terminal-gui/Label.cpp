

#include "Label.hpp"

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

namespace TermGui{

void Label::render(std::string& output_string) const {
	Term::cursor_right(output_string, this->_position.column());
	Term::cursor_down(output_string, this->_position.row());
	this->_string.append_to(output_string);
}
	
}

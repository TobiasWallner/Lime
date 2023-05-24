

#include "Label.hpp"

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

namespace TermGui{

void Label::render(std::string& outputString) const {
	outputString += Term::cursor_right(this->_position.column());
	outputString += Term::cursor_down(this->_position.row());
}
	
}

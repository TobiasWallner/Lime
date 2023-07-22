
#include "Label.hpp"


void TermGui::Label::render(std::string& outputString) const {
	auto string = this->string_cbegin();
	auto styleItr = this->style_list_cbegin();
	const auto styleEnd = this->style_list_cend();
	
	size_type index = 0;
	size_type columnNumber = 0;
	size_type lineNumber = 0;
	outputString += Term::cursor_move(this->screenPosition.y + lineNumber, this->screenPosition.x);
	for(;index < this->size() && lineNumber < this->screenWidth.y; (void)++index, (void)++columnNumber){
		// render styles
		if(styleItr != styleEnd){
			if(styleItr->index == index){
				styleItr->render(outputString);
				++styleItr;
			}
		}

		if (columnNumber < this->screenWidth.x) {
			// print characters that can be seen on the screen
			if (string[index] == '\n') {
				outputString.append(this->screenWidth.x - columnNumber, ' '); // clear after end of line
				++lineNumber;
				columnNumber = 0;
				outputString += Term::cursor_move(this->screenPosition.y + lineNumber, this->screenPosition.x);
			}else if (string[index] == '\t') {
				const auto tabs_to_print = std::min(4ULL, this->screenWidth.x - columnNumber);
				outputString.append(tabs_to_print, ' ');
				columnNumber += tabs_to_print - 1;
			}else{
				outputString += string[index].to_std_string_view();
			}
		}
		else if (string[index] == '\n') {
			// line break that cannot be seen on the screen
			++lineNumber;
			columnNumber = 0;
			outputString += Term::cursor_move(this->screenPosition.y + lineNumber, this->screenPosition.x);
		}
	}

	// end last line
	if (lineNumber < this->screenWidth.y) {
		outputString.append(this->screenWidth.x - columnNumber, ' ');
		++lineNumber;
	}

	// there could be one last style (probably for resetting styles) after the last character
	while(styleItr != styleEnd){
		styleItr->render(outputString);
		++styleItr;
	}

	// clear until end of screen
	for(; lineNumber < this->screenWidth.y; ++lineNumber){
		outputString += Term::cursor_move(this->screenPosition.y + lineNumber, this->screenPosition.x);
		outputString.append(this->screenWidth.x, ' ');
	}
}


void TermGui::Label::set_screen_position(TermGui::ScreenPosition position){
	this->screenPosition = position;
}


TermGui::ScreenPosition TermGui::Label::get_screen_position() const {
	return this->screenPosition;
}


void TermGui::Label::set_screen_width(TermGui::ScreenWidth width){
	this->screenWidth = width;
}


TermGui::ScreenWidth TermGui::Label::get_screen_width() const {
	return this->screenWidth;
}
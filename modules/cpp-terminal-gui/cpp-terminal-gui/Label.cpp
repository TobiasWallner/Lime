
#include "Label.hpp"


void TermGui::Label::render(std::string& outputString) const {
	auto string = this->string_cbegin();
	auto styleItr = style_list_cbegin();
	const auto styleEnd = style_list_cend();
	
	size_type index = 0;
	size_type columnNumber = 0;
	size_type lineNumber = 0;
	outputString += Term::cursor_move(this->screenPosition.y + lineNumber, this->screenPosition.x);
	for(;index < this->size() && lineNumber < this->screenWidth.y; (void)++index, (void)++columnNumber){
		if(styleItr != styleEnd){
			if(styleItr->index == index){
				styleItr->render(outputString);
				++styleItr;
			}
		}
		if(string[index] == '\t'){
			const auto tabs_to_print = std::min(4ULL, this->screenWidth.x - columnNumber);
			outputString.append(tabs_to_print, ' ');
			columnNumber += tabs_to_print-1;
		}else if(string[index] == '\n'){
			outputString.append(this->screenWidth.x - columnNumber, ' '); // clear after end of line
			++lineNumber;
			columnNumber = 0;
			outputString += Term::cursor_move(this->screenPosition.y + lineNumber, this->screenPosition.x);
		}else if(columnNumber < this->screenWidth.x){
			outputString += string[index].to_std_string_view();
		}
	}
	// there could be one last style (probably for resetting styles) after the last character
	if(styleItr != styleEnd){
		if(styleItr->index == index){
			styleItr->render(outputString);
			++styleItr;
		}
	}

	// clear until end of screen
	for(; lineNumber < this->screenWidth.y; ++lineNumber){
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
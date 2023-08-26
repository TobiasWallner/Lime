
#include "Label.hpp"


void TermGui::Label::render(std::string& outputString) const {
	auto string = this->_string.string_cbegin();
	auto styleItr = this->_string.style_list_cbegin();
	const auto styleEnd = this->_string.style_list_cend();
	
	const auto screenPosition = GridCell::get_screen_position();
	const auto screenWidth = GridCell::get_screen_width();

	size_type index = 0;
	size_type columnNumber = 0;
	size_type lineNumber = 0;
	outputString += Term::cursor_move(screenPosition.y + lineNumber, screenPosition.x);
	for(;index < this->size() && lineNumber < screenWidth.y; (void)++index, (void)++columnNumber){
		// render styles
		if(styleItr != styleEnd){
			if(styleItr->index == index){
				styleItr->render(outputString);
				++styleItr;
			}
		}

		if (columnNumber < screenWidth.x) {
			// print characters that can be seen on the screen
			if (string[index] == '\n') {
				outputString.append(screenWidth.x - columnNumber, ' '); // clear after end of line
				++lineNumber;
				columnNumber = 0;
				outputString += Term::cursor_move(screenPosition.y + lineNumber, screenPosition.x);
			}else if (string[index] == '\t') {
				const auto tabs_to_print = std::min(4ULL, screenWidth.x - columnNumber);
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
			outputString += Term::cursor_move(screenPosition.y + lineNumber, screenPosition.x);
		}
	}

	// end last line
	if (lineNumber < screenWidth.y) {
		outputString.append(screenWidth.x - columnNumber, ' ');
		++lineNumber;
	}

	// there could be one last style (probably for resetting styles) after the last character
	while(styleItr != styleEnd){
		styleItr->render(outputString);
		++styleItr;
	}

	// clear until end of screen
	for(; lineNumber < screenWidth.y; ++lineNumber){
		outputString += Term::cursor_move(screenPosition.y + lineNumber, screenPosition.x);
		outputString.append(screenWidth.x, ' ');
	}
}

void TermGui::Label::update_on_append(const char* first){
	TermGui::ScreenWidth targetWidth = GridCell::get_target_width();
	if (*first == '\0') {
		return;
	}
	if(targetWidth.y == 0) {
		++targetWidth.y;
	}
	for(; *first != '\0'; ++first){
		if(*first == '\n'){
			++targetWidth.y;
			targetWidth.x = std::max(targetWidth.x, this->insertLineWidth);
			this->insertLineWidth = 0;
		}else if(*first == '\t'){
			this->insertLineWidth += this->tabsize;
		}else{
			++this->insertLineWidth;
		}
	}
	targetWidth.x = std::max(targetWidth.x, this->insertLineWidth);
	GridCell::set_target_width(targetWidth);
}

void TermGui::Label::update_on_assign(const char* first){
	this->insertLineWidth = 0;
	TermGui::ScreenWidth targetWidth = TermGui::ScreenWidth{0,0};
	if (*first != '\0') {
		++targetWidth.y;
	}
	for(; *first != '\0'; ++first){
		if(*first == '\n'){
			++targetWidth.y;
			targetWidth.x = std::max(targetWidth.x, this->insertLineWidth);
			this->insertLineWidth = 0;
		}else if(*first == '\t'){
			this->insertLineWidth += this->tabsize;
		}else{
			++this->insertLineWidth;
		}
	}
	targetWidth.x = std::max(targetWidth.x, this->insertLineWidth);
	GridCell::set_target_width(targetWidth);
}
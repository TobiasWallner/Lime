
#include "CommandLine.hpp"
#include "FontStyle.hpp"


void TermGui::CommandLine::render(std::string& outputString) const {
	outputString << ': ';
	
	if(this->showCursor()){	
		if(this->is_end_of_line()){
			outputString << this->commandString << TermGui::FontStyle::Reversed::ON << ' ' << TermGui::FontStyle::Reversed::OFF;
		}else{
			auto itr = this->commandString.cbegin();
			const auto cursorItr = this->commandString.cbegin() + this->cursorIndex;
			const auto endItr = this->commandString.cend();
			
			for(; itr != cursorItr; ++itr){
				outputString << *itr;
			}
			
			outputString << TermGui::FontStyle::Reversed::ON << *(itr++) << TermGui::FontStyle::Reversed::OFF;
			
			for(; itr != endItr; ++itr){
				outputString << *itr;
			}
		}
	}else{
		outputString << this->commandString
	}
}


#include "ColorString.hpp"

TermGui::ColorString& TermGui::ColorString::append(const TermGui::ColorString& other){
	const auto string_size = this->_string.size();
	this->_string.append(other._string);
	for(const auto& elem : other._commands){
		this->_commands.insert(elem.commands, string_size + elem.index);
	}
	return *this;
}

TermGui::ColorString& TermGui::ColorString::append(TermGui::ColorString&& other){
	const auto string_size = this->_string.size();
	this->_string.append(other._string);
	for(auto& elem : other._commands){
		this->_commands.insert(std::move(elem.commands), string_size + elem.index);
	}
	return *this;
}

void TermGui::ColorString::render(std::string& outputString) const {
	auto commandItr = this->_commands.cbegin(); // start of the commands
	const auto commandEnd = this->_commands.cend(); // end of the commands
	
	auto indexToHalt = 0;
	
	auto stringItr = this->_string.cbegin(); // start of the string
	const auto stringEnd = this->_string.cend(); // end of the string
	auto stringHalt = this->_string.cbegin(); // iterator at which to print a command
	
	// for loop for printing as long as commands are mixed into the string
	for(; commandItr != commandEnd; ++commandItr){
		const auto ci = commandItr->index;
		const auto diff = ci - indexToHalt;
		stringHalt += diff;
		indexToHalt = commandItr->index;
		for(;stringItr != stringHalt; ++stringItr){
			outputString += stringItr->to_std_string_view();
		}
		commandItr->render(outputString);
	}
	
	// printing the rest of the string without commands
	for(;stringItr != stringEnd; ++stringItr){
		outputString += stringItr->to_std_string_view();
	}
}
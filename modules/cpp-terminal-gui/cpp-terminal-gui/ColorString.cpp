

#include "ColorString.hpp"

TermGui::ColorString& TermGui::ColorString::append(const TermGui::ColorString& other){
	const auto string_size = this->_string.size();
	this->_string.append(other._string);
	for(const auto& elem : other._commands){
		this->_commands.add_override(elem.commands, string_size + elem.index);
	}
	return *this;
}

TermGui::ColorString& TermGui::ColorString::append(TermGui::ColorString&& other){
	const auto string_size = this->_string.size();
	this->_string.append(other._string);
	for(auto& elem : other._commands){
		this->_commands.add_override(std::move(elem.commands), string_size + elem.index);
	}
	return *this;
}

TermGui::ColorString& TermGui::ColorString::move_append(TermGui::ColorString& other, TermGui::ColorString::size_type pos, TermGui::ColorString::size_type n){
	const auto thisOldSize = this->size();
	this->_string.append(other._string, pos, n);
	other._string.erase(pos, n);
	
	auto commandItr = this->_commands.begin();
	const auto commandEnd = this->_commands.end();
	for(; commandItr != commandEnd; ++commandItr){
		if(commandItr->index >= pos){
			break;
		}
	}
	
	const auto moveStart = commandItr;
	for(; commandItr != commandEnd; ++commandItr){
		if(commandItr->index >= (pos + n)){
			break;
		}
		this->_commands.add_override(std::move(commandItr->commands), commandItr->index - pos + thisOldSize);
	}
	
	this->_commands.erase(moveStart, commandItr);
	
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

TermGui::ColorString& TermGui::ColorString::insert(size_type index, utf8::Char c){
	this->_string.insert(index, c);
	this->_commands.offset_index_after(index, 1);
	
	return *this;
}

TermGui::ColorString& TermGui::ColorString::erase(TermGui::ColorString::size_type index){
	this->_string.erase(index);
	this->_commands.merge(index, index+2);
	this->_commands.offset_index_after(index, -1);
	return *this;
}

TermGui::ColorString& TermGui::ColorString::erase(TermGui::ColorString::size_type index_from, TermGui::ColorString::size_type index_to){
	const auto count = index_to - index_from;
	this->_string.erase(index_from, count);
	this->_commands.merge(index_from, index_to);
	this->_commands.offset_index_after(index_from, count);
	return *this;
}
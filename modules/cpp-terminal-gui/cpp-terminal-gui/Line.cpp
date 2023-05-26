

#include "Line.hpp"

TermGui::Line& TermGui::Line::append(const TermGui::Line& other){
	const auto string_size = this->_string.size();
	this->_string.append(other._string);
	for(const auto& elem : other._commands){
		this->_commands.insert(elem.commands, string_size + elem.index)
	}
	return *this;
}

TermGui::Line& TermGui::Line::append(TermGui::Line&& other){
	const auto string_size = this->_string.size();
	this->_string.append(other._string);
	for(auto& elem : other._commands){
		this->_commands.insert(std::move(elem.commands), string_size + elem.index)
	}
	return *this;
}
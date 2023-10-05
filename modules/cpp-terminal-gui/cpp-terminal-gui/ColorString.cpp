#include "ColorString.hpp"

TermGui::ColorString& TermGui::ColorString::append(const TermGui::ColorString& other){
	const auto string_size = this->_string.size();
	this->_string.append(other._string);
	for(const auto& elem : other._styles){
		this->_styles.add_override(elem.styles, string_size + elem.index);
	}
	return *this;
}

TermGui::ColorString& TermGui::ColorString::append(TermGui::ColorString&& other){
	const auto string_size = this->_string.size();
	this->_string.append(other._string);
	for(auto& elem : other._styles){
		this->_styles.add_override(std::move(elem.styles), string_size + elem.index);
	}
	return *this;
}

TermGui::ColorString& TermGui::ColorString::move_append(TermGui::ColorString& other, TermGui::ColorString::size_type pos, TermGui::ColorString::size_type n){
	const auto thisOldSize = this->size();
	this->_string.append(other._string, pos, n);
	
	auto first = other._string.begin(); std::advance(first, pos);
	auto last = first; std::advance(last, n);
	other._string.erase(first, last);
	
	auto stylesItr = this->_styles.begin();
	const auto stylesEnd = this->_styles.end();
	for(; stylesItr != stylesEnd; ++stylesItr){
		if(stylesItr->index >= pos){
			break;
		}
	}
	
	const auto moveStart = stylesItr;
	for(; stylesItr != stylesEnd; ++stylesItr){
		if(stylesItr->index >= (pos + n)){
			break;
		}
		this->_styles.add_override(std::move(stylesItr->styles), stylesItr->index - pos + thisOldSize);
	}
	
	this->_styles.erase(moveStart, stylesItr);
	
	return *this;
}

void TermGui::ColorString::render(std::string& outputString) const {
	auto stylesItr = this->_styles.cbegin(); // start of the styles
	const auto stylesEnd = this->_styles.cend(); // end of the styles
	
	auto stringItr = this->_string.cbegin(); // start of the string
	const auto stringEnd = this->_string.cend(); // end of the string
	auto stringIndex = 0;
	
	for(;stringItr != stringEnd; (void)++stringItr, (void)++stringIndex){
		if(stylesItr->index == stringIndex) stylesItr->render(outputString);
		outputString += *stringItr;
	}
}

TermGui::ColorString& TermGui::ColorString::insert(size_type index, utf8::Char c){
	auto itr = this->_string.begin();
	std::advance(itr, index);
	this->_string.insert(itr, c);
	this->_styles.offset_index_after(index, 1);
	return *this;
}

TermGui::ColorString& TermGui::ColorString::erase(TermGui::ColorString::size_type index){
	auto itr = this->_string.begin();
	std::advance(itr, index);
	this->_string.erase(itr);
	this->_styles.merge(index, index + 2);
	this->_styles.offset_index_after(index, -1);
	return *this;
}

TermGui::ColorString& TermGui::ColorString::erase(TermGui::ColorString::size_type index, TermGui::ColorString::size_type count){
	auto first = this->_string.begin();
	std::advance(first, index);
	auto last = first;
	std::advance(last, count);
	this->_string.erase(first, last);
	this->_styles.merge(index, index + count);
	this->_styles.offset_index_after(index, index + count);
	return *this;
}
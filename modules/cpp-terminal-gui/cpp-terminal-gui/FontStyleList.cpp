
// Project
#include "FontStyle.hpp" 
#include "FontStyleList.hpp"

// c++ std
#include <iostream>


// +++++++++++++++++++++++ iStyles ++++++++++++++++++++++++

//constructor
TermGui::iStylesList::iStyles::iStyles(const FontStyle& fontStyle, size_type index){
	// newly constructed -> list is empty -> just push the fontStyle back and into the list
	this->styles.push_back(fontStyle);
	this->index = index;
}


bool TermGui::iStylesList::iStyles::add(const FontStyle& fontStyle){
	auto itr = this->find_greater_equal(fontStyle.type());
	if(itr == this->end()){
		this->styles.push_back(fontStyle);
		return true;
	}else if(itr->type() != fontStyle.type()){
		this->styles.insert(itr, fontStyle);
		return true;
	}else{
		return false;
	}
}
	
bool TermGui::iStylesList::iStyles::add(const TermGui::iStylesList::iStyles::list_type& listOfFontStyles){
	bool result = true;
	for(const auto& elem : listOfFontStyles){
		result &= this->add(elem);
	}
	return result;
}

bool TermGui::iStylesList::iStyles::add_override(const FontStyle& fontStyle){
	auto itr = this->find_greater_equal(fontStyle.type());
	if(itr == this->end()){
		this->styles.push_back(fontStyle);
		return true;
	}else if(itr->type() != fontStyle.type()){
		this->styles.insert(itr, fontStyle);
		return true;
	}else{
		*itr = fontStyle;
		return false;
	}
}

bool TermGui::iStylesList::iStyles::add_override(const TermGui::iStylesList::iStyles::list_type& listOfFontStyles){
	bool result = true;
	for(const auto& elem : listOfFontStyles){
		result &= this->add_override(elem);
	}
	return result;
}

TermGui::iStylesList::iStyles::iterator TermGui::iStylesList::iStyles::find_greater_equal(FontStyle::Type type){
	for(auto itr = this->begin(); itr != this->end(); ++itr){
		if(static_cast<size_type>(itr->type()) >= static_cast<size_type>(type)){
			return itr;
		}
	}
	return this->end();
}

// ++++++++++++++++++++++++ iStylesList ++++++++++++++++++++++++

TermGui::iStylesList::iStylesList(const TermGui::iStylesList& other, 
									TermGui::iStylesList::size_type pos, 
									TermGui::iStylesList::size_type count){
	auto itr = other.cbegin();
	const auto end = other.cend();
	// search the first iterator of the copy
	for(; itr != end && itr->index < pos; ++itr);
	auto first = itr;
	
	// search the end iterat
	for(; itr != end && itr->index < (pos + count); ++itr);
	auto last = itr;
	
	this->iStyleList.assign(first, last);
}

void TermGui::iStylesList::erase(TermGui::iStylesList::size_type index){
	auto itr = this->find(index);
	this->erase(itr);
}

void TermGui::iStylesList::erase(TermGui::iStylesList::size_type index_from, TermGui::iStylesList::size_type index_to){
	auto first = this->find_greater_equal(index_from);
	auto last = this->find_greater_equal(index_to);
	this->erase(first, last);
}

bool TermGui::iStylesList::add(const FontStyle& fontStyle, TermGui::iStylesList::size_type index){
	if(this->iStyleList.empty()){
		// add a new/first fontStyle point
		this->iStyleList.push_back(iStyles(fontStyle, index));
		return true;
	}else{
		// make a sorted add:
		// reverse search the element in which or after which will be inserted
		// reverse because new elements will be pushed back during writeing so it is expected
		// that the new insertion point is at the end of the list. 
		auto itr = this->find_greater_equal(index);
		if(itr->index == index){
			// fontStyle point already exists -> try adding the new fontStyle
			const bool result = itr->add(fontStyle);
			return result;
		}else{
			// fontStyle point does not exist -> create a new fontStyle point and add it after the iterator
			this->iStyleList.insert(itr, iStyles(fontStyle, index));
			return true;
		}
	}
}

bool TermGui::iStylesList::add(const iStylesList::iStyles::list_type& listOfFontStyles, TermGui::iStylesList::size_type index){
	if(this->iStyleList.empty()){
		this->iStyleList.push_back(iStyles(listOfFontStyles, index));
		return true;
	}else{
		auto itr = this->find_greater_equal(index);
		if(itr->index == index){
			// add into existing container with same index
			const bool result = itr->add(listOfFontStyles);
			return result;
		}else{
			// no container with this index exists -> make one
			this->iStyleList.insert(itr, iStyles(listOfFontStyles, index));
			return true;
		}
	}
}

bool TermGui::iStylesList::add(iStyles::list_type&& listOfFontStyles, TermGui::iStylesList::size_type index){
	if(this->iStyleList.empty()){
		this->iStyleList.push_back(iStyles(std::move(listOfFontStyles), index));
		return true;
	}else{
		auto itr = this->find_greater_equal(index);
		if(itr->index == index){
			// add into existing container with same index
			const bool result = itr->add(listOfFontStyles);
			return result;
		}else{
			// no container with this index exists -> make one
			this->iStyleList.insert(itr, iStyles(std::move(listOfFontStyles), index));
			return true;
		}
	}
}

bool TermGui::iStylesList::add_override(const FontStyle& fontStyle, size_type index){
	if(this->iStyleList.empty()){
		// add a new/first fontStyle point
		this->iStyleList.push_back(iStyles(fontStyle, index));
		return true;
	}else{
		// make a sorted add:
		// reverse search the element in which or after which will be inserted
		// reverse because new elements will be pushed back during writeing so it is expected
		// that the new insertion point is at the end of the list. 
		auto itr = this->find_greater_equal(index);
		if(itr->index == index){
			// fontStyle point already exists -> try adding the new fontStyle
			const bool result = itr->add_override(fontStyle);
			return result;
		}else{
			// fontStyle point does not exist -> create a new fontStyle point and add it after the iterator
			this->iStyleList.insert(itr, iStyles(fontStyle, index));
			return true;
		}
	}
}

bool TermGui::iStylesList::add_override(const iStylesList::iStyles::list_type& listOfFontStyles, TermGui::iStylesList::size_type index){
	if(this->iStyleList.empty()){
		this->iStyleList.push_back(iStyles(listOfFontStyles, index));
		return true;
	}else{
		auto itr = this->find_greater_equal(index);
		if(itr->index == index){
			// add into existing container with same index
			const bool result = itr->add_override(listOfFontStyles);
			return result;
		}else{
			// no container with this index exists -> make one
			this->iStyleList.insert(itr, iStyles(listOfFontStyles, index));
			return true;
		}
	}
}

bool TermGui::iStylesList::add_override(iStyles::list_type&& listOfFontStyles, TermGui::iStylesList::size_type index){
	if(this->iStyleList.empty()){
		this->iStyleList.push_back(iStyles(std::move(listOfFontStyles), index));
		return true;
	}else{
		auto itr = this->find_greater_equal(index);
		if(itr->index == index){
			// add into existing container with same index
			const bool result = itr->add_override(listOfFontStyles);
			return result;
		}else{
			// no container with this index exists -> make one
			this->iStyleList.insert(itr, iStyles(std::move(listOfFontStyles), index));
			return true;
		}
	}
}

TermGui::iStylesList::iterator TermGui::iStylesList::find_greater_equal(size_type index){
	for(auto itr = this->begin(); itr != this->end(); ++itr){
		if(itr->index >= index){
			return itr;
		}
	}
	return this->end();
}

TermGui::iStylesList::iterator TermGui::iStylesList::find(size_type index){
	for(auto itr = this->begin(); itr != this->end(); ++itr){
		if(itr->index == index){
			return itr;
		}
	}
	return this->end();
}

TermGui::iStylesList::const_iterator TermGui::iStylesList::find_greater_equal(size_type index) const {
	for(auto itr = this->begin(); itr != this->end(); ++itr){
		if(itr->index >= index){
			return itr;
		}
	}
	return this->end();
}

TermGui::iStylesList::const_iterator TermGui::iStylesList::find(size_type index) const {
	for(auto itr = this->begin(); itr != this->end(); ++itr){
		if(itr->index == index){
			return itr;
		}
	}
	return this->end();
}

void TermGui::iStylesList::offset_index_after(TermGui::iStylesList::size_type index, long offset){
	auto itr = this->rbegin();
	const auto end = this->rend();
	// find itr with the same or larger index
	for(; itr != end; ++itr){
		if(itr->index > index){
			itr->index += offset;
		}else{
			break;
		}
	}
}

void TermGui::iStylesList::offset_index_after(TermGui::iStylesList::iterator itr, long offset){
	this->offset_index_after(itr->index, offset);
}

void TermGui::iStylesList::merge(iterator first, iterator last){
	auto itr = first;
	while(itr != last){
		first->add_override(itr->styles);
	}
	this->erase(first+1, last);
}

void TermGui::iStylesList::merge(size_type first_index, size_type last_index){
	auto first = this->find_greater_equal(first_index);
	auto last = this->find_greater_equal(last_index);
}
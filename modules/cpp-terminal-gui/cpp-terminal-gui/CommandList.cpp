
// Project
#include "Command.hpp" 
#include "CommandList.hpp"

// c++ std
#include <memory>
#include <iostream>


// +++++++++++++++++++++++ Command Point ++++++++++++++++++++++++

//constructor
TermGui::CommandList::CommandPoint::CommandPoint(std::unique_ptr<Command>&& pCommand, size_type index){
	// newly constructed -> list is empty -> just push the command back and into the list
	this->commands.push_back(std::move(pCommand));
	this->index = index;
}

TermGui::CommandList::CommandPoint::CommandPoint(const list_type& list, size_type index){
	this->index = index;
	for(const auto& elem : list){
		this->commands.push_back(elem->make_unique_copy());	
	}
}

bool TermGui::CommandList::CommandPoint::insert(std::unique_ptr<Command>&& pCommand){
	if(this->commands.empty()){
		// if empty -> no worries just push back
		this->commands.push_back(std::move(pCommand));
		return true;
	}else{
		// find the first in reverse that is smaller or equal
		auto itr = this->find_first_smaller_equal_reverse(pCommand->type());
		if(itr == this->rend()){
			this->commands.push_front(std::move(pCommand));
			return true;
		}else if((*itr)->type() != pCommand->type()){
			this->commands.insert(itr.base(), std::move(pCommand));
			return true;
		}else{
			return false;
		}
	}
}
	
bool TermGui::CommandList::CommandPoint::insert(const list_type& listOfCommands){
	bool result = false;
	for(const auto& elem : listOfCommands){
		if(this->commands.empty()){
			this->commands.push_back(elem->make_unique_copy()); // copy - malloc
			result = true;
		}else{
			auto itr = this->find_first_smaller_equal_reverse(elem->type());
			if(itr == this->rend()){
				this->commands.push_front(elem->make_unique_copy()); // copy - malloc
				result = true;
			}else if((*itr)->type() != elem->type()){
				this->commands.insert(itr.base(), elem->make_unique_copy()); // copy - malloc
				result = true;
			}
		}
	}
	return result;
}

bool TermGui::CommandList::CommandPoint::insert(CommandPoint::list_type&& listOfCommands){
	bool result = false;
	for(auto& elem : listOfCommands){
		if(this->commands.empty()){
			this->commands.push_back(std::move(elem));
			result = true;
		}else{
			auto itr = this->find_first_smaller_equal_reverse(elem->type());
			if(itr == this->rend()){
				this->commands.push_front(std::move(elem));
				result = true;
			}else if((*itr)->type() != elem->type()){
				this->commands.insert(itr.base(), std::move(elem));
				result = true;
			}
		}
	}
	return result;
}
	
bool TermGui::CommandList::CommandPoint::insert_override(std::unique_ptr<Command>&& pCommand){
	return false;
}

bool TermGui::CommandList::CommandPoint::insert_override(const Command& pCommand){
	return false;
}

bool TermGui::CommandList::CommandPoint::contains(CommandType type){
	for(const auto& elem : *this){
		if(elem->type() == type){
			return true;
		}
	}
	return false;
}

TermGui::CommandList::CommandPoint::reverse_iterator TermGui::CommandList::CommandPoint::find_first_smaller_equal_reverse(CommandType type){
	for(auto itr = this->rbegin(); itr != this->rend(); ++itr){
		if(static_cast<size_type>((*itr)->type()) <= static_cast<size_type>(type)){
			return itr;
		}
	}
	return this->rend();
}

// ++++++++++++++++++++++++ Command List ++++++++++++++++++++++++

TermGui::CommandList::CommandList(const TermGui::CommandList& other, 
									TermGui::CommandList::size_type pos, 
									TermGui::CommandList::size_type count){
	auto itr = other.cbegin();
	const auto end = other.cend();
	// search the first iterator of the copy
	for(; itr != end && itr->index < pos; ++itr);
	auto first = itr;
	
	// search the end iterat
	for(; itr != end && itr->index < (pos + count); ++itr);
	auto last = itr;
	
	this->commands.assign(first, last);
}


bool TermGui::CommandList::insert(std::unique_ptr<Command>&& pCommand, size_type index){
	if(this->commands.empty()){
		// insert a new/first command point
		this->commands.push_back(CommandPoint(std::move(pCommand), index));
		return true;
	}else{
		// make a sorted insert:
		// reverse search the element in which or after which will be inserted
		// reverse because new elements will be pushed back during writeing so it is expected
		// that the new insertion point is at the end of the list. 
		auto itr = this->find_first_smaller_equal_reverse(index);
		if(itr->index == index){
			// command point already exists -> try adding the new command
			const bool result = itr->insert(std::move(pCommand));
			return result;
		}else{
			// command point does not exist -> create a new command point and insert it after the iterator
			this->commands.insert(itr.base(), CommandPoint(std::move(pCommand), index));
			return true;
		}
	}
	
}

bool TermGui::CommandList::insert(const CommandList::CommandPoint::list_type& listOfCommands, TermGui::CommandList::size_type index){
	if(this->commands.empty()){
		this->commands.push_back(CommandPoint(listOfCommands, index));
		return true;
	}else{
		auto itr = this->find_first_smaller_equal_reverse(index);
		if(itr->index == index){
			// insert into existing container with same index
			const bool result = itr->insert(listOfCommands);
			return result;
		}else{
			// no container with this index exists -> make one
			this->commands.insert(itr.base(), CommandPoint(listOfCommands, index));
			return true;
		}
	}
}

bool TermGui::CommandList::insert(CommandPoint::list_type&& listOfCommands, TermGui::CommandList::size_type index){
	if(this->commands.empty()){
		this->commands.push_back(CommandPoint(std::move(listOfCommands), index));
		return true;
	}else{
		auto itr = this->find_first_smaller_equal_reverse(index);
		if(itr->index == index){
			// insert into existing container with same index
			const bool result = itr->insert(std::move(listOfCommands));
			return result;
		}else{
			// no container with this index exists -> make one
			this->commands.insert(itr.base(), CommandPoint(std::move(listOfCommands), index));
			return true;
		}
	}
}

bool TermGui::CommandList::insert_override(std::unique_ptr<Command>&& pCommand, size_type index){
	return false;
}

TermGui::CommandList::reverse_iterator TermGui::CommandList::find_first_smaller_equal_reverse(size_type index){
	for(auto itr = this->rbegin(); itr != this->rend(); ++itr){
		if(itr->index <= index){
			return itr;
		}
	}
	return this->rend();
}
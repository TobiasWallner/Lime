
// Project
#include "Command.hpp" 
#include "CommandList.hpp"


// +++++++++++++++++++++++ Command Point ++++++++++++++++++++++++

//constructor
TermGui::CommandList::CommandPoint::CommandPoint(std::unique_ptr<Command>&& pCommand, size_type index){
	// newly constructed -> list is empty -> just push the command back and into the list
	this->commands.push_back(std::move(pCommand));
	this->index = index;
}


bool TermGui::CommandList::CommandPoint::insert(std::unique_ptr<Command>&& pCommand){
	if(this->commands.empty()){
		// if empty -> no worries just push back
		this->commands.push_back(std::move(pCommand));
		return true;
	}else{
		// find the first in reverse that is smaller or equal
		auto itr = this->find_first_smaller_equal_reverse(pCommand->type());
		// it has not the same type -> insert it afterwards, else return false
		if((*itr)->type() != pCommand->type()){
			this->commands.insert(itr.base(), std::move(pCommand));
			return true;
		}else{
			return false;
		}
	}
}
	
	
bool TermGui::CommandList::CommandPoint::insert_override(std::unique_ptr<Command>&& pCommand){
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
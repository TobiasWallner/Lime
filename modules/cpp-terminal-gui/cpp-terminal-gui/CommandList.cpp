
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

bool TermGui::CommandList::CommandPoint::add(std::unique_ptr<Command>&& pCommand){
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

bool TermGui::CommandList::CommandPoint::add(const Command& command){
	if(this->commands.empty()){
		// if empty -> no worries just push back
		this->commands.push_back(command.make_unique_copy());
		return true;
	}else{
		// find the first in reverse that is smaller or equal
		auto itr = this->find_first_smaller_equal_reverse(command.type());
		if(itr == this->rend()){
			this->commands.push_front(command.make_unique_copy());
			return true;
		}else if((*itr)->type() != command.type()){
			this->commands.insert(itr.base(), command.make_unique_copy());
			return true;
		}else{
			return false;
		}
	}
}
	
bool TermGui::CommandList::CommandPoint::add(const TermGui::CommandList::CommandPoint::list_type& listOfCommands){
	bool result = true;
	for(const auto& elem : listOfCommands){
		result &= this->add(*(elem.get()));
	}
	return result;
}

bool TermGui::CommandList::CommandPoint::add(TermGui::CommandList::CommandPoint::list_type&& listOfCommands){
	bool result = false;
	for(auto& elem : listOfCommands){
		result &= this->add(std::move(elem));
	}
	return result;
}

bool TermGui::CommandList::CommandPoint::add_override(std::unique_ptr<Command>&& pCommand){
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
			*itr = std::move(pCommand);
			return true;
		}
	}
}

bool TermGui::CommandList::CommandPoint::add_override(const Command& command){
	if(this->commands.empty()){
		// if empty -> no worries just push back
		this->commands.push_back(command.make_unique_copy());
		return true;
	}else{
		// find the first in reverse that is smaller or equal
		auto itr = this->find_first_smaller_equal_reverse(command.type());
		if(itr == this->rend()){
			this->commands.push_front(command.make_unique_copy());
			return true;
		}else if((*itr)->type() != command.type()){
			this->commands.insert(itr.base(), command.make_unique_copy());
			return true;
		}else /*(*itr)->type() == pCommand->type()*/{
			// since we know that this is the same type we can save a malloc and call the copy assignment instead
			switch(command.type()){
				case CommandType::forgroundColor:{
					static_cast<TermGui::FgColor&>(**itr) = static_cast<const TermGui::FgColor&>(command);
				}break;
				case CommandType::backgroundColor:{
					static_cast<TermGui::BgColor&>(**itr) = static_cast<const TermGui::BgColor&>(command);
				}break;
				default : {
					std::cerr << "Unhandeled Command Type in switch statement\n"
								 "in file: " << __FILE__ << "\n"
								 "in function: " << __FUNCTION__ << "\n"
								 "at line: " << __LINE__ << "\n" << std::endl;
					*itr = command.make_unique_copy();
				}break;
			}
			(*itr)->operator=(command);
			return true;
		}
	}
}

bool TermGui::CommandList::CommandPoint::add_override(const TermGui::CommandList::CommandPoint::list_type& listOfCommands){
	bool result = true;
	for(const auto& elem : listOfCommands){
		result &= this->add_override(*(elem.get()));
	}
	return result;
}

bool TermGui::CommandList::CommandPoint::add_override(TermGui::CommandList::CommandPoint::list_type&& listOfCommands){
	bool result = false;
	for(auto& elem : listOfCommands){
		result &= this->add_override(std::move(elem));
	}
	return result;
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

void TermGui::CommandList::erase(TermGui::CommandList::size_type index){
	auto itr = this->begin();
	const auto end = this->end();
	for(; itr != this->end(); ++itr){
		if(itr->index == index){
			this->commands.erase(itr);
			return;
		}
	}
}

void TermGui::CommandList::erase(TermGui::CommandList::size_type index_from, TermGui::CommandList::size_type index_to){
	auto itr = this->begin();
	for(; itr != this->end(); ++itr){
		if(itr->index >= index_from){
			break;
		}
	}
	
	for(; itr != this->end(); ++itr){
		if(itr->index >= index_to){
			break;
		}else{
			auto copy = itr;
			--itr;
			this->commands.erase(copy);
		}
	}
}

bool TermGui::CommandList::add(std::unique_ptr<Command>&& pCommand, size_type index){
	if(this->commands.empty()){
		// add a new/first command point
		this->commands.push_back(CommandPoint(std::move(pCommand), index));
		return true;
	}else{
		// make a sorted add:
		// reverse search the element in which or after which will be inserted
		// reverse because new elements will be pushed back during writeing so it is expected
		// that the new insertion point is at the end of the list. 
		auto itr = this->find_first_smaller_equal_reverse(index);
		if(itr->index == index){
			// command point already exists -> try adding the new command
			const bool result = itr->add(std::move(pCommand));
			return result;
		}else{
			// command point does not exist -> create a new command point and add it after the iterator
			this->commands.insert(itr.base(), CommandPoint(std::move(pCommand), index));
			return true;
		}
	}
}

bool TermGui::CommandList::add(const Command& command, TermGui::CommandList::size_type index){
	if(this->commands.empty()){
		// add a new/first command point
		this->commands.push_back(CommandPoint(command.make_unique_copy(), index));
		return true;
	}else{
		// make a sorted add:
		// reverse search the element in which or after which will be inserted
		// reverse because new elements will be pushed back during writeing so it is expected
		// that the new insertion point is at the end of the list. 
		auto itr = this->find_first_smaller_equal_reverse(index);
		if(itr->index == index){
			// command point already exists -> try adding the new command
			const bool result = itr->add(command);
			return result;
		}else{
			// command point does not exist -> create a new command point and add it after the iterator
			this->commands.insert(itr.base(), CommandPoint(command.make_unique_copy(), index));
			return true;
		}
	}
}

bool TermGui::CommandList::add(const CommandList::CommandPoint::list_type& listOfCommands, TermGui::CommandList::size_type index){
	if(this->commands.empty()){
		this->commands.push_back(CommandPoint(listOfCommands, index));
		return true;
	}else{
		auto itr = this->find_first_smaller_equal_reverse(index);
		if(itr->index == index){
			// add into existing container with same index
			const bool result = itr->add(listOfCommands);
			return result;
		}else{
			// no container with this index exists -> make one
			this->commands.insert(itr.base(), CommandPoint(listOfCommands, index));
			return true;
		}
	}
}

bool TermGui::CommandList::add(CommandPoint::list_type&& listOfCommands, TermGui::CommandList::size_type index){
	if(this->commands.empty()){
		this->commands.push_back(CommandPoint(std::move(listOfCommands), index));
		return true;
	}else{
		auto itr = this->find_first_smaller_equal_reverse(index);
		if(itr->index == index){
			// add into existing container with same index
			const bool result = itr->add(std::move(listOfCommands));
			return result;
		}else{
			// no container with this index exists -> make one
			this->commands.insert(itr.base(), CommandPoint(std::move(listOfCommands), index));
			return true;
		}
	}
}

bool TermGui::CommandList::add_override(std::unique_ptr<Command>&& pCommand, size_type index){
	if(this->commands.empty()){
		// add a new/first command point
		this->commands.push_back(CommandPoint(std::move(pCommand), index));
		return true;
	}else{
		// make a sorted add:
		// reverse search the element in which or after which will be inserted
		// reverse because new elements will be pushed back during writeing so it is expected
		// that the new insertion point is at the end of the list. 
		auto itr = this->find_first_smaller_equal_reverse(index);
		if(itr->index == index){
			// command point already exists -> try adding the new command
			const bool result = itr->add_override(std::move(pCommand));
			return result;
		}else{
			// command point does not exist -> create a new command point and add it after the iterator
			this->commands.insert(itr.base(), CommandPoint(std::move(pCommand), index));
			return true;
		}
	}
}

bool TermGui::CommandList::add_override(const Command& command, size_type index){
	if(this->commands.empty()){
		// add a new/first command point
		this->commands.push_back(CommandPoint(command.make_unique_copy(), index));
		return true;
	}else{
		// make a sorted add:
		// reverse search the element in which or after which will be inserted
		// reverse because new elements will be pushed back during writeing so it is expected
		// that the new insertion point is at the end of the list. 
		auto itr = this->find_first_smaller_equal_reverse(index);
		if(itr->index == index){
			// command point already exists -> try adding the new command
			const bool result = itr->add_override(command);
			return result;
		}else{
			// command point does not exist -> create a new command point and add it after the iterator
			this->commands.insert(itr.base(), CommandPoint(command.make_unique_copy(), index));
			return true;
		}
	}
}

bool TermGui::CommandList::add_override(const CommandList::CommandPoint::list_type& listOfCommands, TermGui::CommandList::size_type index){
	if(this->commands.empty()){
		this->commands.push_back(CommandPoint(listOfCommands, index));
		return true;
	}else{
		auto itr = this->find_first_smaller_equal_reverse(index);
		if(itr->index == index){
			// add into existing container with same index
			const bool result = itr->add_override(listOfCommands);
			return result;
		}else{
			// no container with this index exists -> make one
			this->commands.insert(itr.base(), CommandPoint(listOfCommands, index));
			return true;
		}
	}
}

bool TermGui::CommandList::add_override(CommandPoint::list_type&& listOfCommands, TermGui::CommandList::size_type index){
	if(this->commands.empty()){
		this->commands.push_back(CommandPoint(std::move(listOfCommands), index));
		return true;
	}else{
		auto itr = this->find_first_smaller_equal_reverse(index);
		if(itr->index == index){
			// add into existing container with same index
			const bool result = itr->add_override(std::move(listOfCommands));
			return result;
		}else{
			// no container with this index exists -> make one
			this->commands.insert(itr.base(), CommandPoint(std::move(listOfCommands), index));
			return true;
		}
	}
}

TermGui::CommandList::reverse_iterator TermGui::CommandList::find_first_smaller_equal_reverse(size_type index){
	for(auto itr = this->rbegin(); itr != this->rend(); ++itr){
		if(itr->index <= index){
			return itr;
		}
	}
	return this->rend();
}


void TermGui::CommandList::offset_index_after(TermGui::CommandList::iterator itr, TermGui::CommandList::size_type offset){
	if(itr == this->end()){
		return;
	}
	++itr;
	for(; itr != this->end(); ++itr){
		itr->index += offset;
	}
}

void TermGui::CommandList::offset_index_after(TermGui::CommandList::size_type index, TermGui::CommandList::size_type offset){
	auto itr = this->begin();
	const auto end = this->end();
	// find itr with the same or larger index
	for(; itr != end; ++itr){
		if(itr->index >= index){
			break;
		}
	}
	this->offset_index_after(itr, offset);
}

void TermGui::CommandList::merge(iterator first, iterator last){
	for(auto itr = first; itr != last; ++itr){
		first->add_override(itr->commands);
		auto erase_itr = itr;
		--itr;
		this->commands.erase(erase_itr);
	}
}

void TermGui::CommandList::merge(size_type first_index, size_type last_index){
	auto itr = this->begin();
	auto first = itr;
	for(; itr != this->end(); ++itr){
		if(itr->index >= first_index){
			first = itr;
			break;
		}
	}
	for(; itr != this->end(); ++itr){
		if(itr->index >= last_index){
			break;
		}
		first->add_override(itr->commands);
		auto erase_itr = itr;
		--itr;
		this->commands.erase(erase_itr);
	}
}
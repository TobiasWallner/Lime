#pragma once

// c++ std
#include <memory>
#include <cstdio>
#include <list>

// project
#include "Command.hpp"

namespace TermGui{

/// A command point stores commands and the index at which those commands will be executed
struct CommandsPoint{
	std::list<Command> commands
	size_type index; 					// the index at which the command should be executed
};

/// Stores a list of commands and the position at which they should be executed.
/// the command list and their execution follows the following rules:
/// 	* all commands are sorted by their index number - the index at which they will be executed.
///		* insertions are sorted insertions into the command list
/// 	* commands are expected to be executed before anything else. 
/// 	* there can only be one Command of one type at any given index.
class CommandList{
public:	
	using size_type = std::size_t;
	using list_type = std::forward_list<CommandsPoint>;
	
	using value_type = list_type::value_type;
	using allocator_type = list_type::allocator_type;
	using difference_type = list_type::difference_type;
	
	using iterator = list_type::iterator;
	using const_iterator = list_type::const_iterator;
	using reference = list_type::reference;
	using const_reference = list_type::const_reference;
	using pointer = list_type::pointer;

private:

	list_type commands; // the list of commands

public:
	/// default construction of an empty list
	CommandList() = default;
	
	/// The command list is copy constructable
	CommandList(const CommandList& other) = default;
	
	/// The command list is move constructable
	CommandList(CommandList&& other) = default;
	
	/// The command list is copy constructable
	CommandList& operator=(const CommandList& other) = default;
	
	/// The command list is move assignment constructable
	CommandList& operator=(CommandList&& other) = default;
	
	/// insert a command into the list, will be inserted by in order of the index
	
}

}
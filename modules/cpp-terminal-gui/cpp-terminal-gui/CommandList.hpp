#pragma once

// c++ std
#include <memory>
#include <cstdio>
#include <list>
#include <utility>
#include <type_traits>

// project
#include "Command.hpp"

// C++ std
#include <list>

namespace TermGui{


/// Stores a list of commands and the position at which they should be executed.
/// the command list and their execution follows the following rules:
/// 	* all commands are sorted by their index number - the index at which they will be executed.
///		* insertions are sorted insertions into the command list
/// 	* commands are expected to be executed before anything else. 
/// 	* there can only be one Command of one type at any given index.
class CommandList{
private:	
	/// A command point stores commands and the index at which those commands will be executed
	/// Each CommandPoint can only ever have one of each command types. 
	/// Commands will be stored in ascending order according to the enum of the command types.
	/// Commands will be executed in ascending order according to their enum.
	class CommandPoint{
	public:
		using list_type =  std::list<std::unique_ptr<Command>>;
		using size_type = list_type::size_type;
	
		using value_type = list_type::value_type;
		using allocator_type = list_type::allocator_type;
		using difference_type = list_type::difference_type;
		
		using iterator = list_type::iterator;
		using const_iterator = list_type::const_iterator;
		using reverse_iterator = list_type::reverse_iterator;
		using const_reverse_iterator = list_type::const_reverse_iterator;
		using reference = list_type::reference;
		using const_reference = list_type::const_reference;
		using pointer = list_type::pointer;
	private:
		list_type commands;		// list of commands to be executed 
		
	public:
		// public member index position because the position of the Command execution might
		// change if the string that it is connected to changes in lenght
		size_type index; 		// the index at which the command should be executed
		
		/// constructs a command point from a Command and the given index
		CommandPoint(std::unique_ptr<Command>&& pCommand, size_type index);
		
		/// The command list is copy constructable
		CommandPoint(const CommandPoint& other) = default;
		
		/// The command list is move constructable
		CommandPoint(CommandPoint&& other) = default;
		
		/// The command list is copy constructable
		CommandPoint& operator=(const CommandPoint& other) = default;
		
		/// The command list is move assignment constructable
		CommandPoint& operator=(CommandPoint&& other) = default;
		
		/// changes
		inline void set_index(size_type newIndex) {this->index = newIndex;}
		
		/// returns 'true' if the container is empty and 'false' otherwise
		inline bool empty() const {return this->commands.empty();}
		
		/// returns the number of elements within the list
		inline size_type size() const {return this->commands.size();}
		
		/// returns an iterator to the beginning of the list aka the first element
		inline iterator begin(){return this->commands.begin();}
		inline const_iterator begin() const {return this->commands.begin();}
		inline const_iterator cbegin() const {return this->commands.cbegin();}
		
		/// returns an iterator past the end of the list
		inline iterator end(){return this->commands.end();}
		inline const_iterator end() const {return this->commands.end();}
		inline const_iterator cend() const {return this->commands.cend();}
		
		/// returns a reverse iterator to the beginning of the reversed range
		/// for more details see: https://en.cppreference.com/w/cpp/iterator/reverse_iterator
		inline reverse_iterator rbegin(){return this->commands.rbegin();}
		inline const_reverse_iterator rbegin() const {return this->commands.rbegin();}
		inline const_reverse_iterator rcbegin() const {return this->commands.crbegin();}
		
		/// returns a reverse iterator to the end of the reversed list
		/// for more details see: https://en.cppreference.com/w/cpp/iterator/reverse_iterator
		inline reverse_iterator rend(){return this->commands.rend();}
		inline const_reverse_iterator rend() const {return this->commands.rend();}
		inline const_reverse_iterator rcend() const {return this->commands.crend();}
		
		/// make an ordered/sorted insert of the command at the given position into the list.
		/// returns true if the operation was successfull, returns false if the new command could not be inserted.
		/// note that the simple insert will not override existing commands and would return false in that case
		bool insert(std::unique_ptr<Command>&& pCommand);
	
	
		/// make an ordered/sorted insert of the command at the given position into the list.
		/// returns true if the operation was successfull, returns false if the new command could not be inserted.
		/// note that this command will override an existing command and would return true in that case
		bool insert_override(std::unique_ptr<Command>&& pCommand);
		
		/// returns 'true' if a command with the same type already exists within this list. 
		/// returns 'false' otherwise.
		bool contains(CommandType type);
		
	private:
		/// returns the first found iterator to an object that has an index that is smaller or equal.
		/// if no satisfieing element has been found the reverse-end-iterator will be returned. 
		reverse_iterator find_first_smaller_equal_reverse(CommandType type);
		
		
	};

public:	
	using size_type = std::size_t;
	using list_type = std::list<CommandPoint>;
	
	using value_type = list_type::value_type;
	using allocator_type = list_type::allocator_type;
	using difference_type = list_type::difference_type;
	
	using iterator = list_type::iterator;
	using const_iterator = list_type::const_iterator;
	using reverse_iterator = list_type::reverse_iterator;
	using const_reverse_iterator = list_type::const_reverse_iterator;
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
	
	/// returns 'true' if the container is empty and 'false' otherwise
	inline bool empty() const {return this->commands.empty();}
	
	/// returns the number of different indizes 
	inline size_type size() const {return this->commands.size();}
	
	/// returns an iterator to the beginning of the list aka the first element
	inline iterator begin(){return this->commands.begin();}
	inline const_iterator begin() const {return this->commands.begin();}
	inline const_iterator cbegin() const {return this->commands.cbegin();}
	
	/// returns an iterator past the end of the list
	inline iterator end(){return this->commands.end();}
	inline const_iterator end() const {return this->commands.end();}
	inline const_iterator cend() const {return this->commands.cend();}
	
	/// returns a reverse iterator to the beginning of the reversed range
	/// for more details see: https://en.cppreference.com/w/cpp/iterator/reverse_iterator
	inline reverse_iterator rbegin(){return this->commands.rbegin();}
	inline const_reverse_iterator rbegin() const {return this->commands.rbegin();}
	inline const_reverse_iterator rcbegin() const {return this->commands.crbegin();}
	
	/// returns a reverse iterator to the end of the reversed list
	/// for more details see: https://en.cppreference.com/w/cpp/iterator/reverse_iterator
	inline reverse_iterator rend(){return this->commands.rend();}
	inline const_reverse_iterator rend() const {return this->commands.rend();}
	inline const_reverse_iterator rcend() const {return this->commands.crend();}
	
	/// returns a reference to the first element
	inline reference front(){return this->commands.front();}
	inline const_reference front() const {return this->commands.front();}
	
	/// returns a reference to the last element
	inline reference back(){return this->commands.back();}
	inline const_reference back() const {return this->commands.back();}
	
	/// make an ordered/sorted insert of the command at the given position into the list.
	/// returns true if the operation was successfull, returns false if the new command could not be inserted.
	/// note that the simple insert will not override existing commands and would return false in that case
	bool insert(std::unique_ptr<Command>&& pCommand, size_type index);
	
	/// wrapper for derived classes
	template<class C>
	inline bool insert(std::unique_ptr<C>&& pCommand, size_type index){
		return this->insert(static_cast<std::unique_ptr<Command>>(std::move(pCommand)), index);
	}
	
	/// wrapper for classes that are not a unique pointer yet and are moved
	/// the passed object will be 'forewareded' into the datastructure using 'std::forward'
	template<class Type>
	inline bool insert(Type&& Object, size_type index){
		using RawType = typename std::remove_reference<Type>::type;
		return this->insert(std::make_unique<RawType>(std::forward<Type>(Object)), index);
	}
	
	/// make an ordered/sorted insert of the command at the given position into the list.
	/// returns true if the operation was successfull, returns false if the new command could not be inserted.
	/// note that this command will override an existing command and would return true in that case
	bool insert_override(std::unique_ptr<Command>&& pCommand, size_type index);
	
	/// wrapper for derived classes
	template<class C>
	inline bool insert_override(std::unique_ptr<C>&& pCommand, size_type index){
		return this->insert_override(static_cast<std::unique_ptr<Command>>(std::move(pCommand)), index);
	}
	
	/// wrapper for classes that are not a unique pointer yet and are moved
	/// the passed object will be 'forewareded' into the datastructure using 'std::forward'
	template<class Type>
	inline bool insert_override(Type&& Object, size_type index){
		using RawType = typename std::remove_reference<Type>::type;
		return this->insert_override(std::make_unique<RawType>(std::forward<Type>(Object)), index);
	}
	
private:

	/// returns the first found iterator to an object that has an index that is smaller or equal.
	/// if no satisfieing element has been found the end-reverse-iterator will be returned. 
	reverse_iterator find_first_smaller_equal_reverse(size_type index);
	
};

}
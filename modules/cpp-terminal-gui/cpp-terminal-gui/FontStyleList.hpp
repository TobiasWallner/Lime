#pragma once

// c++ std
#include <memory>
#include <cstdio>
#include <vector>
#include <utility>
#include <type_traits>

// project
#include "FontStyle.hpp"

// C++ std
#include <list>

namespace TermGui{

using FontStyleList = std::vector<FontStyle>;

/// Stores a list of styles and the position at which they should be executed.
/// the fontStyle list and their execution follows the following rules:
/// 	* all styles are sorted by their index number - the index at which they will be executed.
///		* insertions are sorted insertions into the fontStyle list
/// 	* styles are expected to be executed before anything else. 
/// 	* there can only be one FontStyle of one type at any given index.
class iStylesList{
public:	
	/// A fontStyle point stores styles and the index at which those styles will be executed
	/// Each iStyles can only ever have one of each fontStyle types. 
	/// styles will be stored in ascending order according to the enum of the fontStyle types.
	/// styles will be executed in ascending order according to their enum.
	class iStyles : RenderTrait{
	public:
		using list_type =  FontStyleList;
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
	public:
		list_type styles;		// list of styles to be executed 

		// public member index position because the position of the FontStyle execution might
		// change if the string that it is connected to changes in lenght
		size_type index; 		// the index at which the fontStyle should be executed
		
		/// The fontStyle list is copy constructable
		inline iStyles(const iStyles& other) = default;
		
		/// constructs a fontStyle point from a FontStyle and the given index
		iStyles(const FontStyle& fontStyle, size_type index);
		
		inline iStyles(const list_type& list, size_type index) : styles(list), index(index){}
		inline iStyles(list_type&& list, size_type index) : styles(std::move(list)), index(index){}
		
		/// The fontStyle list is copy constructable
		inline iStyles& operator=(const iStyles& other) = default;
		
		/// changes
		inline void set_index(size_type newIndex) {this->index = newIndex;}
		
		/// returns 'true' if the container is empty and 'false' otherwise
		inline bool empty() const {return this->styles.empty();}
		
		/// returns the number of elements within the list
		inline size_type size() const {return this->styles.size();}
		
		/// returns an iterator to the beginning of the list aka the first element
		inline iterator begin(){return this->styles.begin();}
		inline const_iterator begin() const {return this->styles.begin();}
		inline const_iterator cbegin() const {return this->styles.cbegin();}
		
		/// returns an iterator past the end of the list
		inline iterator end(){return this->styles.end();}
		inline const_iterator end() const {return this->styles.end();}
		inline const_iterator cend() const {return this->styles.cend();}
		
		/// returns a reverse iterator to the beginning of the reversed range
		/// for more details see: https://en.cppreference.com/w/cpp/iterator/reverse_iterator
		inline reverse_iterator rbegin(){return this->styles.rbegin();}
		inline const_reverse_iterator rbegin() const {return this->styles.rbegin();}
		inline const_reverse_iterator rcbegin() const {return this->styles.crbegin();}
		
		/// returns a reverse iterator to the end of the reversed list
		/// for more details see: https://en.cppreference.com/w/cpp/iterator/reverse_iterator
		inline reverse_iterator rend(){return this->styles.rend();}
		inline const_reverse_iterator rend() const {return this->styles.rend();}
		inline const_reverse_iterator rcend() const {return this->styles.crend();}
		
		inline void assign(const_iterator first, const_iterator last){ this->styles.assign(first, last);}
		
		/// make an ordered/sorted add of the fontStyle at the given position into the list.
		/// returns true if the operation was successfull, returns false if the new fontStyle could not be inserted.
		/// note that the simple add will not override existing styles and would return false in that case
		bool add(const FontStyle& fontStyle);
		
		/// returns true if this list has changed due to this insertions
		/// returns false if this list has changed unchanged
		bool add(const list_type& listOfCommands);
		bool add(list_type&& listOfCommands);
	
		/// make an ordered/sorted add of the fontStyle at the given position into the list.
		/// returns true if the operation was successfull, returns false if the new fontStyle could not be inserted.
		/// note that this fontStyle will override an existing fontStyle and would return true in that case
		bool add_override(const FontStyle& fontStyle);
		bool add_override(const list_type& listOfCommands);
		bool add_override(list_type&& listOfCommands);
		
		inline void render(std::string& outputString) const override { for(const auto& elem : this->styles) elem.render(outputString);}
		
	private:
		/// returns the first found iterator to an object that has an index that is smaller or equal.
		/// if no satisfieing element has been found the reverse-end-iterator will be returned. 
		iterator find_greater_equal(FontStyle::Type type);
	};

public:	
	using size_type = std::size_t;
	using list_type = std::vector<iStyles>;
	
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

	list_type iStyleList; 

public:
	/// default construction of an empty list
	iStylesList() = default;
	
	/// The fontStyle list is not copy constructable
	iStylesList(const iStylesList& other) = default;
	
	/// The fontStyle list is move constructable
	iStylesList(iStylesList&& other) = default;
	
	/// The fontStyle list is copy not constructable
	iStylesList& operator=(const iStylesList& other) = default;
	
	/// The fontStyle list is move assignment constructable
	iStylesList& operator=(iStylesList&& other) = default;
	
	/// constructs the FontStyle List with a sublist of all elements that are 
	/// within the range of indizes of [pos, pos + count) of other. 
	iStylesList(const iStylesList& other, size_type pos, size_type count);
	
	/// returns 'true' if the container is empty and 'false' otherwise
	inline bool empty() const {return this->iStyleList.empty();}
	
	/// returns the number of different indizes 
	inline size_type size() const {return this->iStyleList.size();}
	
	inline void clear() {this->iStyleList.clear();}
	
	inline void erase(const_iterator itr){this->iStyleList.erase(itr);}
	inline void erase(const_iterator first, const_iterator last){this->iStyleList.erase(first, last);}
	void erase(size_type index);
	void erase(size_type index_from, size_type index_to);
	
	/// returns an iterator to the beginning of the list aka the first element
	inline iterator begin(){return this->iStyleList.begin();}
	inline const_iterator begin() const {return this->iStyleList.begin();}
	inline const_iterator cbegin() const {return this->iStyleList.cbegin();}
	
	/// returns an iterator past the end of the list
	inline iterator end(){return this->iStyleList.end();}
	inline const_iterator end() const {return this->iStyleList.end();}
	inline const_iterator cend() const {return this->iStyleList.cend();}
	
	/// returns a reverse iterator to the beginning of the reversed range
	/// for more details see: https://en.cppreference.com/w/cpp/iterator/reverse_iterator
	inline reverse_iterator rbegin(){return this->iStyleList.rbegin();}
	inline const_reverse_iterator rbegin() const {return this->iStyleList.rbegin();}
	inline const_reverse_iterator rcbegin() const {return this->iStyleList.crbegin();}
	
	/// returns a reverse iterator to the end of the reversed list
	/// for more details see: https://en.cppreference.com/w/cpp/iterator/reverse_iterator
	inline reverse_iterator rend(){return this->iStyleList.rend();}
	inline const_reverse_iterator rend() const {return this->iStyleList.rend();}
	inline const_reverse_iterator rcend() const {return this->iStyleList.crend();}
	
	/// returns a reference to the first element
	inline reference front(){return this->iStyleList.front();}
	inline const_reference front() const {return this->iStyleList.front();}
	
	/// returns a reference to the last element
	inline reference back(){return this->iStyleList.back();}
	inline const_reference back() const {return this->iStyleList.back();}
	
	inline void assign(const FontStyle& fontStyle){this->assign(fontStyle);}
	
	/// make an ordered/sorted add of the fontStyle at the given position into the list.
	/// returns true if the operation was successfull, returns false if the new fontStyle could not be inserted.
	/// note that the simple add will not override existing iStyleList and would return false in that case
	bool add(const FontStyle& fontStyle, size_type index);
	bool add(const iStyles::list_type& listOfCommands, size_type index);
	bool add(iStyles::list_type&& listOfCommands, size_type index);
	
	/// make an ordered/sorted add of the fontStyle at the given position into the list.
	/// returns true if the operation was successfull, returns false if the new fontStyle could not be inserted.
	/// note that this fontStyle will override an existing fontStyle and would return true in that case
	bool add_override(const FontStyle& fontStyle, size_type index);
	bool add_override(const iStyles::list_type& listOfCommands, size_type index);
	bool add_override(iStyles::list_type&& listOfCommands, size_type index);
	
	void offset_index_after(iterator itr, long offset); // long instead of size_type to also have negative numbers
	void offset_index_after(size_type index, long offset);
	
	// merges all iStyleList within the range useing add_override going from first to last
	void merge(iterator first, iterator last);
	void merge(size_type first, size_type last);

	/// returns the first found iterator to an object that has an index that is smaller or equal.
	/// if no satisfieing element has been found the end-reverse-iterator will be returned. 
	iterator find_greater_equal(size_type index);
	iterator find(size_type index);
	
	const_iterator find_greater_equal(size_type index) const;
	const_iterator find(size_type index) const;
};

}
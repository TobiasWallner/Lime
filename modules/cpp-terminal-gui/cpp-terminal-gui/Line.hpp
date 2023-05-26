
// c++ std
#include <utility>

// utf8_string
#include <utf8_string.hpp>

// Project
#include "RenderTrait.hpp"
#include "CommandList.hpp"


/*
	A line is the combination of an utf8 string and a list of 
	commands that are used to format the string at specific locations
	in the string.
	
	The line is allocator aware
*/

namespace TermGui : public RenderTrait{

class Line : RenderTrait{

private:
	using size_type = utf8::string::size_type

	using string_type = utf8::string;

	string_type _string;
	CommandList _commands;

public:
	
	/// 1) default constuctor
	inline Line() = default;
	
	/// copy constructable
	inline Line(const Line&) = default;
	
	/// copy assignable
	inline Line& operator = (const Line&) = default;
	
	/// move constructable
	inline Line(Line&&) = default;
	
	/// move assignable
	inline Line& operator = (Line&&) = default;
	
	/// Cunstruct an unformated line in any way the underlying string type can be constructed
	template<class ...Args>
	inline Line(Args... args) : _string(std::forward<Args>(args)...){}
	
	/// Constructs the Line with a substring [pos, pos + count) of other. 
	inline Line(const Line& other, size_type pos, size_type count) : 
		_string (other._string, pos, count)
		_formating(other._formating, pos, count){}
		
	/// returns true if there is nothing in the list, false otherwise
	/// the List class has to ensure that if the string is empty 
	/// that also the command list is empty too
	inline bool empty() const {return this->string.empty();}
	
	/// returns the number of utf8 characters in the string
	inline bool size() const {return this->string.size();}
	
	/// appends a line and its formats (aka. command list)
	Line& append(const Line& other);
	Line& append(Line&& other);
	
	
	/// append to the Line as you would append to the underlying string type
	template<class ...Args>
	inline Line& append(Args... args){
		this->_string.append(std::forward<Args>(args)...); 
		return *this;
	}
	
	template<class Arg>
	inline Line& operator+=(Arg args){
		this->_string += std::forward<Arg>(arg); 
		return *this;
	}
	
	template<class Arg>
	inline Line& operator<<(Arg args){
		this->_string << std::forward<Arg>(arg); 
		return *this;
	}
	
	/// appends the given command to the line which will then format all string elements inserted after wards
	inline Line& append(std::unique_ptr<Command>&& pCommand){
		//TODO: change to inser_override -> therefore implement insert_override
		this->_commands.insert(std::move(pCommand), this->_string.size());
		return *this;
	}
	
	/// wrapper for classes that are not a unique pointer yet and are moved
	/// the passed object will be 'forewareded' into the datastructure using 'std::forward'
	template<class Type>
	inline Line& append(Type&& Object){
		using RawType = typename std::remove_reference<Type>::type;
		return this->append(std::make_unique<RawType>(std::forward<Type>(Object)), this->_string.size());
	}
	
	/// push_back as you would to the underlying string 
	template<class ...Args>
	inline push_back(Args... args){this->_string.push_back(std::forward<Args>(args)...);
	
	/// append to the Line as you would to the underlying string type
	template<class ...Args>
	inline assign(Args... args){this->_string.assign(std::forward<Args>(args)...);
	
	template<class Arg>
	inline Line& operator=(Arg args){
		this->_string = std::forward<Arg>(arg); 
		return *this;
	}
	
	// TODO: iterators
	
	
	
	
	

};

}//TermGui
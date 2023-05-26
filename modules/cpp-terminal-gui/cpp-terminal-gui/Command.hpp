#pragma once

// C++ std
#include <string>

// Project
#include "RenderTrait.hpp"

namespace TermGui{

/**
	The command enum type is used to identify 
	different objects that have derived from 
	the following class 'Command'.
*/
enum class CommandType{
	ForgroundColour,
};

/**
	Abstract interface class that should provide the functionallity 
	to save and render a command to a string.
	Additionally holds the Type of the command.
	
	Derived classes have to implement the following functions:
		virtual render(std::string& outputString){}
	
*/
class Command : public RenderTrait{
private:
	const CommandType _type;
public:

	/// The default constructor of this class is deleted - cannot default construct this class
	CommandType() = delete;
	
	virtual ~Command(){}
	
	/// One may only construct an instance of this class by telling it its Command type
	explicit constexpr inline CommandType(CommandType type) : _type(type) {}
	
	/// The derived call has to specifiy how the specific Command gets rendered to a string command for the terminal
	virtual render(std::string& outputString) = 0;
	
	/// returns the command type (enum)
	constexpr inline CommandType type(){ return this->_type;}
};

}
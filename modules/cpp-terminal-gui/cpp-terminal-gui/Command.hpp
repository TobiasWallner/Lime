#pragma once

// C++ std
#include <string>

// Project
#include "RenderTrait.hpp"

//cpp-terminal
#include <cpp-terminal\color.hpp>

namespace TermGui{

/**
	The command enum type is used to identify 
	different objects that have derived from 
	the following class 'Command'.
*/
enum class CommandType{
	forgroundColor,
	backgroundColor,
};

/**
	Abstract interface class that should provide the functionallity 
	to save and render a command to a string.
	Additionally holds the Type of the command.
	
	Derived classes have to implement the following functions:
		virtual render(std::string& outputString){}
	
*/
class Command : public RenderTrait{
public:

	/// default constructable
	Command() = default;
	
	virtual ~Command(){}
	
	/// The derived call has to specifiy how the specific Command gets rendered to a string command for the terminal
	virtual void render(std::string& outputString) const override = 0;
	
	/// returns the command type (enum)
	virtual CommandType type() const = 0;
};


//
// +++++++++++++++++++ Below is the list of basic commands ++++++++++++++++++++
//

/// The forground color is a command that can be put in a command list and will then be executed by the list
class ForegroundColor : public Command{
private:
	Term::Color _color;
	
public:
	inline ForegroundColor(Term::Color color) : _color(color){}
	inline ForegroundColor(Term::Color::Name colorName) : _color(colorName){}
	inline ForegroundColor(std::uint8_t value) :  _color(value){}
	inline ForegroundColor(std::uint8_t red, std::uint8_t green, std::uint8_t blue) : _color(red, blue, green){}
	
	inline void render(std::string& output_string) const override {Term::color_fg(this->_color);}
	inline CommandType type() const override {return CommandType::forgroundColor;}
};

class BackgroundColor : public Command{
private:
	Term::Color _color;
	
public:
	inline BackgroundColor(Term::Color color) : _color(color){}
	inline BackgroundColor(Term::Color::Name colorName) : _color(colorName){}
	inline BackgroundColor(std::uint8_t value) : _color(value){}
	inline BackgroundColor(std::uint8_t red, std::uint8_t green, std::uint8_t blue) : _color(red, blue, green){}
	
	inline void render(std::string& output_string) const override {Term::color_bg(this->_color);}
	inline CommandType type() const override {return CommandType::backgroundColor;}
};

}

#pragma once

// C++ STD
#include <string>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

namespace TermGUI{

/**
	This is an Abstract class that provides a render function for Objects
	that can be rendered to the Console
*/
class RenderTrait{
	public:
	
	virtual RenderTrait() = 0;
	
	virtual ~RenderTrait(){}
	
	/**
		The object renders its whole content into a new std::string object.
		This may cause new memory allocation
		
		The offset positions the label on the screen
	*/
	virtual std::string render(Term::Cursor offset) const = 0;
	
	/**
		The object renders its content into existing memory
		
		The offset positions the label on the screen
	*/
	virtual void render(std::string& output_string) const = 0;
}	

}

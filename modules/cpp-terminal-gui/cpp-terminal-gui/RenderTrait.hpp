
#pragma once

// C++ STD
#include <string>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

namespace TermGui{

/**
	This is an Abstract class that provides a render function for Objects
	that can be rendered to the Console
	
	Derived classes have to implement the following functions:
	virtual render(std::string& outputString) = 0;
*/

struct RenderPosition{
	unsigned long x = 0;
	unsigned long y = 0;
};


struct RenderWidth{
	unsigned long x = 0;
	unsigned long y = 0;
};

class RenderTrait{
	public:
	
	RenderTrait(){};
	
	virtual ~RenderTrait(){}
	
	/**
		The object renders its content into existing memory
		
		The offset positions the label on the screen
	*/
	virtual void render(std::string& outputString) const = 0;
	
	/// sets the position of the object on the screen
	virtual void set_position(RenderPosition position) = 0;
	
	/// get the position of the object on the screen
	virtual RenderPosition get_position() const = 0;
	
	/// sets the width of the object on the screen
	virtual void set_width(RenderWidth width) = 0;
	
	/// get the render width of the object
	virtual RenderWidth get_width() const = 0;
	
};

}

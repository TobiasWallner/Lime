
#pragma once

// C++ STD
#include <string>
#include <cinttypes>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

namespace TermGui{

/**
	This is an Abstract class that provides a render function for Objects
	that can be rendered to the Console
	
	Derived classes have to implement the following functions:
	virtual render(std::string& outputString) = 0;
*/

struct ScreenPosition{
	using size_type = std::int32_t;
	
	size_type x = 0;
	size_type y = 0;
};


struct ScreenWidth{
	using size_type = std::int32_t;
	
	size_type x = 0;
	size_type y = 0;
};

class RenderTrait{
	public:
	
	RenderTrait(){};
	
	virtual ~RenderTrait(){}
	
	///The object renders its content into existing memory
	virtual void render(std::string& outputString) const = 0;
	
	/// sets the position of the object on the screen
	virtual void set_screen_position(ScreenPosition position) = 0;
	
	/// get the position of the object on the screen
	virtual ScreenPosition get_screen_position() const = 0;
	
	/// sets the width of the object on the screen
	virtual void set_screen_width(ScreenWidth width) = 0;
	
	/// get the render width of the object
	virtual ScreenWidth get_screen_width() const = 0;
	
};

}

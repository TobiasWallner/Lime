#pragma once

// C++ std
#include <string>

// project
#include "RenderTrait.hpp"
#include "ColorString.hpp"

namespace TermGui{
	
	
class Label : public RenderTrait, public ColorString{
private:
	
	ScreenPosition screenPosition = {};
	ScreenWidth screenWidth = {};
	
public:
	
	Label() = default;
	
	inline Label(ScreenPosition screenPosition, ScreenWidth screenWidth) : 
		screenPosition(screenPosition), screenWidth(screenWidth){};
	
	Label(const Label&) = default;
	Label(Label&& ) = default;
	
	Label& operator=(const Label&) = default;
	Label& operator=(Label&&) = default;
	
	void render(std::string& outputString) const;
	
	/// sets the position of the object on the screen
	void set_screen_position(ScreenPosition position);
	
	/// get the position of the object on the screen
	ScreenPosition get_screen_position() const;
	
	/// sets the width of the object on the screen
	void set_screen_width(ScreenWidth width);
	
	/// get the render width of the object
	ScreenWidth get_screen_width() const;
	
};


	
}
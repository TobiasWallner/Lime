#pragma once

// C++ std
#include <string>

// Project
#include "RenderTrait.hpp"

//cpp-terminal
#include <cpp-terminal/color.hpp>
#include <cpp-terminal/style.hpp>

namespace TermGui{
	
/**
	Abstract interface class that should provide the functionallity 
	to save and render a command to a string.
	Additionally holds the Type of the command.
	
	Derived classes have to implement the following functions:
		virtual render(std::string& outputString){}
	
*/

class FontStyle : public RenderTrait{
public:
	enum class Width : std::uint8_t{
		#ifdef NEW_CPP_TERMINAL_VERSION
		NORMAL = static_cast<std::uint8_t>(Term::Style::RESET_BOLD_DIM),
		#else
		NORMAL = 22,
		#endif
		BOLD = static_cast<std::uint8_t>(Term::Style::BOLD),
		DIM = static_cast<std::uint8_t>(Term::Style::DIM),
	};
	
	enum class Italic : std::uint8_t{
		ON = static_cast<std::uint8_t>(Term::Style::ITALIC),
		#ifdef NEW_CPP_TERMINAL_VERSION
		OFF = static_cast<std::uint8_t>(Term::Style::RESET_ITALIC),
		#else
		OFF = 23,
		#endif
	};
	
	enum class Blink : std::uint8_t{
		SLOW = static_cast<std::uint8_t>(Term::Style::BLINK),
		FAST = static_cast<std::uint8_t>(Term::Style::BLINK_RAPID),
		#ifdef NEW_CPP_TERMINAL_VERSION
		OFF = static_cast<std::uint8_t>(Term::Style::RESET_BLINKING),
		#else
		OFF = 25,
		#endif
	};
	
	enum class Reversed : std::uint8_t{
		ON = static_cast<std::uint8_t>(Term::Style::REVERSED),
		#ifdef NEW_CPP_TERMINAL_VERSION
		OFF = static_cast<std::uint8_t>(Term::Style::RESET_REVERSED),
		#else
		OFF = 27,
		#endif
	};
	
	enum class Conceal : std::uint8_t{
		ON = static_cast<std::uint8_t>(Term::Style::CONCEAL),
		#ifdef NEW_CPP_TERMINAL_VERSION
		OFF = static_cast<std::uint8_t>(Term::Style::RESET_CONCEAL),
		#else
		OFF = 28,
		#endif
	};
	
	enum class Crossed : std::uint8_t{
		ON = static_cast<std::uint8_t>(Term::Style::CROSSED),
		#ifdef NEW_CPP_TERMINAL_VERSION
		OFF = static_cast<std::uint8_t>(Term::Style::RESET_CROSSED),
		#else
		OFF = 29,
		#endif
	};
	
	enum class Underline : std::uint8_t{
		ON = static_cast<std::uint8_t>(Term::Style::UNDERLINE),
		#ifdef NEW_CPP_TERMINAL_VERSION
		OFF = static_cast<std::uint8_t>(Term::Style::RESET_UNDERLINE),
		#else
		OFF = 24,
		#endif
	};
	
	enum class Font : std::uint8_t{
		#ifdef NEW_CPP_TERMINAL_VERSION
		F0 = static_cast<std::uint8_t>(Term::Style::FONT_0),
		F1 = static_cast<std::uint8_t>(Term::Style::FONT_1),
		F2 = static_cast<std::uint8_t>(Term::Style::FONT_2),
		F3 = static_cast<std::uint8_t>(Term::Style::FONT_3),
		F4 = static_cast<std::uint8_t>(Term::Style::FONT_4),
		F5 = static_cast<std::uint8_t>(Term::Style::FONT_5),
		F6 = static_cast<std::uint8_t>(Term::Style::FONT_6),
		F7 = static_cast<std::uint8_t>(Term::Style::FONT_7),
		F8 = static_cast<std::uint8_t>(Term::Style::FONT_8),
		F9 = static_cast<std::uint8_t>(Term::Style::FONT_9),
		F10 = static_cast<std::uint8_t>(Term::Style::FONT_10),
		#else
		F0 = 11,
		F1 = 12,
		F2 = 13,
		F3 = 14,
		F4 = 15,
		F5 = 16,
		F6 = 17,
		F7 = 18,
		F8 = 19,
		F9 = 20,
		F10 = 21,
		#endif
	};
	
	enum class Bar : std::uint8_t{
		#ifdef NEW_CPP_TERMINAL_VERSION
		LEFT = static_cast<std::uint8_t>(Term::Style::BAR_LEFT),
		RIGHT = static_cast<std::uint8_t>(Term::Style::BAR_RIGHT),
		OFF = static_cast<std::uint8_t>(Term::Style::RESET_BAR)
		#else
		LEFT = 63,
		RIGHT = 60,
		OFF = 65, 
		#endif
	};
	
private:
	struct Reset_t{};
public:
	static constexpr Reset_t Reset{}; // empty type used as a token
	
	enum class Type : std::uint8_t{
		reset, 
		fgColor,
		bgColor,
		fontWidth,
		italic,
		underline,
		blink,
		reversed,
		conceal,
		crossed,
		font,
		Default,
		bar,
	};
	
private:
	Type styleType;
	std::uint8_t param[3];
	
	inline FontStyle(Type type, std::uint8_t param1 = 0, std::uint8_t param2 = 0, std::uint8_t param3 = 0) : 
		styleType(type), param{param1, param2, param3}{}
	
public:
	FontStyle() = default;
	
	inline FontStyle(const FontStyle&) = default;
	inline FontStyle& operator=(const FontStyle&) = default;
	
	//---- foreground color ----
	static inline FontStyle fg_color(std::uint8_t red, std::uint8_t green, std::uint8_t blue){
		return FontStyle(Type::fgColor, red, green, blue);
	}
	
	static inline FontStyle default_fg_color(){
		#ifdef NEW_CPP_TERMINAL_VERSION
		return FontStyle(Type::Default, static_cast<std::uint8_t>(Term::Style::DEFAULT_FOREGROUND_COLOR));
		#else
		return FontStyle(Type::Default, static_cast<std::uint8_t>(39));
		#endif
	}
	
	//---- background color ----
	static inline FontStyle bg_color(std::uint8_t red, std::uint8_t green, std::uint8_t blue){
		return FontStyle(Type::bgColor, red, green, blue);
	}
	
	static inline FontStyle default_bg_color(){
		#ifdef NEW_CPP_TERMINAL_VERSION
		return FontStyle(Type::Default, static_cast<std::uint8_t>(Term::Style::DEFAULT_BACKGROUND_COLOR));
		#else
		return FontStyle(Type::Default, static_cast<std::uint8_t>(49));
		#endif
	}

	
	inline FontStyle(Reset_t)				: FontStyle(Type::reset, static_cast<std::uint8_t>(Term::Style::RESET)){}
	inline FontStyle(Width width) 			: FontStyle(Type::fontWidth, static_cast<std::uint8_t>(width)){}
	inline FontStyle(Italic italic) 		: FontStyle(Type::italic, static_cast<std::uint8_t>(italic)){}
	inline FontStyle(Underline underline) 	: FontStyle(Type::underline, static_cast<std::uint8_t>(underline)){}
	inline FontStyle(Blink blink) 			: FontStyle(Type::blink, static_cast<std::uint8_t>(blink)){}
	inline FontStyle(Reversed reversed) 	: FontStyle(Type::reversed, static_cast<std::uint8_t>(reversed)){}
	inline FontStyle(Conceal conceal) 		: FontStyle(Type::conceal, static_cast<std::uint8_t>(conceal)){}
	inline FontStyle(Crossed crossed)		: FontStyle(Type::crossed, static_cast<std::uint8_t>(crossed)){}
	inline FontStyle(Font font)				: FontStyle(Type::font, static_cast<std::uint8_t>(font)){}
	inline FontStyle(Bar bar)				: FontStyle(Type::bar, static_cast<std::uint8_t>(bar)){}
	
	inline Type type() const {return this->styleType;}
	
	inline friend bool operator==(FontStyle lhs, FontStyle rhs){
		return lhs.styleType == rhs.styleType && 
				lhs.param[0] == rhs.param[0] &&
				lhs.param[1] == rhs.param[1] && 
				lhs.param[2] == rhs.param[2];
	}
	
	inline friend bool operator!=(FontStyle lhs, FontStyle rhs){
		return !(lhs == rhs);
	}
	
	/// The derived call has to specifiy how the specific Command gets rendered to a string command for the terminal
	inline void render(std::string& outputString) const {
		switch(this->type()){
			case Type::fgColor : {
				outputString.append(Term::color_fg(Term::Color(this->param[0], this->param[1], this->param[2])));
			}break;
			case Type::bgColor : {
				outputString.append(Term::color_bg(Term::Color(this->param[0], this->param[1], this->param[2])));
			}break;
			default : {
				outputString.append(Term::style(static_cast<Term::Style>(this->param[0])));
			}break;
		}
	}
};

//---- foreground color ----
inline FontStyle fg_color(std::uint8_t red, std::uint8_t green, std::uint8_t blue){return FontStyle::fg_color(red, green, blue);}

inline FontStyle default_fg_color(){return FontStyle::default_fg_color();}

//---- background color ----
inline FontStyle bg_color(std::uint8_t red, std::uint8_t green, std::uint8_t blue){return FontStyle::bg_color(red, green, blue);}

inline FontStyle default_bg_color(){return FontStyle::default_bg_color();}

}
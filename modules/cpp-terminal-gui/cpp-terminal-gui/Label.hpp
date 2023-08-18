#pragma once

// C++ std
#include <string>
#include <algorithm>

// project
#include "RenderTrait.hpp"
#include "ColorString.hpp"

namespace TermGui{
	
	
class Label : public RenderTrait {
public:
	using size_type = ColorString::size_type;
	
private:
	
	ColorString _string;
	
	ScreenPosition screenPosition = {};
	ScreenWidth screenWidth = {};
	
	ScreenWidth::size_type internalHeight = 0;
	ScreenWidth::size_type internalWidth = 0;
	ScreenWidth::size_type insertLineWidth = 0;
	
	static constexpr int tabsize = 4;
	
public:
	
	
	
	Label() = default;
	
	inline Label(ScreenPosition screenPosition, ScreenWidth screenWidth) : 
		screenPosition(screenPosition), screenWidth(screenWidth){}
	
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
	
	inline bool empty() const { return this->_string.empty(); }
	inline size_type size() const { return this->_string.size(); }
	inline void clear(){ this->_string.clear(); }
	
	template<class CharItr>
	inline void update_wanted_screen_width_on_append(CharItr first, const CharItr last){
		for(; first != last; ++first){
			if(*first == '\n'){
				++this->wantedHeight;
				this->wantedWidth = std::max(this->wantedWidth, this->insertLineWidth);
				this->insertLineWidth = 0;
			}else if(*first == '\t'){
				this->insertLineWidth += this->tabsize;
			}else{
				++this->insertLineWidth;
			}
		}
		this->wantedWidth = std::max(this->wantedWidth, this->insertLineWidth);
	}
	
	template<class CharItr>
	inline void update_wanted_screen_width_on_assign(CharItr first, const CharItr last){
		this->wantedHeight = 0;
		this->wantedWidth = 0;
		this->insertLineWidth = 0;
		update_wanted_screen_width_on_append(first, last);
	}
	
	inline void update_wanted_screen_width_on_append(const char* first){
		for(; *first != '\0'; ++first){
			if(*first == '\n'){
				++this->wantedHeight;
				this->wantedWidth = std::max(this->wantedWidth, this->insertLineWidth);
				this->insertLineWidth = 0;
			}else if(*first == '\t'){
				this->insertLineWidth += this->tabsize;
			}else{
				++this->insertLineWidth;
			}
		}
		this->wantedWidth = std::max(this->wantedWidth, this->insertLineWidth);
	}
	
	inline void update_wanted_screen_width_on_assign(const char* first){
		this->wantedHeight = 0;
		this->wantedWidth = 0;
		this->insertLineWidth = 0;
		update_wanted_screen_width_on_append(first);
	}
	
	inline Label& append(const ColorString& other) { 
		update_wanted_screen_width_on_append(other.string_cbegin(), other.string_cend());
		this->_string.append(other); 
		return *this;
	}
	
	inline Label& append(ColorString&& other) { 
		update_wanted_screen_width_on_append(other.string_cbegin(), other.string_cend());
		this->_string.append(std::move(other)); 
		return *this;
	}
	
	template<class CharItr>
	inline Label& append(CharItr first, CharItr last){
		update_wanted_screen_width_on_append(first, last);
		this->_string.append(first, last); 
		return *this;
	}
	
	inline Label& append(utf8::Char c){
		update_wanted_screen_width_on_append(&c, &c + 1);
		this->_string.append(c); 
		return *this;
	}
	
	inline Label& append(char c){
		update_wanted_screen_width_on_append(&c, &c + 1);
		this->_string.append(c); 
		return *this;
	}
	
	inline Label& append(const char* first){
		update_wanted_screen_width_on_append(first);
		this->_string.append(first); 
		return *this;
	}
	
	inline Label& append(const char* first, size_type n){
		update_wanted_screen_width_on_append(first, first + n);
		this->_string.append(first, first + n); 
		return *this;
	}
	
	inline Label& append(const std::string& str){
		update_wanted_screen_width_on_append(str.begin(), str.end());
		this->_string.append(str); 
		return *this;
	}
	
	inline Label& append(std::string_view str){
		update_wanted_screen_width_on_append(str.begin(), str.end());
		this->_string.append(str); 
		return *this;
	}
	
	inline Label& append(const utf8::string str){
		update_wanted_screen_width_on_append(str.begin(), str.end());
		this->_string.append(str); 
		return *this;
	}
	
	inline Label& append(const utf8::string_view str){
		update_wanted_screen_width_on_append(str.begin(), str.end());
		this->_string.append(str); 
		return *this;
	}
	
	inline Label& operator+=(utf8::Char c){return this->append(c);}
	inline Label& operator+=(char c){return this->append(c);}
	inline Label& operator+=(const char* first){return this->append(first);}
	inline Label& operator+=(const std::string& str){return this->append(str);}
	inline Label& operator+=(std::string_view str){return this->append(str);}
	inline Label& operator+=(const utf8::string str){return this->append(str);}
	inline Label& operator+=(const utf8::string_view str){return this->append(str);}
	
	inline Label& operator<<(utf8::Char c){return this->append(c);}
	inline Label& operator<<(char c){return this->append(c);}
	inline Label& operator<<(const char* first){return this->append(first);}
	inline Label& operator<<(const std::string& str){return this->append(str);}
	inline Label& operator<<(std::string_view str){return this->append(str);}
	inline Label& operator<<(const utf8::string str){return this->append(str);}
	inline Label& operator<<(const utf8::string_view str){return this->append(str);}
	
	
	
	inline Label& append(const FontStyle& fontStyle){this->_string.append(fontStyle); return *this;}
	inline Label& operator << (const FontStyle& fontStyle){ return this->append(fontStyle); }
	inline Label& operator += (const FontStyle& fontStyle){ return this->append(fontStyle); }
	
	inline Label& assign(const FontStyle& command){
		this->_string.assign(command); 
		return *this;
	}
	
	inline Label& operator=(const FontStyle& command){
		return this->assign(command);
	}

	// ---------- refactor all functions down below ----------
	
	/// assigning any unformated and uncolored string to a string will clear the previous stored formats and colores
	template<class CharItr>
	inline Label& assign(CharItr first, CharItr last){
		update_wanted_screen_width_on_assign(first, last);
		this->_string.assign(first, last);
		return *this;
	}
	
	inline Label& assign(utf8::Char c){
		update_wanted_screen_width_on_assign(&c, &c + 1);
		this->_string.assign(c);
		return *this;
	}
	
	inline Label& assign(char c){
		update_wanted_screen_width_on_assign(&c, &c + 1);
		this->_string.assign(c);
		return *this;
	}
	
	inline Label& assign(const char* first){
		update_wanted_screen_width_on_assign(first);
		this->_string.assign(first);
		return *this;
	}
	
	inline Label& assign(const char* first, size_type n){
		update_wanted_screen_width_on_assign(first, first + n);
		this->_string.assign(first, first + n);
		return *this;
	}
	
	inline Label& assign(const std::string& str){
		update_wanted_screen_width_on_assign(str.begin(), str.end());
		this->_string.assign(str);
		return *this;
	}
	
	inline Label& assign(std::string_view str){
		update_wanted_screen_width_on_assign(str.begin(), str.end());
		this->_string.assign(str);
		return *this;
	}
	
	
	inline Label& assign(const utf8::string str){
		update_wanted_screen_width_on_assign(str.begin(), str.end());
		this->_string.assign(str);
		return *this;
	}
	
	inline Label& assign(const utf8::string_view str){
		update_wanted_screen_width_on_assign(str.begin(), str.end());
		this->_string.assign(str);
		return *this;
	}
	
	inline Label& operator=(utf8::Char c){return this->assign(c);}
	inline Label& operator=(char c){return this->assign(c);}
	inline Label& operator=(const char* first){return this->assign(first);}
	inline Label& operator=(const std::string& str){return this->assign(str);}
	inline Label& operator=(std::string_view str){return this->assign(str);}
	inline Label& operator=(const utf8::string str){return this->assign(str);}
	inline Label& operator=(const utf8::string_view str){return this->assign(str);}

	
};


	
}
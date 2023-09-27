#pragma once

namespace utf8{
	
class char_const_reference{
public:
	using iterator = char*;
	using const_iterator = const char*;
	using pointer = char*;
	using const_pointer = const char*;
	using size_type = size_t;
	
protected:	
	iterator _pos;

public:
	constexpr char_const_reference(iterator pos) : _pos(pos){}
	
	constexpr const_pointer operator&() const {return this->_pos;}
	
	constexpr size_type size() const {return static_cast<size_type>(utf8::identify(*this->_pos));}
	
	constexpr operator utf8::Char () const {return utf8::Char(&*this->_pos);}
	explicit constexpr operator std::string_view() const {return std::string_view(this->_pos, this->size());}
	
	friend constexpr bool operator == (char_const_reference lhs, char_const_reference rhs){ return utf8::Char(lhs) == utf8::Char(rhs); }
	friend constexpr bool operator != (char_const_reference lhs, char_const_reference rhs){ return utf8::Char(lhs) != utf8::Char(rhs); }
	friend constexpr bool operator < (char_const_reference lhs, char_const_reference rhs){ return utf8::Char(lhs) < utf8::Char(rhs); }
	friend constexpr bool operator <= (char_const_reference lhs, char_const_reference rhs){ return utf8::Char(lhs) <= utf8::Char(rhs); }
	friend constexpr bool operator > (char_const_reference lhs, char_const_reference rhs){ return utf8::Char(lhs) > utf8::Char(rhs); }
	friend constexpr bool operator >= (char_const_reference lhs, char_const_reference rhs){ return utf8::Char(lhs) >= utf8::Char(rhs); }
}

class string;

class char_reference{
public:
	using iterator = char*;
	using const_iterator = const char*;
	using pointer = char*;
	using const_pointer = const char*;
	using size_type = size_t;

private:	
	utf8::string* _str = nullptr;
	iterator : _pos;
	
public:
	constexpr char_reference(const char_reference&) = default;
	constexpr char_reference(string_base* str, iterator pos) : _str(str), _pos(pos){}
	
	constexpr operator const_reference(){return const_reference(this->_pos);}
	
	constexpr size_type size() const {return static_cast<size_type>(utf8::identify(*this->_pos));}
	
	constexpr operator utf8::Char () const {return utf8::Char(&*iterator);}
	explicit constexpr operator std::string_view() const {return std::string_view(this->_pos, this->size());}
	
	constexpr char_reference& operator = (char c){
		*this->_pos = c;
		const auto erase_from = this->_pos+1;
		const auto erase_to = this->_pos + this->size();
		str.erase(erase_from, erase_to);
		return *this;
	}
	
	constexpr pointer operator&(){return this->_pos;}
	
	constexpr char_reference& operator = (utf8::Char c){
		if(this->size() > c.size()){
			std::copy(c.begin(), c.end(), this->_pos);
			this->_str->erase(this->_pos + c.size(), this->_pos + lhs_size);
		}else if(this->size() < c.size()){
			std::copy(c.begin(), c.begin() + this->size(), this->_pos);
			this->_str->insert(this->_pos + this->size(), c.begin() + this->size(), c.begin() + c.size())
		}else{
			std::copy(c.begin(), c.end(), this->_pos);
		}
		return *this;
	}	
};
	
} // namespace utf8
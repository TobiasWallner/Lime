#pragma once

namespace utf8{
	
class const_reference{
public:
	using iterator = char*;
	using const_iterator = const char*;
	using pointer = char*;
	using const_pointer = const char*;
	using size_type = size_t;
	
protected:	
	iterator _pos;

public:
	constexpr const_reference(iterator pos) : _pos(pos){}
	
	constexpr size_type size() const {return 1;}
	constexpr size_type byte_size() const {
		switch(utf8::identify(*this->_pos)){
			case Unsupported :
				throw std::runtime_error("Error: utf8::string_base::reference.byte_size(): encountered an unsupported unicode character."); 
				break;
			
			case NotFirst :
				throw std::runtime_error("Error: utf8::string_base::reference.byte_size(): encountered an unicode character that should not be at the start of an utf8 character"); 
				break;
			
			case Bytes1 :
				return 1;
				break;
			
			case Bytes2 :
				return 2;
				break;
			
			case Bytes3 : 
				return 3;
				break;
			
			case Bytes4 : 
				return 4;
				break;
			
			default :
				throw std::runtime_error("Error: utf8::string_base::reference.byte_size(): unhandled switch statement"); 
				break;
		}
	}
	
	constexpr operator utf8::Char () const {return utf8::Char(&*iterator);}
	constexpr operator utf8::string_view() const {return utf8::string_view(this->_pos, this->byte_size(), this->size())}
	constexpr operator std::string_view() const {return std::string_view(this->_pos, this->byte_size())}
}

class reference{
public:
	using iterator = char*;
	using const_iterator = const char*;
	using pointer = char*;
	using const_pointer = const char*;
	using size_type = size_t;

private:	
	string_base& _str;
	iterator : _pos;
	
public:
	reference(const reference&) = default;
	reference(string_base& str, iterator pos) : _str(str), _pos(pos){}
	
	constexpr size_type size() const {return 1;}
	constexpr size_type byte_size() const {
		switch(utf8::identify(*this->_pos)){
			case Unsupported :
				throw std::runtime_error("Error: utf8::string_base::reference.byte_size(): encountered an unsupported unicode character."); 
				break;
			
			case NotFirst :
				throw std::runtime_error("Error: utf8::string_base::reference.byte_size(): encountered an unicode character that should not be at the start of an utf8 character"); 
				break;
			
			case Bytes1 :
				return 1;
				break;
			
			case Bytes2 :
				return 2;
				break;
			
			case Bytes3 : 
				return 3;
				break;
			
			case Bytes4 : 
				return 4;
				break;
			
			default :
				throw std::runtime_error("Error: utf8::string_base::reference.byte_size(): unhandled switch statement"); 
				break;
		}
	}
	
	constexpr operator utf8::Char () const {return utf8::Char(&*iterator);}
	constexpr operator utf8::string_view() const {return utf8::string_view(this->_pos, this->byte_size(), this->size())}
	constexpr operator std::string_view() const {return std::string_view(this->_pos, this->byte_size())}
	
	reference& operator = (char c){
		switch(utf8::identify(*this->_pos)){
			case Unsupported :
				throw std::runtime_error("Error: utf8::string_base::reference.operator=(char c): encountered an unsupported unicode character."); 
				break;
			
			case NotFirst :
				throw std::runtime_error("Error: utf8::string_base::reference.operator=(char c): encountered an unicode character that should not be at the start of an utf8 character"); 
				break;
			
			case Bytes1 :
				*_pos = c;
				break;
			
			case Bytes2 :
				*_pos = c;
				str.erase(this->_pos + 1, this->_pos + 2);
				break;
			
			case Bytes3 : 
				*_pos = c;
				str.erase(this->_pos + 1, this->_pos + 3);
				break;
			
			case Bytes4 : 
				*_pos = c;
				str.erase(this->_pos + 1, this->_pos + 4);
				break;
			
			default :
				throw std::runtime_error("Error: utf8::string_base::reference.operator=(char c): unhandled switch statement"); 
				break;
		}
		return *this;
	}
	
	reference& operator = (utf8::Char c){
		long this_size = 0;
		switch(utf8::identify(*this->_pos)){
			case Unsupported :
				throw std::runtime_error("Error: utf8::string_base::reference.operator=(char c): encountered an unsupported unicode character."); break;
			
			case NotFirst :
				throw std::runtime_error("Error: utf8::string_base::reference.operator=(char c): encountered an unicode character that should not be at the start of an utf8 character"); break;
			
			case Bytes1 :
				this_size = 1;
				break;
			
			case Bytes2 :
				this_size = 2;
				break;
			
			case Bytes3 : 
				this_size = 3;
				break;
			
			case Bytes4 : 
				this_size = 4;
				break;
			
			default :
				throw std::runtime_error("Error: utf8::string_base::reference.operator=(char c): unhandled switch statement"); break;
		}
		
		long rhs_size = c.size();
		
		if(this_size > rhs_size){
			std::copy(c.begin(), c.end(), this->_pos);
			this->_str.erase(this->_pos + rhs_size, this->_pos + lhs_size);
		}else if(this_size < rhs_size){
			std::copy(c.begin(), c.begin() + this_size, this->_pos);
			this->_str.insert(this->_pos + this_size, c.begin() + this_size, c.begin() + rhs_size)
		}else{
			std::copy(c.begin(), c.end(), this->_pos);
		}
		
		return *this;
	}	
};
	
} // namespace utf8
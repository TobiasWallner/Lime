
#include <cinttypes>
#include <string>
#include <ostream>

namespace utf8{

namespace Private{
	
	/// reads a char type that should be the first of an utf8 symbol and returns the length in bytes 
	/// that are needed to represent that symbol
	constexpr int utf8_length(char c){
		const auto prefix = (c & 0xF0) >> 4;
		const char lookupTable[16]{
			/*0000*/	1,
			/*0001*/	1,
			/*0010*/	1,
			/*0011*/	1,
			/*0100*/	1,
			/*0101*/	1,
			/*0110*/	1,
			/*0111*/	1,
			/*1000*/	2,
			/*1001*/	2,
			/*1010*/	2,
			/*1011*/	2,
			/*1100*/	3,
			/*1101*/	3,
			/*1110*/	4,
			/*1111*/	4
		};
		const auto lookup = lookupTable[prefix];
		const auto result = static_cast<int>(lookup);
		return result;
	}
}

/*
	Stores an UTF8 symbol in a 4-byte wide buffer.
	
	It works and functions mostly as a container and also provides most standard container functions.
	
	It also has multiple functions that writes and prints this character into new or existing char strings,
	output streams and the like.
	
*/
class Char{
private:
	char utf8[4] {};
	
public:
	using value_type = char;
	using size_type = std::size_t;
	using iterator = char*;
	using const_iterator = const char*;
	using reference = char&;
	using const_reference = const char&;
	using pointer = char*;
	using const_pointer = const char*;

	/// default constuctor constructs a null character
	Char() = default;
	
	/// supports default copy construction
	Char(const Char&) = default;
	
	/// supports default copy assignment
	Char& operator=(const Char&) = default;
	
	/// construct from an ascii character
	explicit constexpr inline Char(char ascii) {utf8[0] = ascii;}
	
	/// reads one utf8 character from the range and stores it in the char
	Char(const char* first, const char* last){
		// constexpr: because it can be done at compile time
		// inline: because it is very short
		const char* itr = first;
		const auto length = utf8_length(*begin);
		for(int i = 0; i != length && first != end; ++i, (void)(++first)){
			this->utf8[i] = *first;
		}
	}
	
	/// assigns the ascii character to the utf8 character
	constexpr inline void assign(char ascii){utf8[0] = ascii;}
	
	/// assigns an ascii character to the utf8 character
	constexpr inline Char& operator=(char c){ 
		this->assign(c); 
		return *this; 
	}
	
	/// returns the first character of the utf8 character string
	constexpr inline char& front() { return this->utf8[0]; }
	constexpr inline const char& front() const {return this->utf8[0]}
	
	/// returns the begin iterator of the utf8 character string
	constexpr inline iterator begin() {return this->utf8;}
	constexpr inline const_iterator begin() const {return this->utf8;}
	constexpr inline const_iterator beginc() const {return this->utf8;}

	/// returns the end iterator of the utf8 character string
	constexpr inline iterator end() {return this->begin() + this->size();}
	constexpr inline const_iterator end() const {return this->begin() + this->size();}
	constexpr inline const_iterator cend() const {return this->begin() + this->size();}
	
	
	/// returns the character at the index of the utf8 string
	constexpr inline reference operator[](std::size_t index){return this->utf8[index];}
	constexpr inline const_reference operator[](std::size_t index) const {return this->utf8[index];}
	
	/// returns the number of bytes that the unicode character needs
	constexpr inline int size() const {
		const auto first_char = this->front();
		const auto result = utf8_length(first_char);
		return result;
	}
	
	/// provides a pointer to the underlieing data
	constexpr inline pointer data() {return this->utf8;}
	constexpr inline const_pointer data() const {return this->utf8;}
	
	/// writes this character into an std::string
	inline void print(std::string str) const {str.append(this->utf8, this->length());}
	
	/// turns this utf8 character string into an std::string
	inline void std::string to_string() const {return std::string(this->utf8, this->length());}
	
	/// writes this char into an std::ostream useing the stream operator <<
	template<class OStream>
	friend inline std::ostream& operator << (OStream& stream, Char c) const {stream.write(this->utf8, c.length()); return stream;}
	
	/// reads an utf8 char from the input stream
	template<class IStream>
	friend std::istream& operator >> (IStream& stream, Char& c) const {
		c[0] = stream.get();
		const auto length = utf8_length(c[0]);
		for(int i = 1; i != length; ++i){
			c[i] = stream.get();
		}
		return stream;
	}
	
};

class String{
	
	
	
};

}
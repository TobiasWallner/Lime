
#pragma once

// c++ std
#include <cinttypes>
#include <string>
#include <string_view>
#include <ostream>
#include <cinttypes>

namespace utf8{
	

enum class Identifier : std::int8_t{
	Unsupported = -1,
	NotFirst 	= 0,
	Bytes1 		= 1,
	Bytes2 		= 2,
	Bytes3 		= 3,
	Bytes4 		= 4
};	

/// returns the length of the utf8 sequence if it is the first character
/// returns '0' if it is a character in the sequence but not the first one
/// returns '-1' if it is an unsupported character by the utf8 standard
constexpr Identifier identify(char c){
	const auto prefix = (c & 0b11111000) >> 3;
	const Identifier lookupTable[32]{
		/*00000*/	Identifier::Bytes1,
		/*00001*/	Identifier::Bytes1,
		/*00010*/	Identifier::Bytes1,
		/*00011*/	Identifier::Bytes1,
		/*00100*/	Identifier::Bytes1,
		/*00101*/	Identifier::Bytes1,
		/*00110*/	Identifier::Bytes1,
		/*00111*/	Identifier::Bytes1,
		/*01000*/	Identifier::Bytes1,
		/*01001*/	Identifier::Bytes1,
		/*01010*/	Identifier::Bytes1,
		/*01011*/	Identifier::Bytes1,
		/*01100*/	Identifier::Bytes1,
		/*01101*/	Identifier::Bytes1,
		/*01110*/	Identifier::Bytes1,
		/*01111*/	Identifier::Bytes1,
		
		/*10000*/	Identifier::NotFirst,
		/*10001*/	Identifier::NotFirst,
		/*10010*/	Identifier::NotFirst,
		/*10011*/	Identifier::NotFirst,
		/*10100*/	Identifier::NotFirst,
		/*10101*/	Identifier::NotFirst,
		/*10110*/	Identifier::NotFirst,
		/*10111*/	Identifier::NotFirst,
		/*11000*/	Identifier::Bytes2,
		/*11001*/	Identifier::Bytes2,
		/*11010*/	Identifier::Bytes2,
		/*11011*/	Identifier::Bytes2,
		/*11100*/	Identifier::Bytes3,
		/*11101*/	Identifier::Bytes3,
		/*11110*/	Identifier::Bytes4,
		/*11111*/	Identifier::Unsupported,
	};
	const auto result = lookupTable[prefix];
	return result;
}

/**
	Stores an UTF8 symbol in a 4-byte wide buffer.
	
	It works and functions mostly as a container and also provides most standard container functions.
	
	It also has multiple functions that writes and prints this character into new or existing char strings,
	output streams and the like.
	
*/
class Char{
private:
	char utf8[4];
	
public:
	using value_type = char;
	using int_type = std::uint32_t;
	using size_type = std::size_t;
	using iterator = char*;
	using const_iterator = const char*;
	using reference = char&;
	using const_reference = const char&;
	using pointer = char*;
	using const_pointer = const char*;

	/// default constuctor constructable
	inline Char() = default;
	
	/// supports default copy construction
	inline Char(const Char&) = default;
	
	/// supports default copy assignment
	inline Char& operator=(const Char&) = default;
	
	/// construct from an ascii character
	inline explicit Char(char ascii) {utf8[0] = ascii;}
	
	/// reads one utf8 character from the range and stores it in the char
	template<class CharItr>
	inline explicit Char(CharItr first){this->assign(first);}
	
	/// reads one utf8 character from the range and stores it in the char, 
	/// makes sure to not read more than the buffer
	template<class CharItr>
	constexpr inline Char(CharItr first, CharItr last){this->assign(first, last);}
	
	template<class CharItr>
	constexpr inline Char(CharItr first, size_type n){this->assign(first, first + n);}
	
	/// reads first utf8 character from the string and makes sure not to read more than the strings content
	inline explicit Char(const std::string& str){this->assign(str);}
	
	/// reads first utf8 character from the string and makes sure not to read more than the strings content
	inline explicit Char(const std::string_view& str){this->assign(str);}
	
	/// assigns the ascii character to the utf8 character
	constexpr inline void assign(char ascii){utf8[0] = ascii;}
	
	/// reads the first utf8 character and stores it in the character, within the range
	/// returns the iterator to the next character after that.
	/// returns the same first iterator if the provided character is not the first byte of an utf8 encoded character
	template<class CharItr>
	constexpr inline CharItr assign(CharItr first, CharItr last){
		const auto identifier = identify(static_cast<char>(*first));
		if((identifier == Identifier::Unsupported) | (identifier == Identifier::NotFirst)){
			return first;
		}else{
			const auto length = static_cast<size_type>(identifier);
			for(int i = 0; i != length && first != last && i < 4; ++i, (void)(++first)){
				this->utf8[i] = static_cast<char>(*first);
			}
			return first;
		}		
	}
	
	/// reads the first utf8 character and stores it in the character
	/// returns the iterator to the next character after that.
	/// returns the same iterator if the character is not a valid utf8 character
	template<class CharItr>
	constexpr inline CharItr assign(CharItr first){
		const auto identifier = identify(static_cast<char>(*first));
		if((identifier == Identifier::Unsupported) | (identifier == Identifier::NotFirst)){
			return first;
		}else{
			const auto length = static_cast<size_type>(identifier);
			for(int i = 0; i != length && i < 4; ++i, (void)(++first)){
				this->utf8[i] = static_cast<char>(*first);
			}
			return first;
		}	
	}
	
	/// reads the first utf8 character from the string and stores it in the character
	/// returns the string iterator after the read character
	inline std::string::const_iterator assign(const std::string& str){return this->assign(str.cbegin(), str.cend());}
	
	/// reads the first utf8 character from the string and stores it in the character
	/// returns the string iterator after the read character
	inline std::string_view::const_iterator assign(const std::string_view& str){return this->assign(str.cbegin(), str.cend());}
	
	
	/// assigns an ascii character to the utf8 character
	constexpr inline Char& operator=(char c){ this->assign(c); return *this; }
	
	template<class CharItr>
	constexpr inline Char& operator=(CharItr itr){ this->assign(itr); return *this; }
	
	inline Char& operator=(const std::string& str){ this->assign(str); return *this; }
	inline Char& operator=(const std::string_view& str){ this->assign(str); return *this; }
	
	/// returns the first character of the utf8 character string
	constexpr inline char& front() { return this->utf8[0]; }
	constexpr inline const char& front() const {return this->utf8[0];}
	
	/// returns the begin iterator of the utf8 character string
	constexpr inline iterator begin() {return this->utf8;}
	constexpr inline const_iterator begin() const {return this->utf8;}
	constexpr inline const_iterator cbegin() const {return this->utf8;}

	/// returns the end iterator of the utf8 character string
	constexpr inline iterator end() {return this->begin() + this->size();}
	constexpr inline const_iterator end() const {return this->begin() + this->size();}
	constexpr inline const_iterator cend() const {return this->begin() + this->size();}
	
	/// returns a string view object pointing to the utf8 symbol
	constexpr inline std::string_view to_std_string_view() const {return std::string_view(this->begin(), this->size());}
	
	/// returns the character at the index of the utf8 string
	constexpr inline reference operator[](size_type index){return this->utf8[index];}
	constexpr inline const_reference operator[](size_type index) const {return this->utf8[index];}
	
	/// returns the number of bytes that the unicode character needs
	constexpr inline size_type size() const {
		const auto first_char = this->front();
		const auto result = identify(first_char);
		return static_cast<size_type>(result);
	}
	
	/// provides a pointer to the underlieing data
	constexpr inline pointer data() {return this->utf8;}
	constexpr inline const_pointer data() const {return this->utf8;}
	
	/// writes this character into an std::string
	inline void print(std::string& str) const {str.append(this->utf8, this->size());}
	
	/// turns this utf8 character string into an std::string
	inline std::string to_std_string() const {return std::string(this->utf8, this->size());}
	
	/// writes this char into an std::ostream useing the stream operator <<
	template<class OStream>
	friend inline std::ostream& operator << (OStream& stream, Char c) {
		stream.write(c.utf8, c.size()); return stream;
	}
	
	/// reads an utf8 char from the input stream
	template<class IStream>
	friend std::istream& operator >> (IStream& stream, Char& c) {
		c[0] = stream.get();
		const auto length = identify(c[0]);
		switch(length){
			case 2: {
				c[1] = stream.get();
			}break;
			case 3: {
				c[1] = stream.get();
				c[2] = stream.get();
			}break;
			case 4: {
				c[1] = stream.get();
				c[2] = stream.get();
				c[3] = stream.get();
			}break;
		}
		return stream;
	}
	
	constexpr inline int_type to_int() const {
		switch(this->size()){
			case 1: return (static_cast<int_type>(utf8[0]));
			case 2: return (static_cast<int_type>(utf8[0] & 0b01111111) << 8) 
							| static_cast<int_type>(utf8[1]);
			case 3: return (static_cast<int_type>(utf8[0] & 0b00111111) << 16)
							| (static_cast<int_type>(utf8[1]) << 8)
							| (static_cast<int_type>(utf8[2]));
			case 4: return (static_cast<int_type>(utf8[0] & 0b00011111) << 24)
							| (static_cast<int_type>(utf8[1]) << 16)
							| (static_cast<int_type>(utf8[2]) << 8)
							| (static_cast<int_type>(utf8[3]));
			default: return -1;
		}	
	}
	
	/// equality comparison with the same type
	friend constexpr inline bool operator==(Char lhs, Char rhs){return lhs.to_int() == rhs.to_int();}
	
	friend constexpr inline bool operator==(Char lhs, char rhs){return lhs.to_int() == static_cast<Char::int_type>(rhs);}
	friend constexpr inline bool operator==(char lhs, Char rhs){return (rhs == lhs);}
	
	/// equality comparison with a const char* string symbol that is null terminated
	friend constexpr bool operator==(Char lhs, const char* rhs){
		auto lhsItr = lhs.cbegin();
		auto lhsEnd = lhs.cend();
		for(; lhsItr != lhsEnd; ++lhsItr, (void)++rhs){
			if(*rhs == '\0' || *lhsItr != *rhs) return false;
		}
		return *rhs == '\0';
	}
	
	/// equality comparison with a const char* string symbol
	friend constexpr inline bool operator==(const char* lhs, Char rhs){return (rhs == lhs);}
	
	/// equality comparison with a range. has to support cbegin() and cend() !
	template<class Range>
	friend bool operator==(Char lhs, const Range& rhs){
		auto lhsItr = lhs.cbegin();
		const auto lhsEnd = lhs.cend();
		auto rhsItr = rhs.cbegin();
		const auto rhsEnd = rhs.cend();
		for(; lhsItr != lhsEnd && rhsItr != rhsEnd; ++lhsItr, (void)++rhsItr){
			if(*lhsItr != *rhsItr) return false;
		}
		return lhsItr == lhsEnd && rhsItr == rhsEnd;
	}
	
	friend constexpr inline bool operator!=(Char lhs, char rhs){return !(lhs == rhs);}
	friend constexpr inline bool operator!=(char lhs, Char rhs){return !(lhs == rhs);}
	
	/// equality comparison with a range. has to support cbegin() and cend() !
	template<class Range>
	friend constexpr inline bool operator==(const Range& lhs, Char rhs){return (rhs == lhs);}
	
	/// unequality comparison with self
	friend constexpr inline bool operator!=(Char lhs, Char rhs){return lhs.to_int() != rhs.to_int();}
	
	/// unequality comparison with c_string
	friend constexpr inline bool operator!=(Char lhs, const char* rhs){return !(lhs == rhs);}
	friend constexpr inline bool operator!=(const char* lhs, Char rhs){return !(lhs == rhs);}
	
	/// unequality comparison with char range
	template<class Range>
	friend inline bool operator!=(const Range& lhs, Char rhs){return !(lhs == rhs);}
	
	/// unequality comparison with char range
	template<class Range>
	friend inline bool operator!=(Char lhs, const Range& rhs){return !(lhs == rhs);}
	
	friend constexpr bool operator<(Char lhs, const char* rhs){
		auto lhsItr = lhs.cbegin();
		auto lhsEnd = lhs.cend();
		for(; lhsItr != lhsEnd; ++lhsItr, (void)++rhs){
			if(*rhs == '\0' || *lhsItr >= *rhs) return false;
		}
		return *rhs == '\0';
	}
	
	friend constexpr bool operator>(Char lhs, const char* rhs){
		auto lhsItr = lhs.cbegin();
		auto lhsEnd = lhs.cend();
		for(; lhsItr != lhsEnd; ++lhsItr, (void)++rhs){
			if(*rhs == '\0' || *lhsItr <= *rhs) return false;
		}
		return *rhs == '\0';
	}
	
	template<class Range>
	friend bool operator<(Char lhs, const Range& rhs){
		auto lhsItr = lhs.cbegin();
		const auto lhsEnd = lhs.cend();
		auto rhsItr = rhs.cbegin();
		const auto rhsEnd = rhs.cend();
		for(; lhsItr != lhsEnd && rhsItr != rhsEnd; ++lhsItr, (void)++rhsItr){
			if(*lhsItr >= *rhsItr) return false;
		}
		return lhsItr == lhsEnd && rhsItr == rhsEnd;
	}
	
	template<class Range>
	friend bool operator>(Char lhs, const Range& rhs){
		auto lhsItr = lhs.cbegin();
		const auto lhsEnd = lhs.cend();
		auto rhsItr = rhs.cbegin();
		const auto rhsEnd = rhs.cend();
		for(; lhsItr != lhsEnd && rhsItr != rhsEnd; ++lhsItr, (void)++rhsItr){
			if(*lhsItr <= *rhsItr) return false;
		}
		return lhsItr == lhsEnd && rhsItr == rhsEnd;
	}
	
	friend constexpr inline bool operator<(Char lhs, char rhs){return lhs.to_int() < static_cast<Char::int_type>(rhs);}
	friend constexpr inline bool operator<(char lhs, Char rhs){return static_cast<Char::int_type>(lhs) < rhs.to_int();}
	friend constexpr inline bool operator<(Char lhs, Char rhs){return lhs.to_int() < rhs.to_int();}
	friend constexpr inline bool operator<(const char* lhs, Char rhs){return rhs > lhs;}
	template<class Range> friend constexpr inline bool operator<(const Range& lhs, Char rhs){return rhs > lhs;}
	
	
	friend constexpr inline bool operator>(Char lhs, char rhs){return rhs < lhs;}
	friend constexpr inline bool operator>(char lhs, Char rhs){return rhs < lhs;}
	friend constexpr inline bool operator>(Char lhs, Char rhs){return lhs.to_int() > rhs.to_int();}
	friend constexpr inline bool operator>(const char* lhs, Char rhs){return rhs < lhs;}
	template<class Range> friend constexpr inline bool operator>(const Range& lhs, Char rhs){return rhs < lhs;}
	
	friend constexpr inline bool operator<=(Char lhs, char rhs){return !(lhs > rhs);}
	friend constexpr inline bool operator<=(char lhs, Char rhs){return !(lhs > rhs);}
	friend constexpr inline bool operator<=(Char lhs, Char rhs){return lhs.to_int() <= rhs.to_int();}
	friend constexpr inline bool operator<=(Char lhs, const char* rhs){return !(lhs > rhs);}
	friend constexpr inline bool operator<=(const char* lhs, Char rhs){return !(rhs < lhs);}
	template<class Range> friend constexpr inline bool operator<=(Char lhs, const Range& rhs){return !(lhs > rhs);}
	template<class Range> friend constexpr inline bool operator<=(const Range& lhs, Char rhs){return !(rhs < lhs);}
	
	friend constexpr inline bool operator>=(Char lhs, char rhs){return !(lhs < rhs);}
	friend constexpr inline bool operator>=(char lhs, Char rhs){return !(lhs < rhs);}
	friend constexpr inline bool operator>=(Char lhs, Char rhs){return lhs.to_int() >= rhs.to_int();}
	friend constexpr inline bool operator>=(Char lhs, const char* rhs){return !(lhs < rhs);}
	friend constexpr inline bool operator>=(const char* lhs, Char rhs){return !(rhs > lhs);}
	template<class Range> friend constexpr inline bool operator>=(Char lhs, const Range& rhs){return !(lhs < rhs);}
	template<class Range> friend constexpr inline bool operator>=(const Range& lhs, Char rhs){return !(rhs > lhs);}
};

}
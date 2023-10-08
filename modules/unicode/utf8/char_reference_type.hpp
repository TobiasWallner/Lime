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
	const_iterator _pos = nullptr;

public:
	constexpr char_const_reference() = default;
	constexpr char_const_reference(const char_const_reference&) = default;
	constexpr char_const_reference& operator= (const char_const_reference&) = default;
	constexpr char_const_reference(const_iterator pos);
	
	constexpr size_type size() const;
	constexpr const_iterator begin() const;
	constexpr const_iterator cbegin() const;
	constexpr const_iterator end() const;
	constexpr const_iterator cend() const;
	
	constexpr operator std::string_view() const;
	
	constexpr const char& operator[] (size_type i) const;
	
	constexpr const_pointer operator&() const;
	
	friend constexpr bool operator == (char_const_reference lhs, char_const_reference rhs);
	friend constexpr bool operator == (char lhs, char_const_reference rhs);
	friend constexpr bool operator == (char_const_reference lhs, char rhs);

	friend constexpr bool operator != (char_const_reference lhs, char_const_reference rhs);
	friend constexpr bool operator != (char lhs, char_const_reference rhs);
	friend constexpr bool operator != (char_const_reference lhs, char rhs);

	friend constexpr bool operator < (char_const_reference lhs, char_const_reference rhs);
	friend constexpr bool operator < (char lhs, char_const_reference rhs);
	friend constexpr bool operator < (char_const_reference lhs, char rhs);

	friend constexpr bool operator <= (char_const_reference lhs, char_const_reference rhs);
	friend constexpr bool operator <= (char lhs, char_const_reference rhs);
	friend constexpr bool operator <= (char_const_reference lhs, char rhs);

	friend constexpr bool operator > (char_const_reference lhs, char_const_reference rhs);
	friend constexpr bool operator > (char lhs, char_const_reference rhs);
	friend constexpr bool operator > (char_const_reference lhs, char rhs);

	friend constexpr bool operator >= (char_const_reference lhs, char_const_reference rhs);
	friend constexpr bool operator >= (char lhs, char_const_reference rhs);
	friend constexpr bool operator >= (char_const_reference lhs, char rhs);
};

class string;
class Char;

class char_reference{
public:
	using iterator = char*;
	using const_iterator = const char*;
	using pointer = char*;
	using const_pointer = const char*;
	using size_type = size_t;

private:	
	utf8::string* _str = nullptr;
	iterator _pos = nullptr;
	
public:
	constexpr char_reference() = default;
	constexpr char_reference(const char_reference&) = default;
	constexpr char_reference& operator= (const char_reference&) = default;
	constexpr char_reference(utf8::string* str, iterator pos);
	constexpr char_reference(utf8::Char& ch);
	
	constexpr size_type size() const;
	constexpr iterator begin();
	constexpr const_iterator begin() const;
	constexpr const_iterator cbegin() const;
	constexpr iterator end();
	constexpr const_iterator end() const;
	constexpr const_iterator cend() const;

	constexpr char& operator[] (size_type i);
	constexpr const char& operator[] (size_type i) const;

	constexpr pointer operator&();

	constexpr operator char_const_reference();
	constexpr operator std::string_view() const;
	
	constexpr char_reference& operator = (char c);
	constexpr char_reference& operator = (const utf8::Char& c);
	
	friend constexpr bool operator == (char_reference lhs, char_reference rhs);
	friend constexpr bool operator == (char_reference lhs, char_const_reference rhs);
	friend constexpr bool operator == (char_const_reference lhs, char_reference rhs);
	friend constexpr bool operator == (char lhs, char_reference rhs);
	friend constexpr bool operator == (char_reference lhs, char rhs);

	friend constexpr bool operator != (char_reference lhs, char_reference rhs);
	friend constexpr bool operator != (char_reference lhs, char_const_reference rhs);
	friend constexpr bool operator != (char_const_reference lhs, char_reference rhs);
	friend constexpr bool operator != (char lhs, char_reference rhs);
	friend constexpr bool operator != (char_reference lhs, char rhs);

	friend constexpr bool operator < (char_reference lhs, char_reference rhs);
	friend constexpr bool operator < (char_reference lhs, char_const_reference rhs);
	friend constexpr bool operator < (char_const_reference lhs, char_reference rhs);
	friend constexpr bool operator < (char lhs, char_reference rhs);
	friend constexpr bool operator < (char_reference lhs, char rhs);

	friend constexpr bool operator <= (char_reference lhs, char_reference rhs);
	friend constexpr bool operator <= (char_reference lhs, char_const_reference rhs);
	friend constexpr bool operator <= (char_const_reference lhs, char_reference rhs);
	friend constexpr bool operator <= (char lhs, char_reference rhs);
	friend constexpr bool operator <= (char_reference lhs, char rhs);

	friend constexpr bool operator > (char_reference lhs, char_reference rhs);
	friend constexpr bool operator > (char_reference lhs, char_const_reference rhs);
	friend constexpr bool operator > (char_const_reference lhs, char_reference rhs);
	friend constexpr bool operator > (char lhs, char_reference rhs);
	friend constexpr bool operator > (char_reference lhs, char rhs);

	friend constexpr bool operator >= (char_reference lhs, char_reference rhs);
	friend constexpr bool operator >= (char_reference lhs, char_const_reference rhs);
	friend constexpr bool operator >= (char_const_reference lhs, char_reference rhs);
	friend constexpr bool operator >= (char lhs, char_reference rhs);
	friend constexpr bool operator >= (char_reference lhs, char rhs);
};
	
} // namespace utf8
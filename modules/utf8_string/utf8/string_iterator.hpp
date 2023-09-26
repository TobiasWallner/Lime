#pragma once

namespace utf8{
	
class const_iterator{
private:
	const char* itr;
	
public:
	constexpr const_iterator(const char* pos) : itr(pos){}
	constexpr const_iterator(const const_iterator&) = default;
	constexpr const_iterator& operator(const const_iterator&) = default;
	
	/*TODO*/
}

class iterator{
private:
	char* itr;
	
public:
	constexpr iterator(const char* pos) : itr(pos){}
	constexpr iterator(const iterator&) = default;
	constexpr iterator& operator(const iterator&) = default;
	
	operator const_iterator() {return const_iterator(this->itr);}
	
	/*TODO*/
	
}
	
}
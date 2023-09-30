#pragma once
namespace utf8{
	
template<class AItr, class BItr>
constexpr bool starts_with(AItr afirst, AItr alast, BItr bfirst, BItr blast) noexcept {
	while(afirst != alast && bfirst != blast){
		if(*(afirst++) != *(bfirst++)){
			return false;
		}
	}
	return bfirst == blast;
}

template<class AItr, class BItr>
constexpr bool ends_with(AItr afirst, AItr alast, BItr bfirst, BItr blast) noexcept {
	while(afirst != alast && bfirst != blast){
		if(*(--alast) != *(--blast)){
			return false;
		}
	}
	return bfirst == blast;
}

template<class AItr, class BItr>
constexpr bool contains(AItr afirst, AItr alast, BItr bfirst, BItr blast) noexcept {
	for(;afirst != alast && bfirst != blast; ++afirst){
		if(utf8::starts_with(afirst, alast, bfirst, blast)){
			return true;
		}
	}
	return false;
}

// returns a pointer to the end of a c_style string that points to the termination character '\0'
constexpr const char* strend(const char* str){while(*str != '\0'){++str;}; return str;}

// returns the length of a c_style string not counting the termination character '\0'
constexpr size_t strlen(const char* str){return utf8::strend(str) - str;}


	
}
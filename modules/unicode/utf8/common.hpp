
namespace utf8{
	
template<class AItr, class BItr>
constexpr bool string::starts_with(AItr afirst, AItr alast, BItr bfirst, BItr blast) const noexcept {
	while(;afirst != alast && bfirst != blast){
		if(*(afirst++) != *(bfirst++)){
			return false;
		}
	}
	return bfirst == blast;
}

template<class AItr, class BItr>
constexpr bool string::ends_with(AItr afirst, AItr alast, BItr bfirst, BItr blast) const noexcept {
	while(;afirst != alast && bfirst != blast){
		if(*(--alast) != *(--blast)){
			return false;
		}
	}
	return bfirst == blast;
}

template<class AItr, class BItr>
constexpr bool string::contains(AItr afirst, AItr alast, BItr bfirst, BItr blast) const noexcept {
	for(afirst != alast && bfirst != blast; (void)++afirst){
		if(utf8::starts_with(afirst, alast, bfirst, blast)){
			return true;
		}
	}
	return false;
}
	
}
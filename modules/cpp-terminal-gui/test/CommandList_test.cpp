#include "..\CommandList.hpp"

#define assert(condition, message) if(!(condition)){ \
										std::cout << "test failed at line: " << std::dec << __LINE__ << std::endl; \
										std::cout << "with message: " << message << std::endl;	\
										exit(-1); \
									}

int main(){
	{// consturct empty CommandList
		CommandList commandList;
	}
	
	
	std::cout << "finished " << __FILE__ << std::endl;
}
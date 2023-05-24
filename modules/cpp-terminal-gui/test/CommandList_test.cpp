
#include <cpp-terminal-gui\CommandList.hpp>

// C std
#include <stdlib.h>

// C++ std
#include <iostream>
#include <algorithm>

#define assert(condition, message) if(!(condition)){ \
										std::cout << "test failed at line: " << __LINE__ << std::endl; \
										std::cout << "with message: " << message << std::endl;	\
										exit(EXIT_FAILURE); \
									}

// use static here not for memory, but for static linking so that the name of this function 
// does not convolute the global name space for the linker
static bool smaller(const TermGui::CommandList::value_type& lhs, const TermGui::CommandList::value_type& rhs){
	return lhs.index < rhs.index;
}

int main(){
	{// consturct empty CommandList
		TermGui::CommandList commandList;
	}
	{// consturct empty CommandList is empty
		TermGui::CommandList commandList;
		assert(commandList.empty(), "Error: new constructed should be empty but is not");
		const auto expected_size = 0;
		assert(commandList.size() == expected_size, "should have size " << expected_size << " but has size " << commandList.size());
	}
	{// constuct a Foreground Command
		TermGui::ForegroundColor forgroundColor(10, 50, 80);
	}
	{// construct a Background Command
		TermGui::BackgroundColor backgroundColor(90, 60, 20);
	}
	{// insert a foreground color into the command list
		TermGui::CommandList commandList;
		TermGui::ForegroundColor forgroundColor(10, 50, 80);
		const int index = 11;
		commandList.insert(forgroundColor, index);
	}
	{// insert a background color into the command list
		TermGui::CommandList commandList;
		TermGui::BackgroundColor backgroundColor(90, 60, 20);
		const int index = 11;
		commandList.insert(backgroundColor, index);
	}
	{// insert into the command list -> command list is no empty
		TermGui::CommandList commandList;
		TermGui::ForegroundColor forgroundColor(10, 50, 80);
		const int index = 11;
		commandList.insert(forgroundColor, index);
		assert(commandList.empty() == false, "command list should not be empty");
		const auto expected_size = 1;
		assert(commandList.size() == expected_size, "should have size " << expected_size << " but has size " << commandList.size());
	}
	{// insert n into the command list -> command list has size n and is sorted
		TermGui::CommandList commandList;
		TermGui::ForegroundColor forgroundColor(10, 50, 80);
		commandList.insert(forgroundColor, 1);
		commandList.insert(forgroundColor, 2);
		commandList.insert(forgroundColor, 3);
		const auto expected = 3;
		assert(commandList.size() == expected, "command list should have size: " << expected << " but has size: " << commandList.size());
		assert(std::is_sorted(commandList.begin(), commandList.end(), smaller), "the list is not sorted after 3 insertions");
	}
	
	
	std::cout << "finished " << __FILE__ << std::endl;
	return EXIT_SUCCESS;
}
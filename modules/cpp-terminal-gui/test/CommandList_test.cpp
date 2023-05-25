
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

static void consturct_empty_CommandList(){
	TermGui::CommandList commandList;
}

static void consturct_empty_CommandList_is_empty(){
	TermGui::CommandList commandList;
	assert(commandList.empty(), "Error: new constructed should be empty but is not");
	const auto expected_size = 0;
	assert(commandList.size() == expected_size, "should have size " << expected_size << " but has size " << commandList.size());
}

static void consturct_a_ForegroundColor_Command(){
	TermGui::ForegroundColor forgroundColor(10, 50, 80);
}

static void consturct_a_BackgroundColor_Command(){
	TermGui::BackgroundColor backgroundColor(90, 60, 20);
}

static void insert_a_foreground_color_into_the_command_list(){
	TermGui::CommandList commandList;
	TermGui::ForegroundColor forgroundColor(10, 50, 80);
	const int index = 11;
	commandList.insert(forgroundColor, index);
}

static void insert_a_background_color_into_the_command_list(){
	TermGui::CommandList commandList;
	TermGui::BackgroundColor backgroundColor(90, 60, 20);
	const int index = 11;
	commandList.insert(backgroundColor, index);
}

static void  insert_into_makes_list_not_empty(){
	TermGui::CommandList commandList;
	TermGui::ForegroundColor forgroundColor(10, 50, 80);
	const int index = 11;
	commandList.insert(forgroundColor, index);
	assert(commandList.empty() == false, "command list should not be empty");
	const auto expected_size = 1;
	assert(commandList.size() == expected_size, "should have size " << expected_size << " but has size " << commandList.size());
}

static void  insert_3_makes_size_3(){
	TermGui::CommandList commandList;
	TermGui::ForegroundColor forgroundColor(10, 50, 80);
	commandList.insert(forgroundColor, 1);
	commandList.insert(forgroundColor, 2);
	commandList.insert(forgroundColor, 3);
	const auto expected = 3;
	assert(commandList.size() == expected, "command list should have size: " << expected << " but has size: " << commandList.size());
	assert(std::is_sorted(commandList.begin(), commandList.end(), smaller), "the list is not sorted after 3 insertions");
}

static void insert_two_different_commands_into_the_same_index(){
	TermGui::CommandList commandList;
	TermGui::ForegroundColor forgroundColor(10, 50, 80);
	TermGui::BackgroundColor backgroundColor(90, 60, 20);
	
	const bool result1 = commandList.insert(forgroundColor, 1);
	const bool result2 = commandList.insert(backgroundColor, 1);
	
	assert(result1, ".insert() should return true");
	assert(result2, ".insert() should return true");
	const auto expected_number_of_index_events = 1;
	assert(commandList.size() == expected_number_of_index_events, "command list should have size: " << expected_number_of_index_events << " but has size: " << commandList.size());
	const auto expected_number_of_efents_at_first_index = 2;
	assert(commandList.front().size() == expected_number_of_efents_at_first_index, "command list should have size: " << expected_number_of_efents_at_first_index << " but has size: " << commandList.size());
}

static void insert_two_different_commands_into_the_same_index_v2(){
	TermGui::CommandList commandList;
	TermGui::ForegroundColor forgroundColor(10, 50, 80);
	TermGui::BackgroundColor backgroundColor(90, 60, 20);
	
	const bool result1 = commandList.insert(backgroundColor, 1);
	const bool result2 = commandList.insert(forgroundColor, 1);
	
	assert(result1, ".insert() should return true");
	assert(result2, ".insert() should return true");
	const auto expected_number_of_index_events = 1;
	assert(commandList.size() == expected_number_of_index_events, "command list should have size: " << expected_number_of_index_events << " but has size: " << commandList.size());
	const auto expected_number_of_efents_at_first_index = 2;
	assert(commandList.front().size() == expected_number_of_efents_at_first_index, "command list should have size: " << expected_number_of_efents_at_first_index << " but has size: " << commandList.size());
}

static void insert_two_similar_commands_into_the_same_index(){
	TermGui::CommandList commandList;
	TermGui::ForegroundColor forgroundColor1(10, 50, 80);
	TermGui::ForegroundColor forgroundColor2(90, 60, 20);
	
	const bool result1 = commandList.insert(forgroundColor1, 1);
	const bool result2 = commandList.insert(forgroundColor2, 1);
	
	assert(result1 == true, "could not insert element into empty list");
	assert(result2 == false, "same element type should not be pushed back into the same index and insert should return false");
	assert(commandList.size() == 1, "inserting second object of same type into the same index should have been rejected and the size of the list should be unchanged");
	assert(commandList.front().size() == 1, "inserting second object of same type into the same index should have been rejected and the size of the list at the index location should be unchanged");
}

int main(){
	consturct_empty_CommandList();
	consturct_empty_CommandList_is_empty();
	consturct_a_ForegroundColor_Command();
	consturct_a_BackgroundColor_Command();
	insert_a_foreground_color_into_the_command_list();
	insert_a_background_color_into_the_command_list();
	insert_into_makes_list_not_empty();
	insert_3_makes_size_3();
	insert_two_different_commands_into_the_same_index();
	insert_two_similar_commands_into_the_same_index();
	
	std::cout << "finished " << __FILE__ << std::endl;
	return EXIT_SUCCESS;
}
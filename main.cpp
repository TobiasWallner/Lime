
// C++ std
#include <iostream>
#include <cstring>

// Project
#include "lime/Lime.hpp"

const char* version_string();
int version();
int help();
int lime_argument_parser(int numberOfArguments, char** listOfArgumentStrings);

int main(int numberOfArguments, char** listOfArgumentStrings){
	return lime_argument_parser(numberOfArguments, listOfArgumentStrings);
}

int lime_argument_parser(int numberOfArguments, char** listOfArgumentStrings){
	
	// remove the program name from the arguments
	--numberOfArguments;
	listOfArgumentStrings = listOfArgumentStrings + 1;
	
	if(numberOfArguments <= 0){
		return Lime().run();
	}else if(0 == std::strcmp(listOfArgumentStrings[0], "--version") 
				|| 0 == std::strcmp(listOfArgumentStrings[0], "version") 
				|| 0 == std::strcmp(listOfArgumentStrings[0], "-v")){
		return version();
	}else if(0 == std::strcmp(listOfArgumentStrings[0], "--help") 
				|| 0 == std::strcmp(listOfArgumentStrings[0], "help") 
				|| 0 == std::strcmp(listOfArgumentStrings[0], "-h")){
		return help();
	}else{
		std::cout 	<< "This command will later create a new file with the file name '" 
					<<  listOfArgumentStrings[0] 
					<< "', but is currently unsupported." 
					<< std::endl;
		return EXIT_SUCCESS;
	}
	
}

const char* version_string(){
	return "0.0.0";
}

int version(){
	std::cout << "lime version: " << version_string() << '\n' << std::endl;
	return EXIT_SUCCESS;
}

int help(){
	std::cout << "Version: " << version_string() << '\n' << 	
		"\n"
		"     / /\n"
		"    / /        ( )  _   __      ___\n"
		"   / /        / / // ) )  ) ) //___) )\n"
		"  / /        / / // / /  / / //\n"
		" / /____/ / / / // / /  / / ((____ \n"
		"\n"
		"Usage: lime [args]\n"
		"\n"
		"help, --help, -h        Shows this help page\n"
		"version, --version, -v  Displays the version string\n"
		<< std::endl;
	return EXIT_SUCCESS;
}



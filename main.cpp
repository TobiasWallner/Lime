

#include <iostream>
#include <stdlib.h>
#include <string.h>

const char* version_string();

/*
	version() is a sub-program of lime that prints the current version string
*/
int version();

/*
	help() is a sub-program of lime that displays the help page
*/
int help();

/* 
	the lime argument parser will call the sub programs of lime
*/
int lime_argument_parser(int numberOfArguments, char** listOfArgumentStrings);

/*
	the main function that invokes the application
*/
int main(int numberOfArguments, char** listOfArgumentStrings){
	
	// remove the program name from the arguments
	--numberOfArguments;
	listOfArgumentStrings = listOfArgumentStrings + 1;
	
	return lime_argument_parser(numberOfArguments, listOfArgumentStrings);
	
}

int lime_argument_parser(int numberOfArguments, char** listOfArgumentStrings){
	
	if(numberOfArguments <= 0){
		std::cout << "This command will later start the Lime editor with a new blank and unsaved file."
					" But currently it is not supported.\n" << std::endl;
		return EXIT_SUCCESS;
	}else if(0 == strcmp(listOfArgumentStrings[0], "--version") 
				|| 0 == strcmp(listOfArgumentStrings[0], "version") 
				|| 0 == strcmp(listOfArgumentStrings[0], "-v")){
		return version();
	}else if(0 == strcmp(listOfArgumentStrings[0], "--help") 
				|| 0 == strcmp(listOfArgumentStrings[0], "help") 
				|| 0 == 0 == strcmp(listOfArgumentStrings[0], "-h")){
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



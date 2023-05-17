// C std
#include <stdlib.h>
#include <string.h>

// C++ std
#include <iostream>
#include <cctype>
#include <cstdint>

// cpp-terminal
#include "cpp-terminal/cursor.hpp"
#include "cpp-terminal/exception.hpp"
#include "cpp-terminal/input.hpp"
#include "cpp-terminal/io.hpp"
#include "cpp-terminal/key.hpp"
#include "cpp-terminal/options.hpp"
#include "cpp-terminal/screen.hpp"
#include "cpp-terminal/terminal.hpp"
#include "cpp-terminal/tty.hpp"
#include "cpp-terminal/version.hpp"

const char* version_string();
int version();
int help();
int lime_argument_parser(int numberOfArguments, char** listOfArgumentStrings);
int lime(int numberOfArguments, char** listOfArgumentStrings);
void main_loop();
bool prozess_event(Term::Event&& event);
bool prozess_key_event(Term::Key keyEvent);

int main(int numberOfArguments, char** listOfArgumentStrings){
	
	// remove the program name from the arguments
	--numberOfArguments;
	listOfArgumentStrings = listOfArgumentStrings + 1;
	
	return lime_argument_parser(numberOfArguments, listOfArgumentStrings);
	
}

/** prozess_event
	=============
	
	Description:
	------------
	prozesses a key event
	
	Return:
	-------
	returns true if the function executed successfully and false if an error occured.
*/
bool prozess_key_event(Term::Key keyEvent){
	// for now just print the key if it is printable to the console
	const auto c = static_cast<char32_t>(keyEvent + Term::Key::NUL); 
	//(Tobias): aparently you have to add the NUL type I found no other way of doing typeconversions
	if(std::isprint(c)){
		std::cout << static_cast<char>(c) << std::flush; 
		//c++ std library cannot really print utf-8 or char32_t characters so I leave it at char types for now
		return true;
	}else{
		return false;
	}
}

/** prozess_event
	=============
	
	Description:
	------------
	prozesses the event passed to the function
	
	Return:
	-------
	returns true if the function executed successfully and false if an error occured.
*/
bool prozess_event(Term::Event&& event){
	switch(event.type()){
		case Term::Event::Type::Empty : {
			std::cout << "Internal Error: Unhandeled Event type 'Empty' " << std::endl;
			return true;
		}break;
		case Term::Event::Type::Key : {
			const auto keyEvent = Term::Key(event);
			return prozess_key_event(keyEvent);
		}break;
		case Term::Event::Type::Screen : {
			std::cout << "Internal Error: Unhandeled Event type 'Screen' " << std::endl;
			return true;
		}break;
		case Term::Event::Type::Cursor : {
			std::cout << "Internal Error: Unhandeled Event type 'Cursor' " << std::endl;
			return true;
		}break;
		case Term::Event::Type::CopyPaste : {
			std::cout << "Internal Error: Unhandeled Event type 'CopyPaste' " << std::endl;
			return true;
		}break; 
		default : {
			/*TODO: a way to display error messages that do not disturb the writeing prozess
				For example by message windows or at a special place at the bottom of the screen
			*/
			std::cout << "Internal Error: Unhandeled Event type ID: " << static_cast<int>(event.type()) << std::endl;
			return true;
		}break;
	}
}

void main_loop(){
	bool done = false;
	while(!done){
		auto event = Term::read_event();
		bool success = prozess_event(std::move(event));
		done = !success;
	}
}

int lime(int numberOfArguments, char** listOfArgumentStrings){
	
	// the following should capture keystrokes and prozess them immediatelly
	// this is just the start of the program that will evolve from this point
	
	// check if the input terminal is attatched to the console
	if(Term::is_stdin_a_tty() == false){
		std::cout << "Error: terminal does not have an input stream ... exiting program" << std::endl;
		return EXIT_FAILURE;
	}
	
	// set options for the console
	const auto Options = {
			Term::Option::NoClearScreen, 
			Term::Option::NoSignalKeys, 
			Term::Option::Cursor, 
			Term::Option::Raw
	};
	
	Term::terminal.setOptions(Options);
	
	// Entering the Program loop: get inputs -> prozess inputs -> display/save changes
	main_loop();
	
	return EXIT_SUCCESS;
}

int lime_argument_parser(int numberOfArguments, char** listOfArgumentStrings){
	
	if(numberOfArguments <= 0){
		return lime(numberOfArguments, listOfArgumentStrings);
	}else if(0 == strcmp(listOfArgumentStrings[0], "--version") 
				|| 0 == strcmp(listOfArgumentStrings[0], "version") 
				|| 0 == strcmp(listOfArgumentStrings[0], "-v")){
		return version();
	}else if(0 == strcmp(listOfArgumentStrings[0], "--help") 
				|| 0 == strcmp(listOfArgumentStrings[0], "help") 
				|| 0 == strcmp(listOfArgumentStrings[0], "-h")){
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



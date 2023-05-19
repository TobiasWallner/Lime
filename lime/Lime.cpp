
// Project
#include "Lime.hpp"

// C std
#include <stdlib.h>
#include <string.h>

// C++ std
#include <iostream>
#include <cctype>
#include <cstdint>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>
#include <cpp-terminal/input.hpp>
#include <cpp-terminal/io.hpp>
#include <cpp-terminal/key.hpp>
#include <cpp-terminal/options.hpp>
#include <cpp-terminal/screen.hpp>
#include <cpp-terminal/terminal.hpp>
#include <cpp-terminal/tty.hpp>
#include <cpp-terminal/event.hpp>


int Lime::run(){
	
	// the following should capture keystrokes and prozess them immediatelly
	// this is just the start of the program that will evolve from this point
	
	// check if the input terminal is attatched to the console
	if(Term::is_stdin_a_tty() == false){
		std::cout << "Error: terminal does not have an input stream ... exiting program" << std::endl;
		return EXIT_FAILURE;
	}
	
	// set options for the console
	const auto Options = {
		Term::Option::ClearScreen, 	// start from a cear screen
		Term::Option::NoSignalKeys, // deactivate key combinations that generate a signal or interrupt
		Term::Option::NoCursor, 	// deactivate the cursor, we will display it ourselfs
		Term::Option::Raw			// get the raw and unprozessed io data from the console buffers
	};
	
	Term::terminal.setOptions(Options);
	
	// Entering the Program loop: get inputs -> prozess inputs -> display/save changes
	this->run_main_loop();
	
	return EXIT_SUCCESS;
}

void Lime::quit(){
	/**
		Savely quit the program.
	*/
	std::cout << "Quitting Lime" << std::endl;
	this->main_loop_continue = false;
}

void Lime::run_main_loop(){
	while(this->main_loop_continue){
		auto event = Term::read_event();
		this->prozess_event(std::move(event));
	}
}


void Lime::prozess_event(Term::Event&& event){
	switch(event.type()){
		case Term::Event::Type::Empty : {
			this->prozess_empty_event();
		}return;
		case Term::Event::Type::Key : {
			const auto keyEvent = Term::Key(event);
			this->prozess_key_event(keyEvent);
		}return;
		case Term::Event::Type::Screen : {
			this->prozess_screen_event();
		}return;
		case Term::Event::Type::Cursor : {
			this->prozess_cursour_event();	
		}return;
		case Term::Event::Type::CopyPaste : {
			this->prozess_copy_paste_event();
		}return; 
		default : {
			this->prozess_unhandeled_event(std::move(event));
		}return;
	}
}

void Lime::prozess_key_event(Term::Key keyEvent){
	// for now just print the key if it is printable to the console
	const auto c = static_cast<char32_t>(keyEvent + Term::Key::NUL); 
	//(Tobias): aparently you have to add the NUL type I found no other way of doing typeconversions
	if(std::isprint(c)){
		std::cout << static_cast<char>(c) << std::flush; 
		//c++ std library cannot really print utf-8 or char32_t characters so I leave it at char types for now
	}else{
		this->quit();
	}
}
  
  
void Lime::prozess_empty_event(){
	std::cout << "Internal Error: Unhandeled Event type 'Empty' " << std::endl;
}

void Lime::prozess_screen_event(){
	std::cout << "Internal Error: Unhandeled Event type 'Screen' " << std::endl;
}

void Lime::prozess_cursour_event(){
	std::cout << "Internal Error: Unhandeled Event type 'Cursor' " << std::endl;
}

void Lime::prozess_copy_paste_event(){
	std::cout << "Internal Error: Unhandeled Event type 'CopyPaste' " << std::endl;
}

void Lime::prozess_unhandeled_event(Term::Event&& event){
	/*TODO: a way to display error messages that do not disturb the writeing prozess
		For example by message windows or at a special place at the bottom of the screen
	*/
	std::cout << "Internal Error: Unhandeled Event type ID: " << static_cast<int>(event.type()) << std::endl;
}
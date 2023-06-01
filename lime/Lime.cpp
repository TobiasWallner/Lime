
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

// cpp-terminal-gui
#include <cpp-terminal-gui/ColorString.hpp>
#include <cpp-terminal-gui/TextEditor.hpp>

Lime::Lime(){
	this->infoText << "Quit: " << TermGui::FgColor(0, 200, 0) << "Ctrl + Q" << TermGui::FgColor(Term::Color::Name::Default);
}

int Lime::run(){
	
	// the following should capture keystrokes and prozess them immediatelly
	// this is just the start of the program that will evolve from this point
	
	// check if the input terminal is attatched to the console
	if(Term::is_stdin_a_tty() == false){
		std::cout << "Error: terminal does not have an input stream ... exiting program" << std::endl;
		return EXIT_FAILURE;
	}
	
	// set options for the console
	Term::terminal.setOptions(
			Term::Option::ClearScreen, 	// start from a cear screen
			Term::Option::NoSignalKeys, // deactivate key combinations that generate a signal or interrupt
			Term::Option::NoCursor, 	// deactivate the cursor, we will display it ourselfs
			Term::Option::Raw			// get the raw and unprozessed io data from the console buffers
	);
	
	
	
	// Entering the Program loop: get inputs -> prozess inputs -> display/save changes
	
	TermGui::ColorString colorString("This ");
		colorString << TermGui::FgColor(255, 0, 0) << "is " << TermGui::FgColor(0, 255, 0) << "the " << TermGui::BgColor(255, 0, 255) << " Lime" << TermGui::FgColor(255, 255, 255) << TermGui::BgColor(0, 0, 0) << "Editor";
	
	std::string outputString;
	colorString.render(outputString);
	
	std::cout << outputString << std::endl;
	
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
	std::string outputString; 	//reuse string memory
	outputString.reserve(1024 * 2); // reserve 2kB of memory in advance
	while(this->main_loop_continue){
		auto event = Term::read_event();
		this->prozess_event(std::move(event));
		outputString.clear();
		this->render(outputString);
		this->draw(outputString);
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
	if(keyEvent == Term::Key::ENTER){
		this->textEditor.insert_new_line();
	}
	if(keyEvent.is_CTRL()){
		const auto ctrlPlusKey = keyEvent - Term::Key::CTRL;
		if(ctrlPlusKey == Term::Key::Q){
			this->quit();
		}else{
			const auto ascii = static_cast<char>(ctrlPlusKey + Term::Key::NUL); 
			this->topMessageBar.assign("Internal Error: Unhandeled Key press: Ctrl + ").append(ascii);
		}
	}else if(keyEvent.is_ASCII()){
		const auto ascii = static_cast<char>(keyEvent + Term::Key::NUL); 
		this->textEditor.insert(ascii);	
	}else{
		const auto character = static_cast<char32_t>(keyEvent + Term::Key::NUL); 
		this->topMessageBar.assign("Internal Error: Unhandeled Key press: ").append(std::to_string(character));
	}
	
}
  
  
void Lime::prozess_empty_event(){
	this->topMessageBar.assign("Internal Error: Unhandeled Event type 'Empty' ");
}

void Lime::prozess_screen_event(){
	this->topMessageBar.assign("Internal Error: Unhandeled Event type 'Screen' ");
}

void Lime::prozess_cursour_event(){
	this->topMessageBar.assign("Internal Error: Unhandeled Event type 'Cursor' ");
}

void Lime::prozess_copy_paste_event(){
	this->topMessageBar.assign("Internal Error: Unhandeled Event type 'CopyPaste' ");
}

void Lime::prozess_unhandeled_event(Term::Event&& event){
	/*TODO: a way to display error messages that do not disturb the writeing prozess
		For example by message windows or at a special place at the bottom of the screen
	*/
	this->topMessageBar.assign("Internal Error: Unhandeled Event type ID: ").append(std::to_string(static_cast<int>(event.type())));
	
}

void Lime::render(std::string& outputString) const{
	this->topMessageBar.render(outputString);
	outputString += '\n';
	this->textEditor.render(outputString);
	outputString += '\n';
	this->infoText.render(outputString);
}

void Lime::draw(const std::string& outputString) const{
	// reset screen
	std::cout << Term::clear_screen() << Term::cursor_move(0, 0) << std::flush;
	// draw new output
	std::cout << outputString << std::flush;
}
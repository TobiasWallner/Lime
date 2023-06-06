
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

// lime (pasting both for windows and unix) 
#ifdef _WIN32
#include <Windows.h>
#else
#include <X11/Xlib.h>
#include <climits>
#include <cstring>
#endif


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
	// initial render of the whole screen
	this->render(outputString);
	this->draw(outputString);
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
	}else if(keyEvent.is_CTRL()){
		const auto ctrlPlusKey = keyEvent - Term::Key::CTRL;
		if(ctrlPlusKey == Term::Key::Q){
			this->quit();
		}
		if (ctrlPlusKey == Term::Key::V) {
			this->topMessageBar.assign("Clipboard text has been copied successfully");

			std::string clipboardText;
			#ifdef _WIN32
			if (RetrieveClipboardTextWindows(clipboardText))
			{
				this->textEditor.insert(clipboardText.c_str());
			}
			#else
			if (RetrieveClipboardTextUnix(clipboardText))
			{
				this->textEditor.insert(clipboardText.c_str());
			}
			#endif
			else {
				this->topMessageBar.assign("Internal Error: Bad input from Clipboard");
				this->textEditor.insert("");
			}
			
			
		
		}
		else {
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
	std::cout 	<< Term::clear_screen() 
				<< Term::cursor_move(0, 0) 
				<< outputString << std::flush;
}

#ifdef _WIN32
bool Lime::RetrieveClipboardTextWindows(std::string& clipboardText) const{
	
	// Open the clipboard
	if (!OpenClipboard(NULL))
	{
		return false;
	}

	// Get the clipboard data
	HANDLE handle = GetClipboardData(CF_TEXT);
	if (handle == NULL)
	{
		CloseClipboard();
		return false;
	}

	// Lock the memory and retrieve the text
	char* text = static_cast<char*>(GlobalLock(handle));
	if (text == NULL)
	{
		CloseClipboard();
		return false;
	}

	// Assign the retrieved text to the output parameter
	clipboardText = text;

	// Release the memory and close the clipboard
	GlobalUnlock(handle);
	CloseClipboard();

	return true;
}
#else

bool Lime::RetrieveClipboardTextUnix(std::string& clipboardText) const {
	
	Display* display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		return false;
	}
	
	
	Atom clipboardAtom = XInternAtom(display, "CLIPBOARD", False);
	Atom fmtid = XInternAtom(display, "PRIMARY", False); // from Stack Overflow user x11user
	Atom propid = XInternAtom(display, "XSEL_DATA", False); // from Stack Overflow user x11user
	Atom incrid = XInternAtom(display, "INCR", False); // from Stack Overflow user x11user
	
	if (clipboardAtom == None)
	{
		XCloseDisplay(display);
		return false;
	}

	XConvertSelection(display, clipboardAtom, fmtid, propid, None, CurrentTime);
	XFlush(display);

	XEvent event;
	while (true)
	{
		XNextEvent(display, &event);

		if (event.type == SelectionNotify)
		{
			Atom selectionAtom = event.xselection.property;
			if (selectionAtom == None)
			{
				XCloseDisplay(display);
				return false;
			}

			int actualFormat;
			unsigned long itemCount, bytesAfter;
			unsigned char* clipboardData = nullptr;

			XGetWindowProperty(display, event.xselection.requestor, selectionAtom, 0, LONG_MAX/4,
				False, AnyPropertyType, &fmtid, &actualFormat, &itemCount,
				&bytesAfter, &clipboardData);

			if (fmtid == incrid){ // from Stack Overflow user x11user
				// buffer is too large and INCR reading is not implemented yet
				XCloseDisplay(display);
				return false;
			}else if (clipboardData != nullptr){
				clipboardText = reinterpret_cast<char*>(clipboardData);
				XFree(clipboardData);
			}

			break;
		}
	}

	XCloseDisplay(display);
	return true;
}

#endif

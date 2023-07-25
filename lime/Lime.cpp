
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
#include <cpp-terminal/style.hpp>

// cpp-terminal-gui
#include <cpp-terminal-gui/ColorString.hpp>
#include <cpp-terminal-gui/TextEditor.hpp>
#include <cpp-terminal-gui/CommandLine.hpp>
#include <cpp-terminal-gui/VerticalGrid.hpp>
#include <cpp-terminal-gui/HorizontalGrid.hpp>
#include <cpp-terminal-gui/Label.hpp>
#include <cpp-terminal-gui/ColorThemes.hpp>


// lime (pasting both for windows and unix) 
#ifdef _WIN32
#include <Windows.h>
#else
#include <X11/Xlib.h>
#include <climits>
#include <cstring>
#endif

Lime::Lime() : 
	mainGrid(	
		TermGui::ScreenPosition{.x = 1, .y = 1}, 
		TermGui::ScreenWidth{	
			.x = static_cast<TermGui::ScreenWidth::size_type>(Term::screen_size().columns()),
			.y = static_cast<TermGui::ScreenWidth::size_type>(Term::screen_size().rows())
		}
	),
	textEditorGrid(),
	topMessageBar(),
	textEditor(),
	infoText(),										
	commandLine(this, &Lime::command_line_callback)
{   
	this->mainGrid.push_back_absolute(&this->topMessageBar, 1);
	this->mainGrid.push_back_relative(&this->textEditorGrid);
	this->mainGrid.push_back_absolute(&this->infoText, 6);
	this->mainGrid.push_back_absolute(&this->commandLine, 1);
	
	this->textEditorGrid.push_back_relative(&this->textEditor, 0, 100); // set the maximal width to 80
	this->textEditorGrid.set_centering(true); //TODO: make min, max width/height and centering functions again that when set or changed trigger a re-distribution
	
	/*TODO: figure out dynamic height ... re-distribution before every render? ... element stores a callback function to the grid and tells it its size when it changes*/

	activeEditor = &(this->textEditor);
	this->deactivate_command_line();
	this->activate_text_editor();
}

inline void Lime::activate_command_line(){
	this->commandLine.show_cursor(true);
	this->activeCursor = &this->commandLine;
	
	// load infotext
	this->infoText.clear();
	
	const auto lime_fg = TermGui::fg_color(LimeTheme::green[0], LimeTheme::green[1], LimeTheme::green[2]);
	const auto default_fg = TermGui::default_fg_color();
	
	this->infoText  << lime_fg << "save" << default_fg << "  ...............  saves the current file\n"
					<< lime_fg << "save-as <filename>" << default_fg << " ..  saves the current file under a new name\n"
					<< lime_fg << "open <filename>" << default_fg << "  ....  opens the file under the provided name";
	
}

void Lime::activate_text_editor(){
	this->textEditor.show_cursor(true);
	this->activeCursor = &this->textEditor;
		
		//load infotext
	this->infoText.clear();
	
	const auto lime_fg = TermGui::fg_color(LimeTheme::green[0], LimeTheme::green[1], LimeTheme::green[2]);
	const auto default_fg = TermGui::default_fg_color();
	
	this->infoText  
		<< "Quit: " << lime_fg << "Ctrl + Q" << default_fg << "\t"
		<< "Save: " << lime_fg << "Ctrl + S" << default_fg << "\t"
		<< "Paste: " << lime_fg << "Ctrl + V" << default_fg << "\n"
			   
		<< "Move Left: " << lime_fg << "Alt + J" << default_fg << "\t"
		<< "Move Up: " << lime_fg << "Alt + I" << default_fg << "\t"
		<< "Move Right: " << lime_fg << "Alt + L" << default_fg << "\t"
		<< "Move Down: " << lime_fg << "Alt + K" << default_fg << "\n"
			   
		<< "Move to Line Start: " << lime_fg << "Alt + U" << default_fg << "\t"
		<< "Move to Line End: " << lime_fg << "Alt + O" << default_fg << "\n"
		   
		<< "Move to File Start: " << lime_fg << "Ctrl + T" << default_fg << "\t"
		<< "Move to File End: " << lime_fg << "Ctrl + E" << default_fg << "\n"
		
		<< "Toggle Command Line: " << lime_fg << "Esc" << default_fg;
		
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

static bool contains(int numberOfArguments, const char* const* listOfArgumentStrings, const char* what){
	for(int i = 0; i < numberOfArguments; ++i){
		if(std::strcmp(listOfArgumentStrings[i], what) == 0){
			return true;
		}
	}
	return false;
}

int Lime::run(int numberOfArguments, const char* const* listOfArgumentStrings){
	const bool newFileFlag = contains(numberOfArguments, listOfArgumentStrings, "-n");
	if(numberOfArguments > 0){
		const char * const filepath = listOfArgumentStrings[0];
		const bool file_exists = std::filesystem::is_regular_file(std::filesystem::status(filepath));
		if(file_exists){
			const bool successful_read = this->textEditor.open(filepath);
			if(!successful_read){
				std::cout << "Error: could not read file: " << filepath << std::endl;
				return EXIT_FAILURE;
			}
		}
	}
	return this->run();
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
			this->prozess_key_event(event);
		}return;
		case Term::Event::Type::Screen : {
			this->prozess_screen_event(event);
		}return;
		case Term::Event::Type::Cursor : {
			this->prozess_cursour_event();	
		}return;
		case Term::Event::Type::CopyPaste : {
			this->prozess_copy_paste_event(std::move(event));
		}return; 
		default : {
			this->prozess_unhandeled_event(std::move(event));
		}return;
	}
}

#ifdef _WIN32
static bool read_clipboard_windows(std::string& clipboardText){
	// Open the clipboard
	if (!OpenClipboard(NULL)){
		return false;
	}

	// Get the clipboard data
	HANDLE handle = GetClipboardData(CF_TEXT);
	if (handle == NULL){
		CloseClipboard();
		return false;
	}

	// Lock the memory and retrieve the text
	char* text = static_cast<char*>(GlobalLock(handle));
	if (text == NULL){
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



static char * x11_paste_type(const Atom& atom, Display*& display, const Window& window, const Atom& UTF8, const int& XA_STRING) {
	// the unix implementation for copiing from the clipboard has been copied from:
	// https://github.com/exebook/x11clipboard/blob/master/x11paste.c
	// and modified accordingly
	XEvent event;
	int format;
	unsigned long N, size;
	char * data, * s = 0;
	Atom target,
		CLIPBOARD = XInternAtom(display, "CLIPBOARD", 0),
		XSEL_DATA = XInternAtom(display, "XSEL_DATA", 0);
	XConvertSelection(display, CLIPBOARD, atom, XSEL_DATA, window, CurrentTime);
	XSync(display, 0);
	XNextEvent(display, &event);
	
	switch(event.type) {
		case SelectionNotify:
		if(event.xselection.selection != CLIPBOARD) break;
		if(event.xselection.property) {
			XGetWindowProperty(event.xselection.display, event.xselection.requestor,
				event.xselection.property, 0L,(~0L), 0, AnyPropertyType, &target,
				&format, &size, &N,(unsigned char**)&data);
			if(target == UTF8 || target == XA_STRING) {
				s = strndup(data, size);
				XFree(data);
			}
			XDeleteProperty(event.xselection.display, event.xselection.requestor, event.xselection.property);
		}
	}
  return s;
}

static char *x11_paste() {
	// the unix implementation for copiing from the clipboard has been copied from:
	// https://github.com/exebook/x11clipboard/blob/master/x11paste.c
	// and modified accordingly
	const int XA_STRING = 31;
	static Display * display = XOpenDisplay(0);
	static Atom UTF8 = XInternAtom(display, "UTF8_STRING", True); 
	const int N = DefaultScreen(display);
	static Window window = XCreateSimpleWindow(display, RootWindow(display, N), 0, 0, 1, 1, 0,
		BlackPixel(display, N), WhitePixel(display, N)
	);	
	char * c = 0;
	if(UTF8 != None) c = x11_paste_type(UTF8, display, window, UTF8, XA_STRING);
	if(!c) c = x11_paste_type(XA_STRING, display, window, UTF8, XA_STRING);
	return c;
}

static bool read_clipboard_unix(std::string& clipboardText){
	clipboardText = x11_paste();
	return true;
}

#endif

static bool read_clipboard(std::string& clipboardText){
	#ifdef _WIN32
	return read_clipboard_windows(clipboardText);
	#else
	return read_clipboard_unix(clipboardText);
	#endif
}

void Lime::insert_from_clipboard(){
	std::string clipboardBuffer;
	const bool successfulRead = read_clipboard(clipboardBuffer);
	if(successfulRead){
		this->activeCursor->insert(clipboardBuffer.c_str(), clipboardBuffer.size());
	}else{
		this->topMessageBar.assign("Bad Copy Paste Event");
	}
}

void Lime::prozess_key_event(Term::Key keyEvent){
	// get analyse input
	const char32_t utf32 = static_cast<char32_t>(keyEvent);
	
	switch(utf32){
		//---- basics -----
		case Term::Key::CTRL_Q : this->quit(); break;
		case Term::Key::ESC : this->toggle_command_line(); break;
		case Term::Key::ENTER : this->activeCursor->enter(); break; 
		case Term::Key::BACKSPACE : this->activeCursor->Delete(); break;
		case Term::Key::DEL: this->activeCursor->erase(); break;
		//case Term::Key::TAB: this->activeCursor->insert('\t'); break;

		//---- i/o ----
		case Term::Key::CTRL_S : this->save(); break;
		
		//----- navigation and cursor movement -----
		case Term::Key::ALT + Term::Key::j : this->activeCursor->move_back(); break; 	// change to CTRL as soon as cpp-terminal library has support
		case Term::Key::ALT + Term::Key::i : this->activeCursor->move_up(); break;		// change to CTRL as soon as cpp-terminal library has support
		case Term::Key::ALT + Term::Key::l : this->activeCursor->move_forward(); break; // change to CTRL as soon as cpp-terminal library has support
		case Term::Key::ALT + Term::Key::k : this->activeCursor->move_down(); break;	// change to CTRL as soon as cpp-terminal library has support
		
		case Term::Key::ARROW_LEFT 	: this->activeCursor->move_back(); break;
		case Term::Key::ARROW_UP 	: this->activeCursor->move_up(); break;
		case Term::Key::ARROW_RIGHT : this->activeCursor->move_forward(); break; 
		case Term::Key::ARROW_DOWN 	: this->activeCursor->move_down(); break; 
		
		case Term::Key::CTRL_T : this->activeCursor->move_to_start_of_file();break;
		case Term::Key::CTRL_E : this->activeCursor->move_to_end_of_file();break;
		
		case Term::Key::ALT + Term::Key::u : this->activeCursor->move_to_start_of_line(); break;
		case Term::Key::ALT + Term::Key::o : this->activeCursor->move_to_end_of_line(); break;
		
		//case Term::Key::ALT + Term::Key::J : this->topMessageBar.assign("/*TODO: move one word left*/"); break;
		//case Term::Key::ALT + Term::Key::I : this->topMessageBar.assign("/*move one paragraph/codeblock up*/"); break;
		//case Term::Key::ALT + Term::Key::L : this->topMessageBar.assign("/*TODO: move one word right*/"); break; 
		//case Term::Key::ALT + Term::Key::K : this->topMessageBar.assign("/*TODO: move one paragraph/codeblock down*/"); break;
		
		// ------- editing -------
		case Term::Key::CTRL_V : this->insert_from_clipboard(); break;
		case Term::Key::CTRL_C : this->topMessageBar.assign("/* TODO: copy selection into clipboard */"); break;
		case Term::Key::CTRL_X : this->topMessageBar.assign("/* TODO: cut selection into clipboard */"); break;
		
		default:{
			if(!keyEvent.isALT() && !keyEvent.isCTRL()){
				const utf8::Char input(utf32);
				this->activeCursor->insert(input);
				this->topMessageBar.assign("Key press: ").append(input.to_std_string_view());
			}
		}break;	
	}
}
  
void Lime::prozess_empty_event(){
	this->topMessageBar.assign("Internal Error: Unhandeled Event type 'Empty' ");
}

void Lime::prozess_screen_event(Term::Screen screen){
	
	const auto columns = static_cast<TermGui::ScreenWidth::size_type>(screen.columns());
	const auto rows = static_cast<TermGui::ScreenWidth::size_type>(screen.rows());
	this->topMessageBar.assign("screen event : ").append(std::to_string(columns)).append(", ").append(std::to_string(rows));
	if (columns > 1 && rows > 1) {
		this->mainGrid.set_screen_width(TermGui::ScreenWidth{ .x = columns, .y = rows });
	}
}

void Lime::prozess_cursour_event(){
	this->topMessageBar.assign("Internal Error: Unhandeled Event type 'Cursor' ");
}

void Lime::prozess_copy_paste_event(Term::Event&& event){
	// This copy paste event will copy from a selection in the terminal
	// for example in wnidows when you mark some text in the console with the left mouse
	// the press the right mouse button twice
	// TODO: BugFix: handle insertion of multiple selected lines
	auto input = static_cast<std::string>(std::move(event));
	this->activeCursor->insert(input.c_str(), input.size());
}

void Lime::prozess_unhandeled_event(Term::Event&& event){
	/*TODO: a way to display error messages that do not disturb the writeing prozess
		For example by message windows or at a special place at the bottom of the screen
	*/
	this->topMessageBar.assign("Internal Error: Unhandeled Event type ID: ").append(std::to_string(static_cast<int>(event.type())));
}

/// parses a utf8 view of a command string into a vector of string views
/// returns a vector where each element holds one word or string ("..") of the command
static std::vector<utf8::string_view> parse_command_string(utf8::string_view commandString){
	std::vector<utf8::string_view> commandList;
	auto itr = commandString.cbegin();
	const auto end = commandString.cend();
	while(itr != end){
		//skip whitespaces
		if(utf8::is_whitespace(*itr) || utf8::is_control(*itr)){
			// go to next
			++itr;
		}else if(*itr == '"'){
			// find next " or end
			auto next = itr+1;
			for (; next != end; ++next) {
				if (*next == '"') break;
			}
			const bool has_quotation_marks = next != end;
			const auto distance = std::distance(itr + 1, next);
			const utf8::string_view::const_pointer pChar = &(*(itr + 1));
			commandList.emplace_back(pChar, distance);
			itr = next + has_quotation_marks;
		}else{
			// find next whitespaces
			auto next = itr+1;
			for (; next != end; ++next) {
				if (utf8::is_whitespace(*next)) break;
			}
			const auto distance = std::distance(itr, next);
			const utf8::string_view::const_pointer pChar = &(*itr);
			commandList.emplace_back(pChar, distance);
			itr = next;
		}
	}
	return commandList;
}



void Lime::command_line_callback(void* limePtr, utf8::string_view commands){
	Lime* This = reinterpret_cast<Lime*>(limePtr);
	
	// parse commands into list of commands
	const std::vector<utf8::string_view> commandList = parse_command_string(commands);
	if(commandList.empty()){
		This->topMessageBar.assign("Error: empty command string");
		return;
	}
	
	// prozess commanad
	if(commandList[0] == "save-as"){
		This->save_as(commandList);
	}else if(commandList[0] == "save"){
		This->save();
	}else if(commandList[0] == "open"){
		This->open(commandList);
	}else if(commandList[0] == "set"){
		This->set(commandList);
	}else{
		This->topMessageBar.assign("Unsupported Command: ").append(commands);
	}
	
	// go back into the editor mode
	This->toggle_command_line();
	This->deactivate_command_line();
}

void Lime::set(const std::vector<utf8::string_view>& commands){
	if(commands.size() < 3){
		this->topMessageBar.assign("Error: the set command needs 2 parameters: set <value_name> <new_value>.");
		return;
	}
	if(commands[1] != "tab-size"){
		this->topMessageBar.assign("Error: unsupported set value name.");
		return;
	}
	std::int32_t value = 0;
	const auto read = commands[2].parse_int32(&value);
	if(read == 0){
		this->topMessageBar.assign("Error: tab size has to be an intager number.");
		return;
	}
	if(value < 0){
		this->topMessageBar.assign("Error: tab size has to be positive.");
		return;
	}
	if(value == 0){
		this->topMessageBar.assign("Error: tab size has to be greater than zero.");
		return;
	}
	
	this->activeEditor->tab_size(value);
	this->topMessageBar.assign("Set tab size to ").append(std::to_string(value));
}

void Lime::save(){
	const bool successful_write = this->textEditor.save();
	if (successful_write) {
		this->topMessageBar.assign("Sussessfully saved file");
		return;
	}else{
		this->topMessageBar.assign("Error: could not write file");
	}
}

void Lime::save_as(const std::vector<utf8::string_view>& commands){
	//TODO: Check if the filename already exists and ask wether or not the file should be overridden.
	if(commands.size() < 2){
		this->topMessageBar.assign("Error: the command open needs a filename");
		return;
	}
	this->textEditor.save_as(commands[1].to_std_string());
}

void Lime::open(const std::vector<utf8::string_view>& commands){
	if(commands.size() < 2){
		this->topMessageBar.assign("Error: the command open needs a filename");
		return;
	}
	this->textEditor.open(commands[1].to_std_string());
}

void Lime::render(std::string& outputString) const{
	this->mainGrid.render(outputString);
}

void Lime::draw(const std::string& outputString) const{
	Term::terminal << Term::cursor_move(0, 0) << outputString << std::flush;
}

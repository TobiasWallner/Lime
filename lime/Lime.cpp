
// Project
#include "Lime.hpp"

// C std
#include <stdlib.h>
#include <string.h>

// C++ std
#include <iostream>
#include <cctype>
#include <cstdint>
#include <chrono>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>
#include <cpp-terminal/input.hpp>
#include <cpp-terminal/iostream.hpp>
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

const TermGui::Command Lime::commandList[]{
	TermGui::Command{
		.name = utf8::string("open"),
		.info = utf8::string("+ <path> opens the specified file."),
		.flags = std::vector<TermGui::Command::Flag>({
			TermGui::Command::Flag{utf8::string("-r"), utf8::string("opens the file in read-only mode")},
			TermGui::Command::Flag{utf8::string("-a"), utf8::string("opens the file in append-only mode")},  
			TermGui::Command::Flag{utf8::string("-copy"), utf8::string("+ <path> makes a copy of the file and opens it")},  
		}),
		.callbackFn = &Lime::open,
	},
	TermGui::Command{
		.name = utf8::string("quit"),
		.info = utf8::string("quits the editor if there are no unsaved files"),
		.flags = std::vector<TermGui::Command::Flag>({
			TermGui::Command::Flag{utf8::string("-s"), utf8::string("saves all files before quitting")},
			TermGui::Command::Flag{utf8::string("-f"), utf8::string("forces to quit and discards all unsaved changes")},  
		}),
		.callbackFn = &Lime::quit,
	},
	TermGui::Command{
		.name = utf8::string("save"),
		.info = utf8::string("saves the active file"),
		.flags = std::vector<TermGui::Command::Flag>({
			TermGui::Command::Flag{utf8::string("-as <path>"), utf8::string("changes the file to the provided 'path' and saves it. Cannot be used together with '-all'")},
			TermGui::Command::Flag{utf8::string("-all"), utf8::string("saves all open files")},  
			TermGui::Command::Flag{utf8::string("-copy <path>"), utf8::string("makes a copy of the file and saves it under the provided path. Cannot be used together with '-all'")},  
		}),
		.callbackFn = &Lime::save,
	},
};

Lime::Lime() : 
	mainGrid(	
		TermGui::ScreenPosition{.x = 1, .y = 1}, 
		TermGui::ScreenWidth{	
			.x = static_cast<TermGui::ScreenWidth::size_type>(Term::screen_size().columns()),
			.y = static_cast<TermGui::ScreenWidth::size_type>(Term::screen_size().rows())
		}
	)								
{   
	{// init top message bar
		auto Label = std::make_unique<TermGui::Label>();
		this->topMessageBar = Label.get();
		this->mainGrid.push_back(std::move(Label));
	}
	{// init text editor grid
		auto hGrid = std::make_unique<TermGui::HorizontalGrid>(1.f);
		this->textEditorGrid = hGrid.get();
		this->mainGrid.push_back(std::move(hGrid));
		
		{// init text editor
			auto textEditor = std::make_unique<TermGui::TextEditor>();
			this->textEditor = textEditor.get();
			this->textEditorGrid->push_back(std::move(textEditor)); // set the maximal width to 80
			this->textEditorGrid->set_centering(true); //TODO: make min, max width/height and centering functions again that when set or changed trigger a re-distribution
		}
		
	}
	{// init info text
		auto Label = std::make_unique<TermGui::Label>();
		this->infoText = Label.get();
		this->mainGrid.push_back(std::move(Label));
	}
	{// init command line
		auto commandLine = std::make_unique<TermGui::CommandLine>(
			this,
			TermGui::const_command_range{Lime::commandList, Lime::commandList + sizeof(this->commandList)}, 
			TermGui::CommandLine::string_type("Press 'Esc' to toggle into the command line"));
		this->commandLine = commandLine.get();
		this->mainGrid.push_back(std::move(commandLine));
	}
	{// init bottom Message Bar
		auto Label = std::make_unique<TermGui::Label>();
		this->bottomMessageBar = Label.get();
		this->mainGrid.push_back(std::move(Label));
	}
	
	this->topMessageBar->assign("Hallo :)\nthis is two lines long");

	activeEditor = this->textEditor;
	this->deactivate_command_line();
	this->activate_text_editor();
}

inline void Lime::activate_command_line(){
	this->commandLine->show_cursor(true);
	this->activeCursor = this->commandLine;
	
	// load infotext
	this->infoText->clear();
	
	const auto lime_fg = TermGui::fg_color(LimeTheme::green[0], LimeTheme::green[1], LimeTheme::green[2]);
	const auto default_fg = TermGui::default_fg_color();
	
	*this->infoText  << lime_fg << "save" << default_fg << "  ...............  saves the current file\n"
					<< lime_fg << "save-as <filename>" << default_fg << " ..  saves the current file under a new name\n"
					<< lime_fg << "open <filename>" << default_fg << "  ....  opens the file under the provided name";
	
}

void Lime::activate_text_editor(){
	this->textEditor->show_cursor(true);
	this->activeCursor = this->textEditor;
		
		//load infotext
	this->infoText->clear();
	
	const auto lime_fg = TermGui::fg_color(LimeTheme::green[0], LimeTheme::green[1], LimeTheme::green[2]);
	const auto default_fg = TermGui::default_fg_color();
	
	*this->infoText  
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
			const bool successful_read = this->textEditor->open(filepath);
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

static bool is_ignore_event(const Term::Event& event){
	switch(event.type()){
		case Term::Event::Type::Empty : {
			return true;
		}break;
		case Term::Event::Type::Key : {
			const Term::Key key = event;
			return key == Term::Key::ALT || key == Term::Key::NO_KEY || key == Term::Key::NUL;
		}break;
		case Term::Event::Type::Screen : {
			const Term::Screen screen = event;
			const auto columns = static_cast<TermGui::ScreenWidth::size_type>(screen.columns());
			const auto rows = static_cast<TermGui::ScreenWidth::size_type>(screen.rows());
			return !(columns > 1 && rows > 1);
		}break;
		case Term::Event::Type::Cursor : {
			return true;
		}break;
		case Term::Event::Type::CopyPaste : {
			// TODO: remove the static_cast as soon as cpp-terminal fixes the API of Term::Event
			return static_cast<std::string>(event).size() == 0;
		}break; 
		default : {
			return true;
		}break;
	}
}

void Lime::run_main_loop(){
	// reserve memory in advance
	std::string outputString; 	
	outputString.reserve(1024 * 10); 
	
	// initial render of the whole screen
	this->render(outputString);
	this->draw(outputString);
	
	// main loop
	while(this->main_loop_continue){
		Term::Event event = Term::read_event();
		if(!is_ignore_event(event)){
			this->prozess_event(std::move(event));
			outputString.clear();
			this->render(outputString);
			this->draw(outputString); // <-- call to ::cout
		}
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
		this->topMessageBar->assign("Bad Copy Paste Event");
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
		
		//case Term::Key::ALT + Term::Key::J : this->topMessageBar->assign("/*TODO: move one word left*/"); break;
		//case Term::Key::ALT + Term::Key::I : this->topMessageBar->assign("/*move one paragraph/codeblock up*/"); break;
		//case Term::Key::ALT + Term::Key::L : this->topMessageBar->assign("/*TODO: move one word right*/"); break; 
		//case Term::Key::ALT + Term::Key::K : this->topMessageBar->assign("/*TODO: move one paragraph/codeblock down*/"); break;
		
		// ------- editing -------
		case Term::Key::CTRL_V : this->insert_from_clipboard(); break;
		case Term::Key::CTRL_C : this->topMessageBar->assign("/* TODO: copy selection into clipboard */"); break;
		case Term::Key::CTRL_X : this->topMessageBar->assign("/* TODO: cut selection into clipboard */"); break;
		
		default:{
			if(!keyEvent.isALT() && !keyEvent.isCTRL()){
				const utf8::Char input(utf32);
				this->activeCursor->insert(input);
				this->topMessageBar->assign("Key press: ").append(input.to_std_string_view());
			}
		}break;	
	}
}
  
void Lime::prozess_empty_event(){
	this->topMessageBar->assign("Internal Error: Unhandeled Event type 'Empty' ");
}

void Lime::prozess_screen_event(Term::Screen screen){
	
	const auto columns = static_cast<TermGui::ScreenWidth::size_type>(screen.columns());
	const auto rows = static_cast<TermGui::ScreenWidth::size_type>(screen.rows());
	this->topMessageBar->assign("screen event : ").append(std::to_string(columns)).append(", ").append(std::to_string(rows));
	if (columns > 1 && rows > 1) {
		this->mainGrid.set_screen_width(TermGui::ScreenWidth{ .x = columns, .y = rows });
	}
}

void Lime::prozess_cursour_event(){
	this->topMessageBar->assign("Internal Error: Unhandeled Event type 'Cursor' ");
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
	this->topMessageBar->assign("Internal Error: Unhandeled Event type ID: ").append(std::to_string(static_cast<int>(event.type())));
}

void Lime::set(const std::vector<utf8::string_view>& commands){
	if(commands.size() < 3){
		this->topMessageBar->assign("Error: the set command needs 2 parameters: set <value_name> <new_value>.");
		return;
	}
	if(commands[1] != "tab-size"){
		this->topMessageBar->assign("Error: unsupported set value name.");
		return;
	}
	std::int32_t value = 0;
	const auto read = commands[2].parse_int32(&value);
	if(read == 0){
		this->topMessageBar->assign("Error: tab size has to be an intager number.");
		return;
	}
	if(value < 0){
		this->topMessageBar->assign("Error: tab size has to be positive.");
		return;
	}
	if(value == 0){
		this->topMessageBar->assign("Error: tab size has to be greater than zero.");
		return;
	}
	
	this->activeEditor->tab_size(value);
	this->topMessageBar->assign("Set tab size to ").append(std::to_string(value));
}


void Lime::open(void* ptr, const std::vector<utf8::string_view>& commands){
	Lime* This = reinterpret_cast<Lime*>(ptr);
	switch(commands.size()){
		case 1 : {
			return (void)This->commandLine->message.assign("Error: the command open needs a filename");
		}break;
		case 2 : {
			(void)This->open(commands[1].to_std_string());
		}break;
		case 3 : {
			if(commands[2] == "-r"){
				return (void)This->commandLine->message.assign("Error: open: -r flag is not supported yet");
			}else if(commands[2] == "-a"){
				return (void)This->commandLine->message.assign("Error: open: -a flag is not supported yet");
			}else if(commands[2] == "-copy"){
				return (void)This->commandLine->message.assign("Error: open: -copy flag is not supported yet");
			}
		}break;
	}
	This->commandLine->message.assign("Error: incorrect arguments");
}

bool Lime::open(const std::string& path){
	const bool successfulRead = this->textEditor->open(path);
	if (!successfulRead) {
		this->commandLine->message.assign("Error: could not read file");
	}
	return successfulRead;
}


void Lime::quit(void* ptr, const std::vector<utf8::string_view>& commands){
	Lime* This = reinterpret_cast<Lime*>(ptr);
	switch(commands.size()){
		case 1 : {
			return (void)This->quit(); //TODO: -> save_quit();
		}break;
		case 2 : {
			if(commands[2] == "-s"){
				return (void)This->commandLine->message.assign("Error: open: -s flag is not supported yet");
			}else if(commands[2] == "-f"){
				return (void)This->quit(); //TODO: -> force_quit();
			}
		}break;
	}
	This->commandLine->message.assign("Error: incorrect arguments");
}


void Lime::save(void* ptr, const std::vector<utf8::string_view>& commands){
	Lime* This = reinterpret_cast<Lime*>(ptr);
	switch(commands.size()){
		case 1 : {
			return (void)This->save();
		}break;
		case 2 : {
			if(commands[2] == "-all"){
				return (void)This->commandLine->message.assign("Error: save: -all flag is not supported yet");
			}
		}break;
		case 3 : {
			if(commands[2] == "-as"){
				return (void)This->save_as(commands[2].to_std_string());
			}else if(commands[2] == "-copy"){
				return (void)This->commandLine->message.assign("Error: save: -copy flag is not supported yet");
			}
		}break;
	}
	This->commandLine->message.assign("Error: incorrect arguments");
}

bool Lime::save(){
	const bool successful_write = this->textEditor->save();
	if (!successful_write) {
		this->commandLine->message.assign("Error: could not write file");
	}
	return successful_write;
}

bool Lime::save_as(const std::string& path){
	return this->textEditor->save_as(path);
}

void Lime::render(std::string& outputString) const{
	this->mainGrid.render(outputString);
}

void Lime::draw(const std::string& outputString) {
	auto start = std::chrono::high_resolution_clock::now();
	
	Term::cout << outputString << std::flush;

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = duration_cast<std::chrono::microseconds>(stop - start);
	this->bottomMessageBar->assign("draw time: ").append(std::to_string(duration.count())).append("us");
}

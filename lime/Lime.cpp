
// Project
#include "Lime.hpp"

// C std
#include <stdlib.h>
#include <string.h>

// C++ std
#include <type_traits>
#include <iostream>
#include <cctype>
#include <cstdint>
#include <chrono>
#include <filesystem>

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

static TermGui::Command generate_cd_command(){
	TermGui::Command result{
		.name = "cd",
		.info = "+ <path> changes the current directory to the new ",
		.flags = std::vector<TermGui::Command::Flag>(),
		.callbackFn = &Lime::change_directory,
	};
	
	return result;
}

static TermGui::Command generate_pwd_command(){
	TermGui::Command result{
		.name = "pwd",
		.info = "prints the working directory",
		.flags = std::vector<TermGui::Command::Flag>(),
		.callbackFn = &Lime::print_working_directory,
	};
	
	return result;
}

static TermGui::Command generate_open_command(){
	TermGui::Command result{
		.name = "open",
		.info = "+ <path> opens the specified file.",
		.flags = std::vector<TermGui::Command::Flag>({
			TermGui::Command::Flag{"-r", "opens the file in read-only mode"},
			TermGui::Command::Flag{"-a", "opens the file in append-only mode"},  
			TermGui::Command::Flag{"-copy", "+ <path> makes a copy of the file and opens it"},  
		}),
		.callbackFn = &Lime::open,
	};
	
	return result;
}

static TermGui::Command generate_quit_command(){
	TermGui::Command result{
		.name = "quit",
		.info = "quits the editor if there are no unsaved files",
		.flags = std::vector<TermGui::Command::Flag>({
			TermGui::Command::Flag{"-s", "saves all files before quitting"},
			TermGui::Command::Flag{"-f", "forces to quit and discards all unsaved changes"},  
		}),
		.callbackFn = &Lime::quit,
	};
	return result;
}

static TermGui::Command generate_save_command(){
	TermGui::Command result{
		.name = "save",
		.info = "saves the active file",
		.flags = std::vector<TermGui::Command::Flag>({
			TermGui::Command::Flag{"-as", "+ <path>, changes the file to the provided 'path' and saves it. Cannot be used together with '-all'"},
			TermGui::Command::Flag{"-all", "saves all open files"},  
			TermGui::Command::Flag{"-copy", "+ <path>, makes a copy of the file and saves it under the provided path. Cannot be used together with '-all'"},  
		}),
		.callbackFn = &Lime::save,
	};
	
	return result;
}

static TermGui::Command generate_set_command(){
	TermGui::Command result{
		.name = "set",
		.info = "<value_name> <new_value> sets a value to the provided one",
		.flags = std::vector<TermGui::Command::Flag>({
			TermGui::Command::Flag{"tabsize", "+ <number>, sets the tabsize"}
		}),
		.callbackFn = &Lime::set,
	};
	return result;
}

const TermGui::Command Lime::commandList[]{ // !!! Has to be in alphabetical order !!!
	generate_cd_command(),
	generate_open_command(),
	generate_pwd_command(),
	generate_quit_command(),
	generate_save_command(),
	generate_set_command(),
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
	{// init command line
		auto commandLine = std::make_unique<TermGui::CommandLine>(
			this,
			TermGui::const_command_range{Lime::commandList, Lime::commandList + sizeof(this->commandList) / sizeof(std::remove_all_extents<decltype(this->commandList)>::type)},
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
}

void Lime::activate_text_editor(){
	this->textEditor->show_cursor(true);
	this->activeCursor = this->textEditor;
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

void Lime::force_quit(){ 
	this->main_loop_continue = false; 
	std::cout << "exiting Lime" << std::endl;
}

void Lime::quit(){
	/**
		Savely quit the program.
	*/
	if(this->has_unsaved_changes()){
		this->commandLine->message.assign("Unsaved changes, use ':quit -s' to save all unsaved files or ':quit -f' to force quit."); 
	}else{
		this->force_quit();
	}
}

void Lime::save_quit(){
	this->save_all();
	this->force_quit();
}


static bool is_ignore_event(const Term::Event& event){
	switch(event.type()){
		case Term::Event::Type::Empty : {
			return true;
		}break;
		case Term::Event::Type::Key : {
			const Term::Key key = event;
			return key == Term::Key::Value::NoKey || key == Term::Key::Value::Null;
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
			return event.get_if_copy_paste()->empty();
		}break; 
		default : {
			return true;
		}break;
	}
}

void Lime::run_main_loop(){
	// reserve memory in advance
	std::string outputString; 	
	outputString.reserve(static_cast<size_t>(1024 * 10)); 
	
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
	switch(keyEvent){
		//---- basics -----
		case Term::Key::Value::Ctrl_Q : this->quit(); break;
		case Term::Key::Value::Esc : this->toggle_command_line(); break;
		case Term::Key::Value::Enter : this->activeCursor->enter(); break; 
		case Term::Key::Value::Backspace : this->activeCursor->Delete(); break;
		case Term::Key::Value::Del : this->activeCursor->erase(); break;

		//---- i/o ----
		case Term::Key::Value::Ctrl_S : this->save(); break;
		
		//----- navigation and cursor movement -----
		case Term::MetaKey::Value::Alt + Term::Key::Value::j : this->activeCursor->move_back(); break; 	// change to CTRL as soon as cpp-terminal library has support
		case Term::MetaKey::Value::Alt + Term::Key::Value::i : this->activeCursor->move_up(); break;		// change to CTRL as soon as cpp-terminal library has support
		case Term::MetaKey::Value::Alt + Term::Key::Value::l : this->activeCursor->move_forward(); break; // change to CTRL as soon as cpp-terminal library has support
		case Term::MetaKey::Value::Alt + Term::Key::Value::k : this->activeCursor->move_down(); break;	// change to CTRL as soon as cpp-terminal library has support
		
		case Term::Key::Value::ArrowLeft 	: this->activeCursor->move_back(); break;
		case Term::Key::Value::ArrowUp 	: this->activeCursor->move_up(); break;
		case Term::Key::Value::ArrowRight 	: this->activeCursor->move_forward(); break; 
		case Term::Key::Value::ArrowDown 	: this->activeCursor->move_down(); break; 
		
		case Term::MetaKey::Value::Alt + Term::Key::Value::b : this->activeCursor->move_to_start_of_file();break;
		case Term::MetaKey::Value::Alt + Term::Key::Value::e : this->activeCursor->move_to_end_of_file();break;
		case Term::MetaKey::Value::Alt + Term::Key::Value::u  : this->activeCursor->move_to_start_of_line(); break;
		case Term::MetaKey::Value::Alt + Term::Key::Value::o  : this->activeCursor->move_to_end_of_line(); break;
		
		// ------- editing -------
		case Term::Key::Value::Ctrl_V : this->insert_from_clipboard(); break;		
		case Term::Key::Value::Ctrl_C : this->topMessageBar->assign("/* TODO: copy selection into clipboard */"); break;
		case Term::Key::Value::Ctrl_X : this->topMessageBar->assign("/* TODO: cut selection into clipboard */"); break;
		
		default:{
			if(!keyEvent.hasAlt() && !keyEvent.hasCtrl()){
				const utf8::Char input(keyEvent.value);
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

void Lime::set(void* ptr, const std::vector<utf8::const_string_view>& commands){
	Lime* This = reinterpret_cast<Lime*>(ptr);
	if(commands.size() == 3){
		//error
		This->commandLine->message.assign("Error: the set command needs 2 parameters: set <value_name> <new_value>.");
	}else if(commands[1] == "tabsize"){
		This->set_tab_size(commands[2]);
	}else{
		//error
		This->commandLine->message.assign("Error: unsupported set value name.");
	}
}

void Lime::set_tab_size(utf8::const_string_view str){
	std::int32_t value = 0;
	const auto itr = scan_i32(str.begin(), str.end(), value);
	if(itr == str.begin()){
		//error
		this->commandLine->message.assign("Error: tab size has to be an intager number.");
	}else if(value < 0){
		//error
		this->commandLine->message.assign("Error: tab size has to be positive.");
	}else if(value == 0){
		//error
		this->commandLine->message.assign("Error: tab size has to be greater than zero.");
	}else{
		this->activeEditor->tab_size(value);
	}
}

void Lime::change_directory(void* ptr, const std::vector<utf8::const_string_view>& commands){
	Lime* This = reinterpret_cast<Lime*>(ptr);
	switch(commands.size()){
		case 1 : {
			This->commandLine->message = "Error: cd: the command, cange directory, needs a path";
		}break;
		
		case 2 : {
			const auto path = static_cast<std::string_view>(commands[1]);
			if(std::filesystem::is_directory(path)){
				std::error_code ec;
				std::filesystem::current_path(path, ec);
				if(ec){
					//error:
					This->commandLine->message.assign("Error: cd: system:").append(ec.message());
				}
			}else{
				This->commandLine->message = "Error: cd: the provided path has to be a directory but it is not.";
			}
		}break;
		
		default : {
			This->commandLine->message = "Error: cd: only needs one parameter: ch + <path>";
		}break;
	}
}

void Lime::print_working_directory(void* ptr, const std::vector<utf8::const_string_view>& commands){
	Lime* This = reinterpret_cast<Lime*>(ptr);
	if(commands.size() == 1){
		const auto path = std::filesystem::current_path();
		This->commandLine->message = path.string();
	}else{
		This->commandLine->message = "Error: pwd: this command just prints the directory and does not accept any parameters";
	}
}

void Lime::open(void* ptr, const std::vector<utf8::const_string_view>& commands){
	Lime* This = reinterpret_cast<Lime*>(ptr);
	switch(commands.size()){
		case 1 : {
			This->commandLine->message = "Error: the command 'open' needs a filename";
		}break;
		case 2 : {
			const std::filesystem::path path(std::to_address(commands[1].begin()), std::to_address(commands[1].end()));
			const bool successful_read = This->open(path);
			if (!successful_read) {
				This->commandLine->message.assign("Error: could not read filename")
					.append("'").append(commands[1]).append("'");
			}
		}break;
		case 3 : {
			if(commands[2] == "-r"){
				This->commandLine->message.assign("Error: open: -r flag is not supported yet");
			}else if(commands[2] == "-a"){
				This->commandLine->message.assign("Error: open: -a flag is not supported yet");
			}else if(commands[2] == "-copy"){
				This->commandLine->message.assign("Error: open: -copy flag is not supported yet");
			}else {
				This->commandLine->message.assign("Error: '").append(commands[2]).append("' is not a valid flag");
			}
		}return;
		default: {
			This->commandLine->message.assign("Error: incorrect number of arguments");
		}break;
	}
}

bool Lime::open(const std::filesystem::path& path){
	const bool successfulRead = this->textEditor->open(path);
	if (!successfulRead) {
		this->commandLine->message.assign("Error: could not read file");
	}
	return successfulRead;
}

bool Lime::has_unsaved_changes() const{
	return textEditor->has_unsaved_changes(); // TODO: later ask all editors
}

void Lime::quit(void* ptr, const std::vector<utf8::const_string_view>& commands){
	Lime* This = reinterpret_cast<Lime*>(ptr);
	switch(commands.size()){
		case 1 : {
			This->quit();
		}break;
		case 2 : {
			if(commands[1] == "-s"){
				This->save_quit();
			}else if(commands[1] == "-f"){
				This->force_quit(); //TODO: -> force_quit();
			}else {
				This->commandLine->message.assign("Error: '").append(commands[1]).append("' is not a valid flag");
			}
		}break;
		default: {
			This->commandLine->message.assign("Error: incorrect number of arguments"); 
		}break;
	}
}


void Lime::save(void* ptr, const std::vector<utf8::const_string_view>& commands){
	Lime* This = reinterpret_cast<Lime*>(ptr);
	switch(commands.size()){
		case 1 : {
			return (void)This->save();
		}break;
		case 2 : {
			if(commands[1] == "-all"){
				return (void)This->commandLine->message.assign("Error: save: -all flag is not supported yet");
			}else {
				This->commandLine->message.assign("Error: '").append(commands[1]).append("' is not a valid flag");
			}
		}break;
		case 3 : {
			if(commands[1] == "-as"){
				std::string_view strPath = commands[2];
				const std::filesystem::path path(strPath);
				return (void)This->save_as(path);
			}else if(commands[1] == "-copy"){
				return (void)This->commandLine->message.assign("Error: save: -copy flag is not supported yet");
			}else {
				This->commandLine->message.assign("Error: '").append(commands[1]).append("' is not a valid flag");
			}
		}break;
		default: {
			This->commandLine->message.assign("Error: incorrect number of arguments");
		}break;
	}
}

bool Lime::save(){
	const bool successful_write = this->textEditor->save();
	if (!successful_write) {
		this->commandLine->message.assign("Error: could not write file");
	}
	return successful_write;
}

bool Lime::save_all(){
	bool successful_write = true;
	
	//TODO: ask all text editors instead
	successful_write &= this->textEditor->save();
	
	return successful_write;
}

bool Lime::save_as(const std::filesystem::path& path){
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

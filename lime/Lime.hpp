
// cpp-terminal
#include <cpp-terminal/event.hpp>

//utf8 string
#include <utf8/string.hpp>

// cpp-terminal-gui
#include <cpp-terminal-gui/ColorString.hpp>
#include <cpp-terminal-gui/TextEditor.hpp>
#include <cpp-terminal-gui/CommandLine.hpp>
#include <cpp-terminal-gui/VerticalGrid.hpp>
#include <cpp-terminal-gui/HorizontalGrid.hpp>
#include <cpp-terminal-gui/Label.hpp>

class Lime{
	
	static const TermGui::Command commandList[]; // sorted array of commans
	
	TermGui::VerticalGrid mainGrid;
	
	TermGui::HorizontalGrid* textEditorGrid = nullptr;
	TermGui::Label* topMessageBar = nullptr;
	TermGui::Label* bottomMessageBar = nullptr;
	TermGui::TextEditor* textEditor = nullptr;
	TermGui::CommandLine* commandLine = nullptr;
	TermGui::TextEditor * activeEditor = nullptr;
	TermGui::EditTrait * activeCursor = nullptr;
	
	bool main_loop_continue = true;
	
	void init_command_list(); 
	
public:
	
	Lime();
	
	/**
		The Copy consturctor of the class Lime is deleted, because the 
		it may hold open files or handels that should not be copied but 
		moved instead.
	*/
	Lime(const Lime&) = delete;
	Lime& operator = (const Lime&) = delete;
	
	/**
		Enable default move constructor
	*/
	Lime(Lime&&) = default;
	Lime& operator = (Lime&&) = default;
	
	/**
		run the lime text editor with the provided console arguments
		
		Returns:
		--------
		* returns EXIT_SUCCESS if the editor finished and closes without any errors.
		* returns EXIT_SUCCESS if errors appeared that the editor could handle with or
			without the help of some user input
		* returns EXIT_FAILURE if an error occured that the editor can do nothing about.
			like un-mounted input and output streams to the console.
	*/
	int run();

	int run(int numberOfArguments, const char * const* listOfArgumentStrings);

	/**
		Savely quits the editor from the current state.
		May asks/prompts the user to save unsaved files before 
		quitting and canceles the quitting event.
		
		(May feed the quit event in the received events.)
	*/
	void quit();
	
//private:
	void activate_command_line();
	
	inline bool is_command_line_active() const { return this->activeCursor == this->commandLine; }
	
	inline void deactivate_command_line(){
		this->commandLine->show_cursor(false);
		if(this->is_command_line_active()){
			this->activeCursor = nullptr;
		}
	}
	
	void activate_text_editor();
	
	inline bool is_text_editor_active() const { return this->activeCursor == this->textEditor; }
	
	inline void deactivate_text_editor(){
		this->textEditor->show_cursor(false);
		if(this->is_text_editor_active()){
			this->activeCursor = nullptr;	
		}
	}
	
	/// if the main editor is active -> sets the console as the active editor
	/// if the console is the active editor -> sets the textEditor as the active editor
	inline void toggle_command_line(){
		if(this->is_text_editor_active()){
			this->deactivate_text_editor();
			this->activate_command_line();
		}else{
			this->deactivate_command_line();
			this->activate_text_editor();
		}
	}

	/**
		the main loop does three things
			1. wait for and event/input
			2. prozess the input
			3. display the output (may already be included in prozess input)
	*/
	void run_main_loop();
	
	/**
		Prozesses any event like a key press or the resizeing of the console window and
		passes them to the corresponding event handlers.
	*/
	void prozess_event(Term::Event&& event);
	
	/**
		Prozesses a key event and passes it to the GUI object that is currently in focus
		and has the active cursor. 
	*/
	void prozess_key_event(Term::Key keyEvent);	
	
	/**
		TODO: do more research what this event does
	*/
	void prozess_empty_event();
	
	/**
		Prozesses a screen event, like the resizeing of the console window and passes the 
		new length and width recursively to all GUI elements so that they can dynamically
		resize and adapt to the new avaliable screen size.
	*/
	void prozess_screen_event(Term::Screen screen);
	
	/**
		Prozesses a cursour event
	*/
	void prozess_cursour_event();
	
	/**
		Prozesses a copy paste event
	*/
	void prozess_copy_paste_event(Term::Event&& event);
	
	/**
		Event handler that handles any un-handled events by for example displaying an 
		error message somewhere on the screen and maybe additionally print a log file
		with further information about the event type.
	*/
	void prozess_unhandeled_event(Term::Event&& event);
	
	/// renders the whole displayed content of the editor into a string that can be printed to the screen
	void render(std::string& outputString) const;
	
	/// clears the screen and draws the output string onto the screen
	void draw(const std::string& outputString);
	
	/// inserts the content from the clipboard into the current active editor.
	void insert_from_clipboard();
	
	static void change_directory(void* ptr, const std::vector<utf8::string_view>& commands);
	static void print_working_directory(void* ptr, const std::vector<utf8::string_view>& commands);
	
	/// opens the file at the specified location and loads it into a new or empty and unnamed editor that is in focus
	static void open(void* ptr, const std::vector<utf8::string_view>& commands);
	bool open(const std::string& path);
	
	static void quit(void* ptr, const std::vector<utf8::string_view>& commands);
	
	/// saves the currently focussed file at the currently specified filepath
	static void save(void* ptr, const std::vector<utf8::string_view>& commands);
	bool save();
	bool save_as(const std::string& path);
	
	static void set(void* ptr, const std::vector<utf8::string_view>& commands);
	void set_tab_size(utf8::string_view tabSize);
};
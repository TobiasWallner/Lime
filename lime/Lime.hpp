
// cpp-terminal
#include <cpp-terminal/event.hpp>

// cpp-terminal-gui
#include <cpp-terminal-gui/ColorString.hpp>
#include <cpp-terminal-gui/TextEditor.hpp>


class Lime{
	
	TermGui::ColorString topMessageBar;
	TermGui::TextEditor textEditor;
	TermGui::ColorString infoText;
	
	bool main_loop_continue = true;
	
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

	/**
		Savely quits the editor from the current state.
		May asks/prompts the user to save unsaved files before 
		quitting and canceles the quitting event.
		
		(May feed the quit event in the received events.)
	*/
	void quit();
	
private:

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
	void prozess_screen_event();
	
	/**
		Prozesses a cursour event
	*/
	void prozess_cursour_event();
	
	/**
		Prozesses a copy paste event
	*/
	void prozess_copy_paste_event();
	
	/**
		Event handler that handles any un-handled events by for example displaying an 
		error message somewhere on the screen and maybe additionally print a log file
		with further information about the event type.
	*/
	void prozess_unhandeled_event(Term::Event&& event);
	
	/// renders the whole displayed content of the editor into a string that can be printed to the screen
	void render(std::string& outputString) const;
	
	/// clears the screen and draws the output string onto the screen
	void draw(const std::string& outputString) const;

	/// copies the clipboard text into screen, attatch text at the end of main string
	bool RetrieveClipboardTextWindows(std::string& clipboardText) const;

	bool RetrieveClipboardTextUnix(std::string& clipboardText) const;
};
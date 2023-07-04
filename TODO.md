Issues
------
+ Figure out why the editor crashes, if the screen width is too small
+ Figure out why sometimes random letters are printed on the top right of the screen
+ the save command does not work
+ when the console is not in text wrap mode, then the application crashes when resizeing the 
	console to a smaller screen size.
+ Sometimes the screen cursor gets printed, despite it being deactivated. Figure out why and make it
	disappear.
+ Figure out how escape codes get inserted into the editor and prevent that from happening

TODOs
-----
+ cpp-terminal has implemented a new way for handling utf8 codes -> download the newes version and update Lime
+ Do not re-render the whole screen but only the parts that change
+ Do not trigger a render if Ctrl or Alt is pressed
+ When the cursor is moveing up and down it should stay at the current colum and not jump fom end 
	to end
+ For objects with the TextTrait, change the methods so that they output a string that only changes
	the view characters on screen that actually got changed.
+ make a class that contains a textEditor as well as the title of the file and prints that
	ontop of the editor. It should also print a '*' if the file is unsaved.
+ consider printing status and result messages in the console entry
+ place the basic movements and controls at the end of a file where the '~' are printed
+ remove the topMessageBar
+ make tabs
+ implement a command that opens another file in a tab or in a new window.
+ implement a command that closes a file, instead of the editor.
+ implement a dynamic message bar that prints information of commands that start with the 
	same letters that have just been typed, and if a command has been completed show information
	and a small help text of the command.
+ prevent the editor from closing on unsaved files
+ make a shortcut that duplicates a line Ctrl+d
+ make a shortcut that deleltes a line Ctrl+D / Alt+d? if cpp-terminal manages to support that,
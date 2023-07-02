Issues
------
+ Figure out why the editor crashes, if the screen width is too small
+ Figure out why sometimes random letters are printed on the top right of the screen

TODOs
-----
+ When the cursor is moveing up and down it should stay at the current colum and not jump fom end 
	to end
+ For objects with the TextTrait, change the methods so that they output a string that only changes
	the view characters on screen that actually got changed.
+ make the tilde'~' characters after the file lime green.
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
Issues
------
+ Sometimes when resizeing the console (larger), characters appear at the very right of the screen
+ when the console will get resized to small it crashes
	probably the text editor and margins that maybe turn into negative numbers and maybe loops that then 
	cause an segmentation fault

TODOs
-----
+ implement line cursors for screen boundaries and margin boundaries for automatic scrowling
	of the rendered screen. 
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
+ an autosave or place to store backups of unsaved files. That can be later recovered
+ make themes and the gui elements should be able to take a theme on their construction and or set
	one later that they then use to color the text forground / background + syntax highlighting.
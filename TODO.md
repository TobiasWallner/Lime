Issues
------
+ If the cursor is at the end of the line the following line dissappears. 
	probably a rendering error.
+ Ctrl+E does not move the cursor to the end of the file, only the line number advances.
+ saveing a file with Ctrl+S does not remove the save indicator '*' in front of the 
	filename.
+ moveing backwards at the end of a line does not move the cursor to the end of the 
	previous line.
+ moveing the cursor  
+ Change movement with to the top and end of the file to top(alt+t) and end(alt+e)
	that makes it more consistent and then 
+ Moving the cursor to the end of the screen makes the cursor dissappear
+ Sometimes when resizeing the console (larger), characters appear at the very right of the screen

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
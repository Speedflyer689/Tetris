#include <stdio.h>


/* Esc[2JEsc[1;1H             - Clear screen and move cursor to 1,1 (upper left) pos. */
#define clrscr()              puts ("\e[2J\e[1;1H")
	
/* Esc[K                      - Erases from the current cursor position to the end of the current line. */
#define clreol()              puts ("\e[K")
	
/* Esc[2K                     - Erases the entire current line. */
#define delline()             puts ("\e[2K")
	
/* Esc[Line;ColumnH           - Moves the cursor to the specified position (coordinates) */
#define gotoxy(x,y)           printf("\e[%d;%dH", y, x)
	
/* Esc[?25l (lower case L)    - Hide Cursor */
#define hidecursor()          puts ("\e[?25l")
	
/* Esc[?25h (lower case H)    - Show Cursor */
#define showcursor()          puts ("\e[?25h")



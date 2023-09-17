#ifndef TETRIS_HEADER
#define TETRIS_HEADER

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





    #define B_COLS 12
    #define B_ROWS 23
    #define B_SIZE (B_COLS * B_ROWS)

    #define coordinate(x, y) (y * B_COLS + x)

    #define TL     -B_COLS-1	/* top left */
    #define TC     -B_COLS		/* top center */
    #define TR     -B_COLS+1	/* top right */
    #define ML     -1		    /* middle left */
    #define MR     1		    /* middle right */
    #define BL     B_COLS-1		/* bottom left */
    #define BC     B_COLS		/* bottom center */
    #define BR     B_COLS+1		/* bottom right */


    #define N 7


    #ifdef __linux__
        int getch();
        int kbhit(void);
        char get_char( int seconds );
    #endif
    typedef int TETROMINO;

    extern TETROMINO TETROMINOS[N][4][3]; 

    
    
    int update(int *board, int* shadow, int gravity);
    int collision(int* board, TETROMINO* tetromino, int x, int y);
    void place(int* board, TETROMINO* tetromino, int x, int y, int val);
    int lines_filled(int *board);
    int clearline(int *board, int* shadow);


#endif
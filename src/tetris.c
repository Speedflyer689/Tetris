#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "tetris.h"

  #ifdef __linux__
  	#include <termios.h>
	#include <fcntl.h>

	/*getch() for linux*/

    int getch(void)
	{
	   	struct termios oldattr, newattr;
	   	int ch;
	   	tcgetattr(STDIN_FILENO, &oldattr);
	   	newattr=oldattr;
	   	newattr.c_lflag &= ~ECHO;
	   	tcsetattr( 0, TCSANOW, &newattr);
   		ch=getchar();
   		tcsetattr(0, TCSANOW, &oldattr);
   		return(ch);
	}

        int kbhit(void)
        {
          struct termios oldt, newt;
          int ch;
          int oldf;

          tcgetattr(STDIN_FILENO, &oldt);
          newt = oldt;
          newt.c_lflag &= ~(ICANON | ECHO);
          tcsetattr(STDIN_FILENO, TCSANOW, &newt);
          oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
          fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

          ch = getchar();

          tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
          fcntl(STDIN_FILENO, F_SETFL, oldf);

          if(ch != EOF)
          {
            ungetc(ch, stdin);
            return 1;
          }

          return 0;
        }

	char get_char( int seconds ){
		struct termios oldattr, newattr;
	   	
	   	tcgetattr(STDIN_FILENO, &oldattr);
	   	newattr=oldattr;
	   	newattr.c_lflag &= ~ECHO;
	   	tcsetattr( 0, TCSANOW, &newattr);
   		
    char c = '_'; //default return
    while( seconds != 0 ) {
        if( kbhit() ) { //if there is a key in keyboard buffer
			c=getchar();
   		tcsetattr(0, TCSANOW, &oldattr); //get the char
            break; //we got char! No need to wait anymore...
        }

        usleep(10000); //one second sleep
        --seconds; //countdown a second
    }
    return c;
}



  #elif _WIN32
    #include <conio.h>
	#include <windows.h>

	char get_char( int seconds ){
    char c = '_'; //default return
    while( seconds != 0 ) {
        if( _kbhit() ) { //if there is a key in keyboard buffer
            c = _getch(); //get the char
            break; //we got char! No need to wait anymore...
        }

        Sleep(1000); //one second sleep
        --seconds; //countdown a second
    }
    return c;
}
  #else 
  	#error "OS not supported!"
  #endif

TETROMINO TETROMINOS[N][4][3] = {
	{	
		{TC, ML, MR}, /*T 0   DEGREE*/

		{TC, MR, BC}, /*T  90  DEGREE*/

		{ML, MR, BC}, /*T 180  DEGREE*/

		{TC, ML, BC}  /*T 270 DEGREE*/
	},

	{	
		{TC, BC, 2 * B_COLS}, /*I 0   DEGREE*/

		{ML, MR, 2},          /*I  90  DEGREE*/

		{TC, BC, 2 * B_COLS}, /*I 180  DEGREE*/

		{ML, MR, 2}          /*I 270 DEGREE*/
	},

	{
		{ML, BC, BL},	// 0

		{ML, BC, BL},
		
		{ML, BC, BL},
		
		{ML, BC, BL}
	},

	{	
		{TC, BC, BR},	/*L 0   DEGREE*/

		{BL, ML, MR},	/*L 90   DEGREE*/

		{BC, TC, TL},	/*L 180   DEGREE*/

		{ML, MR, TR}	/*L 270 DEGREE*/
	},

	{
		{ML, BC, BR}, //z
		
		{TC, ML, BL},
		
		{ML, BC, BR},

		{TC, ML, BL}
	},

	{	
		{TC, BC, BL},	// L flipped

		{TL, ML, MR},	

		{BC, TC, TR},	

		{ML, MR, BR}	
	},

	{
		{MR, BC, BL}, //z flipped
		
		{TC, MR, BR},
		
		{ML, BC, BR},

		{TC, ML, BL}
	}

};

int G = 50;

int update(int *board, int* shadow, int gravity)
{
	//clrscr();
    int x, y;
    
	G = gravity;
    
	
    for (y = 1; y < B_ROWS - 1; y++)
	{
		for (x = 0; x < B_COLS; x++)
		{
			if (board[y * B_COLS + x] - shadow[y * B_COLS + x])
			{
				shadow[y * B_COLS + x] = board[y * B_COLS + x];
				gotoxy(x * 2 + 28, y);
				printf("\e[%dm  ", board[y * B_COLS + x]);
			}
		}
	}
	return get_char(gravity);
}


int collision(int* board, TETROMINO* tetromino, int x, int y)
{
	int pos = coordinate(x, y);
	if (board[pos] || board[pos + tetromino[0]] || board[pos + tetromino[1]] || board[pos + tetromino[2]])
	{
		return 1;
	}
	return 0;
}

void place(int* board, TETROMINO* tetromino, int x, int y, int val)
{
	int pos = coordinate(x, y);
	board[pos] = val;
	board[tetromino[0] + pos] = val;
	board[tetromino[1] + pos] = val;
	board[tetromino[2] + pos] = val;
}

int lines_filled(int *board)
{
	int y;
	for (y = 1; y < B_ROWS - 2; y++)
	{
		for (int x = 1; x < B_COLS; x++)
		{
			if (board[y * B_COLS + x] != 46)
			{
				break;
			}
			else if (x == B_COLS - 1)
			{
				return y;
			}
		}
	}
	return 0;
}

int clearline(int *board, int *shadow)
{	int linescleard = 0;
	hidecursor();
	int y = lines_filled(board);
	while (y != 0)
	{
		for (int x = 1; x < (B_COLS + 1) / 2; x++)
		{	
			//usleep(10000);
			board[coordinate(x, y)] = 42;
			board[coordinate(B_COLS - x - 1, y)] = 42;

			update(board, shadow, G);
		}
		for(int j = y; j >= 1; j--)
		{
			for (int i = 1; i < B_COLS - 1; i++)
			{
				board[j * B_COLS + i] = board[(j - 1) * B_COLS + i];
				if (j == 1)
				{
					board[j * B_COLS + i] = 0;
				}
				
			}
		}
		
		linescleard++;
		y = lines_filled(board);
	}
	return linescleard;
}

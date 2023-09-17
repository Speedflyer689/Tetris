#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#include "tetris.h"
#include "conio.h"


#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define QUIT '\e'
#define PAUSE 'p'

int COLOR = 46, BLANK = 7;
int board[B_SIZE], shadow[B_SIZE];



int main()
{
	hidecursor();
    int *ptr = board, i = 0;
	int rotation = 0;
	for (i = B_SIZE; i; i--)
	{
		*ptr++ = i < 25 || i % B_COLS < 2 ? COLOR : 0;
	}

	int linescleared = 0;
	int score = 0;
	int gravity = 50;
	
	srand(time(NULL));
	
	clrscr();
	//cbreak();
	//noecho();
	
	int key = '\0';
	
	while (1)
	{
		int j = rand() % N;
		int posY = 1;
		int posX = 5;
		int pos = B_COLS + 5;
		int rot = 0;
		key = '\0';

		int gameover = 1;
		
		while (!collision(board, TETROMINOS[j][rot], posX, posY + 1))
		{
			gameover = 0;
			//posY++;
			
			if (key == '_' || key == DOWN)
			{
				posY++;
			}
			else if (key == RIGHT)
			{
				//posY--;
				if (!collision(board, TETROMINOS[j][rot], posX + 1, posY))
				{
					posX++;
					
				}
				
			}
			else if (key == LEFT)
			{
				//posY--;
				if (!collision(board, TETROMINOS[j][rot], posX - 1, posY))
				{
					posX--;
				}
				
			}
			else if (key == UP)
			{
				if (!collision(board, TETROMINOS[j][(rot + 1) % 4], posX, posY))
				{
					rot = (rot + 1) % 4;
				}
				
			}
			else if (key == ' ')
			{
				while (!collision(board, TETROMINOS[j][rot], posX, posY + 1))
				{
					posY++;
				}
			}
			else if (key == QUIT)
			{
				showcursor();
				break;
			}
			else if (key == PAUSE)
			{
				key = getch();
			}
			
			
			
			
			
			fflush(stdout);
			
			place(board, TETROMINOS[j][rot], posX, posY, COLOR);

			key = update(board, shadow, gravity);
			

			place(board, TETROMINOS[j][rot], posX, posY, 0);
		}
		
		if (gameover == 1)
		{
			gotoxy(B_COLS + 10, B_ROWS + 5);
			printf("GAME OVER \n");
			break;
		}
		

		if (key == QUIT)
		{	
			break;
		}
		
		place(board, TETROMINOS[j][rot], posX, posY, COLOR);
		score++;

		if (lines_filled(board) != 0)
		{
			linescleared += clearline(board, shadow);
			gotoxy(B_COLS * 5, 7);

			printf("Lines Cleared: %d", linescleared);

			
			gotoxy(B_COLS * 5, 8);
			printf("Score: %d", score);
		}
	
	}
	printf("\n\n\n");
	
	showcursor();
}

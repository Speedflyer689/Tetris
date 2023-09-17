CC=gcc

all: main

main: main.o tetris.o
	$(CC) main.o tetris.o -o main.out

main.o: main.c
	$(CC) -c main.c
tetris.o: tetris.c
	$(CC) -c tetris.c

clean:
	rm *.o

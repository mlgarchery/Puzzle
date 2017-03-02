
CC= gcc -std=c99 -Wall -Wextra
INCLUDE= `sdl2-config --cflags`
LIBS= `sdl2-config --libs` -lm

all: main.o fonctionPuzzle.o
	$(CC) -o puzzle main.o fonctionPuzzle.o $(LIBS)
	make clean

main.o: main.c fonctionPuzzle.h
	$(CC) $(INCLUDE) -c main.c

fonctionPuzzle.o : fonctionPuzzle.h fonctionPuzzle.c
	$(CC) $(INCLUDE) -c fonctionPuzzle.c 

clean: .
	rm *.o

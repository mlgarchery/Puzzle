
CC= gcc -std=c99 -Wall -Wextra
INCLUDE= `sdl2-config --cflags`
LIBS= `sdl2-config --libs` -lm

all: main.o
	$(CC) -o puzzle main.o $(LIBS)
	make clean

main.o: main.c
	$(CC) $(INCLUDE) -c main.c

sprite.o: sprite.h sprite.c
	$(CC) $(INCLUDE) -c sprite.c

clean: .
	rm *.o

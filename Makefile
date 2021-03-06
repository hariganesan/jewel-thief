# Hari Ganesan 1/12/13
# Makefile for jewel-thief

# compiler options
CC=g++
CFLAGS=-pedantic -Wall -I/sw/include -L/sw/lib
SDL=-lSDLmain -lSDL -lSDL_ttf
COCOA=-framework Cocoa
GL=-framework OpenGL -L/sw/lib

# build main and remove .o files
all: main clean

main: main.o Grid.o
	$(CC) -o $@ $(COCOA) $(GL) $(SDL) $^

main.o: main.cpp
	$(CC) $(CFLAGS) -c $^

Grid.o: Grid.cpp
	$(CC) $(CFLAGS) -c $^

clean:
	rm *.o

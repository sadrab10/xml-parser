CC=gcc
CFLAGS=-I.

all: main.o mxml-file.o mxml-node.o

main.o: main.c mxml.h 
	$(CC) -c main.c mxml.h config.h

mxml-file.o: mxml-file.c mxml-private.h
	$(CC) -c mxml-file.c

mxml-node.o: mxml-node.c mxml.h
	$(CC) -c mxml-node.c mxml.h config.h 

programme: main.o mxml-file.o mxml-node.o
	gcc -o programme main.o mxml-file.o mxml-node.o -I.



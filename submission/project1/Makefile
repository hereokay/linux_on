CC=gcc
CFLAGS=-O

all: smallsh

main.o :
	gcc -c main.c

smallsh.o :
	gcc -c smallsh.c

smallsh : main.o smallsh.o smallsh.h
	gcc -o smallsh main.o smallsh.o smallsh.h
clean:
	rm -f *.o smallsh

CC=gcc
CFLAGS=-O

all: pj

main.o :
	gcc -c main.c

smallsh.o :
	gcc -c smallsh.c

pj : main.o smallsh.o smallsh.h
	gcc -o pj main.o smallsh.o smallsh.h
clean:
	rm -f main.o smallsh.o pj

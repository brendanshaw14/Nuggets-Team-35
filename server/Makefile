# Makefile for server
# Author: Ming Cheng, Feb 2023

# Compiler and flags
CC = gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I ../support -L ../support -lsupport -I ../libcs50 -I. -L../libcs50 -lcs50-given
PROG = server

all: $(PROG)

# Object files
OBJS = server.o ../player/player.o ../grid/grid.o 
LIBS = ../support/libsupport.a ../libcs50/libcs50-given.a

test: server 
	./server ../maps/small.txt

valgrind: server
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./server ../maps/small.txt

server: $(OBJS) $(LIBS) 
	$(CC) $(CFLAGS) $^ -o $@ -lm

%.o: %.c $(LIBS)
	$(CC) $(CFLAGS) -c $< -o $@ -lm

clean:
	rm -f $(PROG) $(OBJS)
	rm -f vgcore.*
# Makefile for grid module and test program
# Author: Brendan Shaw, February 2023

# Compiler and flags
CC = gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -lm -I../suppport -I../libcs50 -I. -L../libcs50 -lcs50-given
PROG = gridtest
LIBS = ../libcs50/libcs50-given.a
# Source files
SRCS = grid.c ../player/player.c gridtest.c

# Object files
OBJS = grid.o ../player/player.o gridtest.o ../support/message.o ../support/log.o

.PHONY: all clean

all: $(PROG)

$(PROG): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

%.o: %.c $(LIBS) ../player/player.h grid.h 
	$(CC) $(CFLAGS) -c $< -o $@ -lm

test: 
	./gridtest ../maps/main.txt

clean:
	rm -f $(PROG) $(OBJS)




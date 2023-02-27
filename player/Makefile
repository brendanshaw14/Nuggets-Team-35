# Makefile for grid module and test program
# Author: Brendan Shaw, February 2023

# Compiler and flags
CC = gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I../support -I../libcs50 -I. -L../libcs50 -lcs50-given 
PROG = playertest
LIBS = ../libcs50/libcs50-given.a
# Source files
SRCS = player.c playertest.c

# Object files
OBJS = player.o playertest.o

.PHONY: all clean

all: $(PROG)

$(PROG): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

%.o: %.c $(LIBS) player.h 
	$(CC) $(CFLAGS) -c $< -o $@ -lm

test: 
	./playertest ../maps/main.txt

clean:
	rm -f $(PROG) $(OBJS)
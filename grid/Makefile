# Makefile for grid module and test program
# Author: Brendan Shaw, February 2023

# Compiler and flags
CC = gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I../suppprt -I../libcs50 -I. -L../libcs50 -lcs50-given 
PROG = gridtest
LIBS = ../libcs50/libcs50-given.a
# Source files
SRCS = grid.c gridtest.c

# Object files
OBJS = grid.o gridtest.o

.PHONY: all clean

all: $(PROG)

$(PROG): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(LIBS) grid.h 
	$(CC) $(CFLAGS) -c $< -o $@

test: 
	./gridtest ../maps/main.txt

clean:
	rm -f $(PROG) $(OBJS)




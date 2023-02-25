# Makefile for grid module and test program
# Author: Brendan Shaw, February 2023

# Compiler and flags
CC = gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I../suppprt -I. -L../libcs50 -lcs50-given
PROG = gridtest

# Source files
SRCS = grid.c gridtest.c

# Object files
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c grid.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(PROG) $(OBJS)




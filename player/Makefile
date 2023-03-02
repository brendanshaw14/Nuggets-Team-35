# Makefile for player module and test program
# Author: Brendan Shaw, February 2023

# Compiler and flags
CC = gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I../support -L../support -lsupport -I../libcs50 -I. -L../libcs50 -lcs50-given 
PROG = playertest
LIBS = ../libcs50/libcs50-given.a ../support/support.a
# Source files
SRCS = player.c playertest.c

# Object files
OBJS = player.o playertest.o ../grid/grid.o ../support/message.o ../support/log.o

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
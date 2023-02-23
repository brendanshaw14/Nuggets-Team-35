# Makefile for server
# Author: Ming Cheng, Feb 2023

# Compiler and flags
CC = gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I ../suppprt -L ../support -lsupport 
PROG = server

all: $(PROG)

server: server.c libsupport.a
	$(CC) -o $@ $< $(CFLAGS)

libsupport.a:
	cd ../support; make


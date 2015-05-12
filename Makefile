# Makefile for 'maybe'

CC=gcc
CFLAGS=-O -g -Wall
DEST=/usr/local/bin

OBJS=maybe.o

all: maybe

maybe: $(OBJS)
	$(CC) -o maybe $(OBJS)

maybe.o: 	maybe.c

clean:
	-rm -f *.o core maybe *~ \#*

distclean: clean


install: maybe
	cp maybe $(DEST)


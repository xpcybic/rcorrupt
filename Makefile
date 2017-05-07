CC = gcc
CFLAGS = -Wall
DEBUG = -g

all: rcorrupt

rcorrupt: rcorrupt.c rcorrupt.h
	$(CC) $(CFLAGS) -o $@ $<

debug: rcorrupt.c rcorrupt.h
	$(CC) $(CFLAGS) $(DEBUG) -o rcorrupt $<

.PHONY: clean

clean:
	rm -f rcorrupt

CC = gcc
CFLAGS = -Wall -O2
DEBUG = -g
PROF = -pg

all: rcorrupt

rcorrupt: rcorrupt.c rcorrupt.h
	$(CC) $(CFLAGS) -o $@ $<

debug: rcorrupt.c rcorrupt.h
	$(CC) $(CFLAGS) $(DEBUG) -o rcorrupt $<

prof: rcorrupt.c rcorrupt.h
	$(CC) $(CFLAGS) $(PROF) -o rcorrupt $<

.PHONY: clean

clean:
	rm -f rcorrupt gmon.out

CC=gcc
CFLAGS=-pedantic -Wall
CFILES=src/index.c src/search.c src/main.c


all:
	$(CC) $(CFILES) $(CFLAGS) -o search

debug:
	$(CC) -g $(CFILES)  $(CFLAGS) -o search

clean:
	rm search

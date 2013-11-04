CC=gcc
CFLAGS=-pedantic -Wall
CFILES=src/index.c src/search.c src/main.c


all: index search

search:
	$(CC) $(CFILES) $(CFLAGS) -o search

index:
	$(CC) $(CFLAGS) src/index.c src/indexer.c -o index

searchdebug:
	$(CC) -g $(CFILES) $(CFLAGS) -o search

indexdebug:
	$(CC) -g $(CFLAGS) src/index.c src/indexer.c -o index

debug: indexdebug searchdebug

clean:
	rm search index

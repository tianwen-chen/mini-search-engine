# Makefile for crawler.c
# Tianwen Chen, 2/1/2023

OBJS = crawler.o
LIBS = -lm
L = ../libcs50
C = ../common
LLIBS = $L/libcs50-given.a $C/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
MAKE = make

crawler: $(OBJS) $(LLIBS)
		$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

# primary test on letters and toscrape (with valgrind)
test: testing.sh crawler
	bash -v ./testing.sh >& testing.out

# larger test file on wikipedia (with valgrind)
testwiki: testwiki.sh crawler
	bash -v ./testwiki.sh >& testwiki.out

# object files depend on include files
crawler.o: $L/bag.h $L/hashtable.h $L/mem.h $L/webpage.h $C/pagedir.h

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core

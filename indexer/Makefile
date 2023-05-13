# Makefile for indexer in Lab5
# Tianwen Chen, 2/1/2023

OBJ1 = indexer.o
OBJ2 = indextest.o
LIBS = -lm
L = ../libcs50
C = ../common
LLIBS = $L/libcs50-given.a $C/common.a
MYPROG = indexer indextest

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

indexer: $(OBJ1) $(LLIBS)
		$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

indextest: $(OBJ2) $(LLIBS)
		$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

test1: indexer
		$(VALGRIND) ./indexer ../data/letters10 ../data/letters10/letters10.index

test2: indextest
		$(VALGRIND) ./indextest ../data/letters10/letters10.index ../data/letters10/test.out

testcmp: testing.sh
	bash ./testing.sh >& testing.out

all: $(MYPROG)

# object files depend on include files
indexer.o: $L/hashtable.h $L/mem.h $L/webpage.h $C/pagedir.h $C/index.h $C/word.h
indextest.o: $L/hashtable.h $L/mem.h $L/webpage.h $C/pagedir.h $C/index.h $C/word.h

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core

# Makefile for querier in Lab6
# Tianwen Chen, Feb 2023

OBJ1 = querier.o
OBJ2 = fuzzquery.o
LIBS = -lm
L = ../libcs50
C = ../common
LLIBS = $C/common.a $L/libcs50-given.a 
MYPROG = querier fuzzquery

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

querier: $(OBJ1) $(LLIBS)
		$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

fuzzquery: $(OBJ2) $(LLIBS)
		$(CC) $(CFLAGS) $^ $(LIBS) -o $@


test: testing.sh
	bash ./testing.sh >& testing.out

all: $(MYPROG)

# object files depend on include files
querier.o: $L/counters.h $L/file.h $L/mem.h $L/webpage.h $C/pagedir.h $C/index.h $C/word.h
fuzzquery.o: $L/mem.h $L/file.h

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
 
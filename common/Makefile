# Makefile for common
#   Builds and compiles all these utility functions into a library.
#
# David Kotz, April 2016, July 2017, April 2019, 2021

# object files, and the target library
OBJS = pagedir.o index.o word.o
L = ../libcs50
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L
CC = gcc
MAKE = make

# Build $(LIB) by archiving object files
# (you will need to drop in copy of set.c, counters.c, hashtable.c)
$(LIB): $(OBJS) -lm
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h $L/webpage.h $L/mem.h
index.o: index.h $L/hashtable.h $L/mem.h

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
# (this rule is used only by the Professor in preparing the starter kit)
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o

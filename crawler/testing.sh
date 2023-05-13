#!/bin/bash
# testing file for crawler.c
# for Lab4 specifically
# Tianwen Chen, Jan 2023

# make output directory
mkdir ../data

#################################### primary test ############################################

mkdir ../data/letters0
mkdir ../data/letters1
mkdir ../data/letters2
mkdir ../data/letters10

# test handling invalid argument
./crawler 
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters

# test with valid input, letters with depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters0 0

# test with valid input, letters with depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters1 1

# test with valid input, letters with depth 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters2 2

# test with valid input, letters with depth 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters10 10

#################################### test on a larger playground with valgrind ######################################
############################# toscrape ###########################
mkdir ../data/toscrape0
mkdir ../data/toscrape1
mkdir ../data/toscrape2

# toscrape with depth 0
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape0 0

# toscrape with depth 1
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape1 1

# toscrape with depth 2
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape2 2



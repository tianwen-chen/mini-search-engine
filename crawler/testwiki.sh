#!/bin/bash
# testing file for crawler.c on wiki
# for Lab4 specifically
# Tianwen Chen, Jan 2023

# make output directory
mkdir ../data/wiki0
mkdir ../data/wiki1

# test with valgrind, depth 0
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wiki0 0

# test with valgrind, depth 1
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wiki1 1

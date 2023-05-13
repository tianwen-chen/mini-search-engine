#!/bin/bash
# testing.sh for querier in lab6
# Tianwen Chen, Feb 3

pdir1="../data/toscrape-2"
indx1="../data/toscrape-2.index"
val="valgrind --leak-check=full --show-leak-kinds=all"

echo ............fuzztest for toscrape-2
./fuzzquery $indx1 5 0 | ./querier $pdir1 $indx1
echo


pdir2="../data/wiki1"
indx2="../data/wiki1/wiki1.index"

echo ............fuzztest for wiki1
./fuzzquery $indx2 5 0 | ./querier $pdir2 $indx2
echo

echo ............valgrind test for toscrape-2
./fuzzquery $indx1 1 0 | $val ./querier $pdir1 $indx1
echo

echo ............valgrind test for wiki1
./fuzzquery $indx2 1 0 | $val ./querier $pdir2 $indx2
echo

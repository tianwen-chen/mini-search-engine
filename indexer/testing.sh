#!/bin/bash
# testing.sh for indexer in Lab5
# Tianwen Chen, Feb 2023

echo ...test on letters10, letters with depth 10
./indexer ../data/letters10 ../data/letters10/letters10.index
./indextest ../data/letters10/letters10.index ../data/letters10/test.out
~/cs50-dev/shared/tse/indexcmp ../data/letters10/letters10.index ../data/letters10/test.out

echo ...passed test

echo ...test on toscrape0, toscrape with depth 1
./indexer ../data/toscrape0 ../data/toscrape0/toscrape.index
./indextest ../data/toscrape0/toscrape.index ../data/toscrape0/test.out
~/cs50-dev/shared/tse/indexcmp ../data/toscrape0/toscrape.index ../data/toscrape0/test.out

echo ...passed test

echo ...test on toscrape2, toscrape with depth 2
./indexer ../data/toscrape2 ../data/toscrape2/toscrape.index
./indextest ../data/toscrape2/toscrape.index ../data/toscrape2/test.out
~/cs50-dev/shared/tse/indexcmp ../data/toscrape2/toscrape.index ../data/toscrape2/test.out

echo ...passed test

echo ...test on wiki0, wiki with depth 0
./indexer ../data/wiki0 ../data/wiki0/wiki0.index
./indextest ../data/wiki0/wiki0.index ../data/wiki0/test.out
~/cs50-dev/shared/tse/indexcmp ../data/wiki0/wiki0.index ../data/wiki0/test.out

echo ...passed test

echo ...test on wiki1, wiki with depth 1
./indexer ../data/wiki1 ../data/wiki1/wiki1.index
./indextest ../data/wiki1/wiki1.index ../data/wiki1/test.out
~/cs50-dev/shared/tse/indexcmp ../data/wiki1/wiki1.index ../data/wiki1/test.out

echo ...passed test

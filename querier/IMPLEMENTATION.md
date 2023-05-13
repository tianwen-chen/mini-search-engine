# Implementation Spec for Querier in TSE, Lab 6
## CS50 TSE Querier

In this document we focus on the implementation choices of Querier module.

This file includes:
1) pseudo code for major functions
2) test plan

### function def
```c
static int parse_input(char* input, char** words);
static int validate_input(char** words, int wordCount, char** newWords);
static void implement_logic(index_t* index, char** words, int wordCount, counters_t* result);
static counters_t* count_intersect(counters_t* c1, counters_t* c2);
static counters_t* count_union(counters_t* c1, counters_t* c2);
static void index_load(char* indexFilename, index_t* index);
static void result_rank(counters_t* result, FILE* fp, char* pageDirectory);
```

### main
check arguments
create an index object from indexFilename
ask for query
while the stdin is not null:
    read from stdin
    normalize input
    call parse_input
    call validate_input
    implement logic of the input
    rank and print the result
    handle the memory

### parse_input
if input is not NULL,
    keep two pointers p1, p2 on the input
    while p2 does not reach the end of string
        check if p2 is pointing to a word
            if yes, keep incrementing p2
            until p2 reach the end of the word
            store the current word
        else
            increment p2
return the number of words

### validate_input
keep track of whether there should be an adjunct
while we do not check all words
    if we don't need adjunct
        check if the current word is adjunct
            if yes, return 0
            else, reset boolean
    else
        if there is adjunct
            reset adjunct
        else
            add AND to new word list
    next word
return the number of words in the new word list

### implement_logic
while we do not reach the end of word list
    add current word to product
    while the next adjunct is AND
        keep adding next word to product
    add product to result
    next word

### index_load
open index file, handle error
read everything line by line:
    create a new counter
    keep scan the docID and occurance count, save to counter
    save counter to index, key is the word and item is the counter

### result_rank
count the number of matches in the result, n
loop for n times
    iterate through every match to find the highest score
    load the webpage from the match
    print the match
    reset the score for the match

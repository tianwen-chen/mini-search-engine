# CS50 TSE Indexer
## Implementaion Spec

In this document we focus on the implementation choices on Indexer module. We will discuss:
1) Data structures
2) Pseudo code for over all flow and for each of the functions
3) Detailed function prototypes and their parameters
4) Testing plan

## Data structures
For index, we write a wrapper data structure type index.c, with the header file index.h. Index implements methods in hashtable.
The index object is a hashtable with word as key and counter as items. For each counter, we have docID as key and count of how many times the word appears in the document.

## Control flow
The indexer is implemented in indexer.c
Functions in indexer.c: main, indexBuild, indexPage, indexprint, counterprint, deletecounter

## Utility modules
```c
static index_t* indexBuild(char* pageDirectory);
static void indexPage(webpage_t* page, const int docID, index_t* index);
void indexprint(void* arg, const char* key, void* item);  //helper function when write to file, handle index
void counterprint(void* arg, const int key, const int count); // helper function handle counter
void deletecounter(void* counter);
```

### main
```c
    parse arugments, check if valid
    call indexBuild
    open output file to write and write index to file
```
### indexBuild
```c
    create a new index object
    laod a webpage from the document file
        if successful, pass the webpage and docID to index
    return pointer to index
```
### indexPage
```c
    steps through each word of the webpage,
    skips trivial words (less than length 3),
    normalizes the word (converts to lower case),
    looks up the word in the index,
        adding the word to the index if needed
    increments the count of occurrences of this word in this docID
```

### indexprint
```c
    call counters iterate and pass counterprint
```

### counterprint
```c
    print [docID, count], separated by blank space
```


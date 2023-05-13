/*
* querier.c for lab6 in TSE for CS50
* Tianwen Chen, Feb 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/counters.h"
#include "../libcs50/set.h"
#include "../libcs50/file.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"

/* declare utility module */
static int parse_input(char* input, char** words);
static int validate_input(char** words, int wordCount, char** newWords);
static void implement_logic(index_t* index, char** words, int wordCount, counters_t* result);
static counters_t* count_intersect(counters_t* c1, counters_t* c2);
static counters_t* count_union(counters_t* c1, counters_t* c2);
static void index_load(char* indexFilename, index_t* index);
static void result_rank(counters_t* result, FILE* fp, char* pageDirectory);
static void deletecounter(void* counter);
static void itemcount(void* arg, const int key, int count);

/************************************ main ********************************/
int main(const int argc, char* argv[]){
    // check arguments
    if(argc != 3){
        fprintf(stderr, "invalid number of arguments\n");
        return 1;
    }
    //validate page directory and index filename
    char* pageDirectory = argv[1];
    char* indexFilename = argv[2];
    FILE* fp_read = fopen(indexFilename, "r");  //open file to read
    if(!pagedir_validate(pageDirectory) || fp_read == NULL){
        fprintf(stderr, "failed to validate arguments\n");
        return 1;
    }
    // load index page from indexFile
    int NUM_SLOT = 500;
    index_t* index = index_new(NUM_SLOT);
    index_load(indexFilename, index);
    if(index == NULL){
        fclose(fp_read);
        return 1;
    }
    fclose(fp_read);
    // ask for stdin
    printf("Query? \t");
    char* input;
    while((input = file_readLine(stdin)) != NULL){
        //normalize
        char* normal = normalizeWord(input);
        // call parseInput
        int len = strlen(input);
        char** words = mem_malloc(sizeof(char**)*len);
        int wordCount = parse_input(normal, words);
        // handle error
        if(wordCount == 0){
            return 1;
        }
        // validate the format of input
        char** newWords = mem_malloc(sizeof(char**)*len);
        int validate = validate_input(words, wordCount, newWords);
        if(validate==0){
            fprintf(stderr, "invalid grammar\n");
            return 1;
        }
        // if validated, search for the webpage according to grammar
        counters_t* result = counters_new();
        implement_logic(index, newWords, validate, result);
        // sort & build the final output
        result_rank(result, stdout, pageDirectory);
        //exit(0);
        // free all the memories
        mem_free(words);
        mem_free(newWords);
        mem_free(input);
        mem_free(normal);
        //exit(0);
        counters_delete(result);
    }
    //exit(0);
    index_delete(index, deletecounter);
    return 0;
}


/************************************** parse_input ******************************/
/* read input and tokenize it to pointer to words, return an array of pointers to each word*/
/* assumption: input is not NULL*/
static int parse_input(char* input, char** words){
    // handle error
    if(input == NULL){
        fprintf(stderr, "input cannot be null\n");
        return false;
    }
    char* p1;            // 1st pointer to the current word
    int p2 = 0;         // 2nd pointer to the current location in input
    int count = 0;        //counter of the position in array words
    int len = strlen(input);
    while(p2 < len){
        if(isalpha(input[p2])){
            // if meet the current character
            p1 = &input[p2];      // store the word
            while(!isspace(input[p2]) && p2 < len){
                // while the word lasts
                p2 += 1;
            }
            // end while, input[p2] is the end of word, make it null
            input[p2] = '\0';
            // normalize the word and store p1 into words
            words[count] = p1;
            count += 1;
            p2 += 1;
        }else{
            p2 += 1;
        }
    }
    return count;
}

/************************************** validate_input ********************/
static int validate_input(char** words, int wordCount, char** newWords){
    int i = 0;
    int j = 0;          // counter of the position in newWords
    char* currWord;
    bool adjunct = false;   // there should be no an/or in the beginning
    while(i < wordCount){
        currWord = words[i] ;
        if(!adjunct){
            // when we don't need adjunct
            if(strcmp(currWord, "and")==0 || strcmp(currWord,"or")==0){
                return 0;
            }else{
                // else, set the adjunct to true
                adjunct = true;
                // put currWord into newWords
                newWords[j] = currWord;
            }
        }else{            
            // when there can be an adjunct
            if(strcmp(currWord, "and")==0 || strcmp(currWord,"or")==0){
                // set the adjunct to be false
                adjunct = false;
                newWords[j] = currWord;
            }
            else{
                // else, there is an implicit AND, add the AND before adding currWord
                newWords[j] = "and";
                j += 1;
                newWords[j] = currWord;
            }
        }
        // increment counters
        i += 1;
        j += 1;
    }
    return j;
}


/************************************ implement_logic ********************/
static void implement_logic(index_t* index, char** words, int wordCount, counters_t* result){
    int i = 0;
    // result is sum
    char* currVar;
    char* nextVar;
    while(i < wordCount){
        currVar = words[i];
        // prod is viewed as an empty counter set (resetted every loop)
        counters_t* prod = index_find(index, currVar);
        while(i+1 < wordCount && strcmp(words[i+1], "and")==0){
            // peek in advance
            nextVar = words[i+2];
            // temp is viewed as an empty counter set (resetted every time)
            counters_t* temp = index_find(index, nextVar);
            prod = count_intersect(prod, temp);
            i += 2;     // proceed to the next variable
            // counters_delete(temp);
        }
        result = count_union(result, prod);
        i += 2;
        // counters_delete(prod);
    }
}

/*****************************count_intersect********************************/

static int min(int i, int j){
    if(i <= j){
        return i;
    }else{
        return j;
    }
}

static void intersect_helper(void* arg, const int key, int count){
    // key and count are in c2 (the new counters)
    counters_t* prod = arg;
    int oldScore = counters_get(prod, key);
    if(oldScore != 0){
        //if key in prod, update the score
        int newScore = min(oldScore, count);
        counters_set(prod, key, newScore);
    }
}

static counters_t* count_intersect(counters_t* c1, counters_t* c2){
    // c1 (product until now): docID, score
    // c2 (newly added docs upon searching word): docID, score
    // loop through all, call intersect_helper
    counters_iterate(c2, c1, intersect_helper);
    return c1;
}

/*******************************************count_union****************************/

static void union_helper(void* arg, const int key, int count){
    // key and count are in c2 (new counters)
    counters_t* sum = arg;
    int oldScore = counters_get(sum, key);
    int newScore = count + oldScore;
    counters_set(sum, key, newScore);
}

static counters_t* count_union(counters_t* c1, counters_t* c2){
    // c1 is sum (old), c2 is prod (new)
    counters_iterate(c2, c1, union_helper);
    return c1;
}

/************************** index_load ***************************/
static void index_load(char* indexFilename, index_t* index){
    //open the old file for reading
    FILE* fp1 = fopen(indexFilename, "r");
    if(fp1 == NULL){
        fprintf(stderr, "error opening file\n");
        return;
    }
    int d = file_numLines(fp1);
    while(d>0){
        //read everything line by line
        char* word = file_readWord(fp1);
        if(word == NULL){
            fprintf(stderr, "error reading word from file\n");
            return;
        }
        int docID;
        int occur;
        //create counter, set key value
        counters_t* counter = counters_new();
        while(fscanf(fp1, " %d %d", &docID, &occur)==2){
            bool res = counters_set(counter, docID, occur);
            if(!res){
                fprintf(stderr, "failed to set counter value\n");
                return;
            }
        }
        //save counter to the index
        bool saveres = index_save(index, word, counter);
        if(!saveres){
            fprintf(stderr, "failed to save counter to index\n");
            return;
        }
        d-=1;
        mem_free(word);
    }
    fclose(fp1);
}

/************************************* result_rank ***********************/
/* count the non-null key-count pair in the counter.
 */
static void itemcount(void* arg, const int key, int count)
{
  int* nitems = arg;

  if (nitems != NULL){
    (*nitems)+=1;
  }
}
/***************************local struct ****************/
typedef struct docscore{
    int docID;
    int score;
}docscore_t;

/*********************************** find_max **********************/
static void find_max(void* arg, const int key, const int count){
    docscore_t* curr_max = arg;
    if(count > curr_max->score){
        curr_max->score = count;
        curr_max->docID = key;
    }
}

/*
* sort and print out sorted result
*/
static void result_rank(counters_t* result, FILE* fp, char* pageDirectory){
    // first, get the number of total results
    int temp_count = 0;
    counters_iterate(result, &temp_count, itemcount);
    // print out the number of results
    if(fp == NULL){
        fprintf(stderr, "file pointer is null\n");
    }
    fprintf(fp, "Matches %d documents (ranked):\n", temp_count);
    // loop through all counternodes and print out the max
    docscore_t* curr_max = mem_malloc(sizeof(docscore_t*));
    curr_max->score = 0;
    //webpage_t* page;
    int maxScore;
    int maxArg;
    for(int i = 0; i < temp_count; i++){
        //loop through all key-item pairs and find&print the max
        counters_iterate(result, curr_max, find_max);
        maxScore = curr_max->score;
        maxArg = curr_max->docID;
        // get url
        webpage_t* page = pagedir_load(pageDirectory, maxArg);
        char* url = webpage_getURL(page);
        // print out the current max result and reset the counternode
        fprintf(fp, "score: %d; docID: %d: %s \n", maxScore, maxArg, url);
        counters_set(result, maxArg, 0);
        // reset curr_max
        curr_max->score = 0;
        webpage_delete(page);
    }
    mem_free(curr_max);
}

/***************************** deletecounter ********************/
//handle item as counter and delete it
static void deletecounter(void* counter){
    counters_delete(counter);
}

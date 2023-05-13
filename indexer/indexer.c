/*
* indexer.c module for lab5
* Tianwen Chen, Feb 2023
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/bag.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"

/* declare utility module */

/****************************** main *************************************/
int main(const int argc, char* argv[]){
    //validate arguments
    if(argc != 3){
        fprintf(stderr, "invalid number of arguments\n");
        return 1;
    }
    //validate parameters
    char* pageDriectory = argv[1];
    char* indexFilename = argv[2];
    if(! pagedir_validate(pageDriectory)){
        fprintf(stderr, "invalid page directory\n");
        return 1;
    }
    //call indexBuild
    index_t* index = indexBuild(pageDriectory);
    //after index built, write it to the file
    FILE* fp = fopen(indexFilename, "w");
    if(fp == NULL){
        fprintf(stderr, "cannot open file to write\n");
        return 1;
    }
    //write to file
    index_iterate(index, fp, indexprint);
    fclose(fp);
    //free memory
    index_delete(index, deletecounter);
    return 0;
}

/***************************** indexBuild ********************************/
static index_t* indexBuild(char* pageDirectory){
    //create a new 'index' object
    /* size of the hashtable ?? */
    const int NUM_SLOT = 50;
    index_t* indexp = index_new(NUM_SLOT);
    for(int i=1; ; i++){
        //load a webpage from document file pageDirectory/id
        webpage_t* page = pagedir_load(pageDirectory, i);
        if(page != NULL){
            //if successful, pass the webpage and docID to indexPage
            indexPage(page, i, indexp);
            webpage_delete(page);
        }
        else{
            break;
        }
    }
    return indexp;
}

/******************************* indexPage  ********************/
static void indexPage(webpage_t* page, const int docID, index_t* index){
    //step through each word of the webpage
    int pos = 0;
    char* currWord;
    while((currWord = webpage_getNextWord(page, &pos))!= NULL){
        //skip trivial words
        if(strlen(currWord)<3){
            mem_free(currWord);
            continue;
        }
        //normalize the word
        char* normal = normalizeWord(currWord);
        //looks up the word in the index
        counters_t* counter = index_find(index, normal);
        // if not found, add the word
        if(counter == NULL){
            counter = counters_new();
            bool res = index_save(index, normal, counter);
            if(! res){
                //on un unsuccessul index_save, return and print error
                fprintf(stderr, "failed to save counter to index\n");
                return;
            }
        }
        // increments the count of this word in this docID
        int res = counters_add(counter, docID);
        if(res==0){
            //failed to add
            fprintf(stderr, "failed to increment counter\n");
            return;
        }
        mem_free(normal);
        mem_free(currWord);
    }
}

/**************************** indexprint **************************/
// key is word, item is counter, print word and call counterprint 
void indexprint(void* fp, const char* key, void* item)
{
    counters_t* counter = item;
    if(counter == NULL){
        fprintf(fp, "%s (null)", key);
    }
    else{
        // print the word
        fprintf(fp, "%s ", key);
        counters_iterate(counter, fp, counterprint);
        // new line
        fprintf(fp, "\n");
    }
}

/****************************** counterprint ************************/
// print [docID, count], separated by blank space
void counterprint(void* fp, const int key, const int count){
    fprintf(fp, "%d %d ", key, count);
}

/***************************** deletecounter ********************/
//handle item as counter and delete it
void deletecounter(void* counter){
    counters_delete(counter);
}
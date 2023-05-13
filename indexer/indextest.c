/*
* indextest.c for Lab5
* takes an index output file and read index from it
* write out to a new output file
* Tianwen Chen, Feb 2023
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
void indexprint(void* arg, const char* key, void* item);  //helper function when write to file, handle index
void counterprint(void* arg, const int key, const int count); // helper function handle counter
void deletecounter(void* counter);

int main(const int argc, char* argv[]){
    //validate arguments
    if(argc != 3){
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }
    char* oldName = argv[1];
    char* newName = argv[2];
    //open the old file for reading
    FILE* fp1 = fopen(oldName, "r");
    if(fp1 == NULL){
        fprintf(stderr, "error opening file\n");
        return 1;
    }
    // load the content into an inverted-index data structure
    int NUM_SLOT = 500;
    index_t* index = index_new(NUM_SLOT);
    int d = file_numLines(fp1);
    while(d>0){
        //read everything line by line
        char* word = file_readWord(fp1);
        if(word == NULL){
            fprintf(stderr, "error reading word from file\n");
            return 1;
        }
        int docID;
        int occur;
        //create counter, set key value
        counters_t* counter = counters_new();
        while(fscanf(fp1, " %d %d", &docID, &occur)==2){
            bool res = counters_set(counter, docID, occur);
            if(!res){
                fprintf(stderr, "failed to set counter value\n");
                return 1;
            }
        }
        //save counter to the index
        bool saveres = index_save(index, word, counter);
        if(!saveres){
            fprintf(stderr, "failed to save counter to index\n");
            return 1;
        }
        d-=1;
        mem_free(word);
    }
    fclose(fp1);
    //write to newFile
    FILE* fp2 = fopen(newName, "w"); 
    if(fp2 == NULL){
        fprintf(stderr, "cannot open file to write\n");
        return 1;
    }
    //write to file
    index_iterate(index, fp2, indexprint);
    fclose(fp2);
    //free memory
    index_delete(index, deletecounter);
    return 0;
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
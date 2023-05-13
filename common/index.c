/*
* index.c module for index struct
* Lab5, CS50
* Tianwen Chen, Jan 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "mem.h"

/********************global types *********************/
typedef struct index{
    // index is just a hashtable of counters
    //key -> words, item -> counters (docID, count)
    hashtable_t* table;
}index_t;

/************************index_new ***********************/
index_t*
index_new(const int num_slots){
    index_t* indexp = mem_malloc(sizeof(index_t));
    if(indexp == NULL){
        return NULL;
    }
    hashtable_t* hasht  = hashtable_new(num_slots);
    if(hasht == NULL){
        mem_free(indexp);
        return NULL;
    }
    indexp->table = hasht;
    return indexp;
}

/*********************** index_save *****************/
bool
index_save(index_t* index, const char* key, void* item){
    bool res = hashtable_insert(index->table, key, item);
    return res;
}

/*********************index_delete *********************/
void
index_delete(index_t* index, void(*itemdelete)(void* item)){
    //call hashtable delete to delete
    hashtable_delete(index->table, itemdelete);
    //free index
    mem_free(index);
}

/*********************** index_find **********************/
void* 
index_find(index_t* index, const char* key){
    return hashtable_find(index->table, key);
}

/************************ index_iterate *********************/
void index_iterate(index_t* index, void* arg, 
                void (*itemfunc)(void* arg, const char* key, void* item)){
    hashtable_iterate(index->table, arg, itemfunc);
}
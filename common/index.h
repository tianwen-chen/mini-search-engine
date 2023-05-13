/*
* index.h - header file for index module
* a *index* is a hashtable of counters
* Tianwen Chen, Feb 2023
*/

#include <stdio.h>
#include <stdbool.h>

/********************* global types *****************/
typedef struct index index_t;

/***********************functions ************************/

/************************** index_new **********************/
/* Create a new (empty index)
* caller provides: 
*   numebr of slots to be used for the index, which is a hashtable
* we return:
*   pointer to a new index; return NULL is error
* caller is responsible for:
*   later calling index_delete
*/
index_t* index_new(const int num_slots);

/**************************index_save******************************/
/*
* save an item by a key into the index
* call provide:
* index pointer, key, item
* we return:
* true if saving successful, false it not
*/
bool index_save(index_t* index, const char* key, void* item);

/**************************index_delete***************/
void index_delete(index_t* index, void(*itemdelete)(void* item));

/*************************** index_find *********************/
/* return the item associated with the given key
*
* caller provides:
*   valid pointer to index, valid string for key
* we return:
*   pointer tot he item corresponding to the given key
*   NULL if index is NULL, key is NULL, or key is not found
* Notes: index is unchanged
*/
void* index_find(index_t* index, const char* key);

/************************index_iterate ********************/
void index_iterate(index_t* index, void* arg, void (*itemfunc)(void* arg, const char* key, void* item));
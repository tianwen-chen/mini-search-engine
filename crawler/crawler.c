/* 
* crawler.c for lab4
* for details see in README.md
* Tianwen Chen, Jan 2023
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/bag.h"
#include "../libcs50/mem.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"

/* declare all functions & modules */
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

int main(const int argc, char* argv[]){
    //check if the arguments are valid, there should be 4
    if (argc != 4){
        fprintf(stderr, "wrong arguments\n");
        return 1;
    }
    // create pointers and feed the arguments to the other functions
    int d;
    sscanf(argv[3], "%d", &d);
    parseArgs(argc, argv, &argv[1], &argv[2], &d);
    crawl(argv[1], argv[2], d);
    
}

/* parseArgs */
/*Given arguments from the command line, extract them into the function parameters;
* return only if successful.*/
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth){
    //normalize & validate URL
    char* new_url = normalizeURL(*seedURL);
    if(!isInternalURL(new_url)){
        fprintf(stderr, "seedURL is not internal\n");
        return;
    }
    // if url is internal, initiate page directory
    if(!pagedir_init(*pageDirectory)){
        fprintf(stderr, "error in open page directory\n");
        return;
    }
    // ensure maxDepth is an integer in specified range
    if(*maxDepth<0 || *maxDepth>10){
        /* check is integer? */
        fprintf(stderr, "Invalid maxDepth\n");
        return;
    }
    mem_free(new_url);
}

/* crawl */
/*Crawl from seedURL to maxDepth and saving pages in pageDirectory*/
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth){
    // initialize hashtable and add seedURL
    hashtable_t* table = hashtable_new(200);
    hashtable_insert(table, seedURL, "");
    // initialize the bag and add webpage at depth 0
    bag_t* bag0 = bag_new();
    // make a copy for seedURL in the allocated memory
    char * seedCpy = mem_malloc(strlen(seedURL)+1);
    strcpy(seedCpy, seedURL);
    // create webpage for seedURL, add to bag
    webpage_t* wp = webpage_new(seedCpy, 0, NULL);
    bag_insert(bag0, wp);
    int docID = 1;
    webpage_t* nwp; 
    while((nwp = bag_extract(bag0))!=NULL){
        // fetch html
        if(webpage_fetch(nwp)){
            //if successful, save nwp to pageDirectory
            pagedir_save(nwp, pageDirectory, docID);
            docID += 1;
            int currDepth = webpage_getDepth(nwp);
            if(currDepth < maxDepth){
                pageScan(nwp, bag0, table);
            }
        }
        // delete webpage
        webpage_delete(nwp);
    }
    //delete hashtable and bag
    hashtable_delete(table, NULL);
    bag_delete(bag0, webpage_delete);
}

/* pagescan */
/* scan the page if not at maxDepth and extract internal url from it */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen){
    // while there is another url in the page
    int pos = 0;
    char* result;
    int currDepth = webpage_getDepth(page);
    while((result = webpage_getNextURL(page, &pos)) != NULL){
        char* normal = normalizeURL(result);
        // check if it is internal
        if(isInternalURL(normal)){
            bool hashin = hashtable_insert(pagesSeen, normal, "");
            if(hashin){
                // create webpage and put into bag
                webpage_t* nwp = webpage_new(normal, currDepth+1, NULL);
                bag_insert(pagesToCrawl, nwp);
            }
            else{
                // if insert fails, free normal 
                mem_free(normal);
            }
        }else{
            mem_free(normal);
        }
        // free the url
        mem_free(result);
    }
}
/*
* pagedir - untility functions for handle pagesaver
* this module handle writings a page into page directory
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <mag.h>
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"

/***************************** pagedir_init ****************************/
bool pagedir_init(const char* pageDirectory){
    // create pathname
    char* fname = "/.crawler";
    // allocate memory for pathname
    char* pname = mem_calloc_assert(strlen(fname)+strlen(pageDirectory)+1, sizeof(char), "failed to allocate memory for file path\n");
    // check if allocation failed
    if(pname == NULL){
        return false;
    }
    // copty page directory into the new allocated space
    strncpy(pname, pageDirectory, strlen(pageDirectory));
    // conbine page directory and file name
    strncat(pname, fname, strlen(fname));
    //open the file for writing; on error, return false
    FILE* fp = fopen(pname, "w");
    if(fp == NULL){
        fprintf(stderr, "error in open file: %s \n", pname);
        mem_free(pname);
        return false;
    }
    fclose(fp);
    mem_free(pname);
    return true;
}

/*************************** pagedir_save ******************************/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID){
    //calculate the length of docID, (>= 0)
    int numd;
    if(docID != 0){
        numd = floor(log10(docID)) + 1;
    }
    else{
        numd = 1;
    }
    //construct path name
    /* how to allocate memory here??? */
    char* pname = mem_calloc_assert(strlen(pageDirectory) + numd + 2, sizeof(char), "failed to allocate memory for new file path\n");
    strncpy(pname, pageDirectory, strlen(pageDirectory));
    strcat(pname, "/");
    sprintf(pname + strlen(pageDirectory)+1, "%d", docID);
    //open file for writing
    FILE* fp = fopen(pname, "w");
    if(fp == NULL){
        fprintf(stderr, "error in open file: %s \n", pname);
        mem_free(pname);
    }
    //print the url, depth , contents of the webpage
    fprintf(fp, "%s\n", webpage_getURL(page));
    fprintf(fp, "%d\n", webpage_getDepth(page));
    fprintf(fp, "%s\n", webpage_getHTML(page));
    //close the file, free the path name
    fclose(fp);
    mem_free(pname);
}

/************************* pagedir_validate ***********************/
bool pagedir_validate(char* pageDirectory){
    if(pageDirectory == NULL){
        return false;
    }
    // construct path name for .crawler and check if it is in the directory
    char* fname = "/.crawler";
    char* pname = mem_calloc(strlen(fname)+strlen(pageDirectory)+1, sizeof(char));
    if(pname == NULL){
        fprintf(stderr, "memory allocation failure\n");
        return false;
    }
    strncpy(pname, pageDirectory, strlen(pageDirectory));
    strncat(pname, fname, strlen(fname));
    // open the file for reading
    FILE* fp = fopen(pname, "r");
    if(fp == NULL){
        fprintf(stderr, "cannot access /.crawler\n");
        mem_free(pname);
        return false;
    }
    fclose(fp);
    mem_free(pname);
    return true;
    
}

/************************** pagedir_load ***************************/
webpage_t* pagedir_load(char* directory, const int docID){
    //construct path name for the file
    if(docID <= 0){
        fprintf(stderr, "Invalid docID\n");
        return NULL;
    }
    //calculate the length of docID, (>= 0)
    int numd;
    numd = sizeof(docID);
    //allocate memory for path name
    char* pname = mem_malloc(strlen(directory) + numd + 2);
    //check allocation
    if(pname == NULL){
        fprintf(stderr, "memory allocation failure\n");
        return NULL;
    }
    //construct path name
    sprintf(pname, "%s/%d", directory, docID);
    //open file for reading
    FILE* fp = fopen(pname, "r");
    if(fp == NULL){
        // fprintf(stderr, "error in open file: %s \n", pname);
        mem_free(pname);
        return NULL;
    }
    // load the webpage from file, extract info
    char* url = file_readLine(fp);
    char* depth = file_readLine(fp);
    char* html = file_readUntil(fp, NULL);
    // check if info is null
    if((url == NULL || depth == NULL)||html == NULL){
        fprintf(stderr, "null content detected in the file: %s \n", pname);
        return NULL;
    }
    // pass them into a new webpage
    int d;
    sscanf(depth, "%d", &d);
    webpage_t* newpage = webpage_new(url, d, html);
    mem_free(depth);
    mem_free(pname);
    if(newpage == NULL){
        fprintf(stderr, "failed to create new webpage object\n");
        return NULL;
    }
    fclose(fp);
    return newpage;
}
/*
* pagedir - untility functions for handle pagesaver
* this module handle writings a page into page directory
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/*************************** pagedir_init******************************/
/*
* construct the pathname for the .crawler file in that directory
* open the file for writing; on error, return false.
* close the file and return true.
*/
bool pagedir_init(const char* pageDirectory);

/*************************** pagedir_save *****************************/
/*
* construct the pathname for the page file in pageDirectory
* open that file for writing
* print the URL
* print the depth
* print the contents of the webpage
* close the file
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/************************* pagedir_validate ***********************/
/*
* validate if the pageDirectory is indeed the directory of a cralwer's result
* if it is a crawler output directory, it should include .crawler and docID from 1 
* user pass in :
*   char* pageDirectory
* function returns:
*   true if direcotry is validated, false if not
*/
bool pagedir_validate(char* pageDirectory);

/************************** pagedir_load ***************************/
/*
* load a webpage from a file in that directory
*/
webpage_t* pagedir_load(char* directory, const int docID);
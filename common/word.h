/*
* index.h - header file for words module
* it execute functions proccessing words
* Tianwen Chen, Feb 2023
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <netdb.h>


char* normalizeWord(const char* word);
/*
* word.c module for indexer
* Lab5, CS50
* Tianwen Chen, Jan 2023
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <netdb.h>

/********************* normalizeWord ********************/
const char* normalizeWord(const char* word){
    if (word == NULL){
        return NULL;
    }
    //allocate space for result word
    char* result = malloc(strlen(word)+1);
    for(int i = 0; i < strlen(word); i++){
        result[i] = tolower(word[i]); 
    }
    result[strlen(word)] = '\0';
    return result;
}

// /********unit test**********/
// int main(const int argc, char* argv[]){
//     char* test = "TEst";
//     char* res = normalizeWord(test);
//     return 0;
// }
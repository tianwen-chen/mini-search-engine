# CS50 TSE Querier
## Design Spec

In this document we focus on the design choices of Querier module.

## Functions & modules needed for querier
In querier.c, we need to parse the arugment, validate argument, and then initiate search according to processed argument. We need these functions:
1) main
2) parse_input
3) validate_input
4) implement_logic
5) index_load
6) result_rank

### parse_input
it takes in the input and parse it to an array of pointer to words(char*). Note that we don't allocate memory for words, we just use what's in the input

### validate_input
It takes in an array of char* `words` and the number of words in the array, and if the input's grammer is valid, return the number of wordd in the new array of char*, which is "normalized", i.e. "dartmouth college" becomes "dartmouth and college".
If the input's grammar is not valid, return 0.

### implement_logic
It takes in an array of char* `words` and implement the logic in the sentence
pseudo code:
```
char* input[n]
int i = 0    //pointer of current read
int sum = 0
while(i < n):
	prod = input[i]
		while(input[i+1] == '*'):  //peek in advance
 			prod = prod * next_var     // curr_var = input[i+2]
			i += 2   //proceed to the next variable
	sum = sum + prod
	i += 2
```

### index_load
load an index object from an index file.

### result_rank
Given the searched result (counter), we rank according to the score and print them. 

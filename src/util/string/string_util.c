#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*
Extract a substring from a source string
@param string Source string
@param begin Start index for substring (included)
@param end End index for substring (excluded)
@return Pointer to a new string
*/
char* substring(char* string, int begin, int end) {
    int substring_size = end - begin;

    if(substring_size <= 0) {
        fprintf(stderr, "ERROR: Invalid intexes for substring extraction");
        return NULL;
    }

    char* substr = malloc(substring_size);

    if(substr == NULL) {
        fprintf(stderr, "FATAL: could not allocate memory for substring extraction");
        exit(EXIT_FAILURE);
    }

    strncpy(substr, string, substring_size);
}
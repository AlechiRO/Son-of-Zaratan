#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "lexer_util.h"


/*
Initialize a line struct
@return Pointer to a line struct
*/
line_s* initialize_line(void) {
    line_s* line = malloc(sizeof(struct line));
    if(line == NULL)
        return NULL;
    line->buffer = NULL; 
    line->length = 0; 
    line->input = stdin;    // Default input for line
    return line;
} 

/*
Wrapper function for the getline() function
@param line Pointer to line struct
@param input Pointer to FILE struct
*/
void getline_wrap(line_s* line, FILE* input) {
    if(line == NULL) {
        printf("Line can't be read, memory has not been allocated!");
        return;
    }
    line->input = input;
    getline(&line->buffer, &line->length, input);
}

/*
Free the memory allocated for a line struct
@param line Pointer to a pointer of a line struct
*/
void destroy_line(line_s** line) {
    if((*line) == NULL || line == NULL) {
        printf("Could not destroy line struct!");
        return;
    }
    if((*line)->buffer != NULL) 
        free((*line)->buffer);

    free((*line));
    (*line) = NULL;
    printf("Line struct has been destroied!");
}
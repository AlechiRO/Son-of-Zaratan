#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "lexer_util.h"
#include "lexer.h"

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
@return the number of characters read
*/
int getline_wrap(line_s* line, FILE* input) {
    if(line == NULL) {
        printf("Line can't be read, memory has not been allocated!\n");
        return -1;
    }
    line->input = input;
    return getline(&line->buffer, &line->length, input);
}

/*
Free the memory allocated for a line struct
@param line Pointer to a pointer of a line struct
*/
void destroy_line(line_s** line) {
    if((*line) == NULL || line == NULL) {
        printf("Could not destroy line struct!\n");
        return;
    }
    if((*line)->buffer != NULL) 
        free((*line)->buffer);

    free((*line));
    (*line) = NULL;
    printf("Line struct has been destroyed!\n");
}

/*
Append a line of code to the source code
@param line Pointer to line struct containing the line of code
*/
void append_to_source(lexer_context_s* lctx, line_s* line) {
    char* content = line->buffer;
    lctx->source = realloc(lctx->source, lctx->source_length + strlen(content));

    if(lctx->source == NULL) {
        fprintf(stderr, "FATAL: could not reallocate memory for source code");
        exit(EXIT_FAILURE);
    }

    strcat(lctx->source, content);
    lctx->source_length += strlen(content);
}

/*
Append code to existing line of code
@param line Pointer to line struct 
@param content Code to append
*/
void append_to_line(line_s* line, char* content) {
    int content_length = strlen(content);
    line->buffer = realloc(line->buffer, sizeof(line->buffer) + content_length);

    if(line->buffer == NULL) {
        fprintf(stderr, "FATAL: could not reallocate memory for line buffer");
        exit(EXIT_FAILURE);
    }

    strcat(line->buffer, content);
    line->length += content_length;
}

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "lexer_util.h"
#include "lexer.h"
#include "string_util.h"

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


/*
Constructor
Initializes the lexer context struct and 
assigns each field to base value
@return Pointer to a lexer context struct
*/
lexer_context_s* initialize_lexer_context(void) {
    lexer_context_s* lctx = malloc(sizeof(lexer_context_s));
    if(lctx == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for the lexer context!\n");
        exit(EXIT_FAILURE);
    }
    // Initialize a new token list
    token_list* tokens = token_list_initialize();
    
    lctx->state = STATE_DEFAULT;
    lctx->source = NULL;
    lctx->tokens = tokens;
    lctx->start = 0;
    lctx->current = 0;
    lctx->line_number = 1;
    lctx->source_length = 0;
}

/*
Destructor
Free the memory allocated for the lexer context
@params lctx Pointer to a pointer to a lexer context struct
*/
void destroy_lexer_context(lexer_context_s** lctx) {
    if((*lctx) == NULL || lctx == NULL) {
        fprintf(stderr, "INFO: Could not destroy NULL lexer context");
        return;
    }

    token_list_destroy(&((*lctx)->tokens));

    if((*lctx)->source != NULL) 
        free((*lctx)->source);
    
    free(*lctx);
    *lctx = NULL;
}

/*
Check if end of source is reached
@return 1 if true or 0 if false
*/
int is_at_end(lexer_context_s* lctx) {
    return lctx->current >= lctx->source_length;
}

/*
Add token to the token list
@param lctx Pointer to lexer context struct
@param type Type of token
@param literal Pointer to literal struct
*/
void add_token(lexer_context_s* lctx, token_type_e type, literal_s* literal) {
    char* lexeme = substring(lctx->source, lctx->start, lctx->current);
    token_list_add(lctx->tokens, initialize_token(type, lexeme, literal, lctx->line_number)); 
}

/*
Retrieve the character at the current position and move to the next character
@return The character at the current position
*/
char advance(lexer_context_s* lctx) {
    return lctx->source[lctx->current++];
}

/*
Check if the next character matches the expected character
@param lctx Pointer to lexer context struct
@param expected Character that is expected after the last scanned character
@return 1 if the character matches the expected value, else 0
*/
int match(lexer_context_s* lctx, char expected) {
    if(is_at_end(lctx)) 
        return 0; 
    if((lctx->source)[lctx->current] != expected) 
        return 0;
    lctx->current++;
    return 1;
}

/*
Get the character at the current index
@param lctx Pointer to lexer context struct
@return The character at current index or '\0'(NULL) if end of source is reached
*/
char peek(lexer_context_s* lctx) {
    if(is_at_end(lctx)) 
        return '\0';
    return (lctx->source)[lctx->current];
}

/*
Peek at the next character in the source code
@param lctx Pointer to lexer context struct
@return the next character in the source code or '\0' if there is no such character
*/
char peek_next(lexer_context_s* lctx) {
    if(lctx->current + 1 >= lctx->source_length)
        return '\0';
    return (lctx->source)[lctx->current + 1];
}

/*
Match the next character, identify the token and add it to the list
@param lctx Pointer to lexer context struct
@param expected Array of expected characters
@param types Array of corresponding types for the exected characters
@param size Size of both arrays (Must have the same size)
@param default_type Default token type if none of the expected characters match
*/
void match_multiple_tokens(lexer_context_s* lctx, char expected[], token_type_e types[], size_t size, token_type_e default_type) {
    for(size_t i = 0; i < size; i++) {
        if(match(lctx, expected[i])) {
            add_token(lctx, types[i], NULL);
            return;
        }
    }
    add_token(lctx, default_type, NULL);
}

/*
For each token check for leading whitespace
@param lctx Pointer to lexer context struct
@return 1 if token has leading whitespace and 0 otherwise
*/
int check_leading_whitespace(lexer_context_s* lctx) {
    int found = 0;
    while(!is_at_end(lctx)) {
        char c = peek(lctx);
        if(c == ' ' || c == '\t' || c == '\r') {
            advance(lctx);
            found = 1;
        } else break;
    }
    return found;
}

/*
Mark the leading whitespace flag for last added token
@param lctx Pointer to lexer context struct
@param whitespace Boolean denoting leading whitespace
@param token_count_before The number of tokens before calling this function
*/
void mark_token_whitespace(lexer_context_s* lctx, int whitespace, int token_count_before) {
    if(token_count_before != token_list_get_size(lctx->tokens) - 1) 
        return;
    token_s* last_token = token_list_get(lctx->tokens, token_list_get_size(lctx->tokens) - 1);
    last_token->leading_whitespace = whitespace;
    
}
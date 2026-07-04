#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lexer.h"
#include "token.h"
#include "error.h"
#include "string_util.h"

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
Main Lexer Loop
@param line The line of code currently being scanned
*/
token_list* lex(line_s* line) {
    lexer_context_s* lctx = initialize_lexer_context();
    append_to_source(lctx, line);

    while(!is_at_end(lctx)) {
        lctx->start = lctx->current;
        scan_token(lctx);
    }

    // Add this token to mark the end of teh source code
    add_token(lctx, TOKEN_EOF, NULL);
    
    return lctx->tokens;
}

/*
Scan the current token
*/
void scan_token(lexer_context_s* lctx) {
    char c = advance(lctx);
    switch(c) {
    case '(' : add_token(lctx, TOKEN_ROUND_BRACE_LEFT, NULL); break;
    case ')' : add_token(lctx, TOKEN_ROUND_BRACE_RIGHT, NULL); break;
    case '[' : add_token(lctx, TOKEN_SQUARE_BRACE_LEFT, NULL); break;
    case ']' : add_token(lctx, TOKEN_SQUARE_BRACE_RIGHT, NULL); break;
    case '{' : add_token(lctx, TOKEN_CURLY_BRACE_LEFT, NULL); break;
    case '}' : add_token(lctx, TOKEN_CURLY_BRACE_RIGHT, NULL); break;
    case '.' : add_token(lctx, TOKEN_DOT, NULL); break;
    case ':' : add_token(lctx, TOKEN_COLON, NULL); break;
    case ';' : add_token(lctx, TOKEN_SEMICOLON, NULL); break;
    case ',' : add_token(lctx, TOKEN_COMMA, NULL); break;
        
    default: error(lctx->line_number, "Unexpected character"); break;
    }
}



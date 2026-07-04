#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lexer.h"
#include "token.h"
#include "lexer_globals.h"
#include "error.h"


lexer_context_s* initialize_lexer_config(void) {
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
Check if end of source is reached
@return 1 if true or 0 if false
*/
int is_at_end(void) {
    return current >= source_length;
}

/*
Add token to the token list
@param type Type of token
@param literal Pointer to literal struct
*/
void add_token(token_type_e type, literal_s* literal) {
    char* lexeme = substring(source, start, current);
    token_list_add(tokens, initialize_token(type, lexeme, literal, line_number)); 
}

/*
Retrieve the character at the current position and move to the next character
@return The character at the current position
*/
char advance(void) {
    return source[current++];
}

/*
Main Lexer Loop
@param line The line of code currently being scanned
*/
token_list* lex(line_s* line) {
    append_to_source(line);

    while(!is_at_end()) {
        start = current;
        scan_token();
    }

    // Add this token to mark the end of teh source code
    add_token(TOKEN_EOF, NULL);
    
    return tokens;
}

/*
Scan the current token
*/
void scan_token(void) {
    char c = advance();
    switch(c) {
    case '(' : add_token(TOKEN_ROUND_BRACE_LEFT, NULL); break;
    case ')' : add_token(TOKEN_ROUND_BRACE_RIGHT, NULL); break;
    case '[' : add_token(TOKEN_SQUARE_BRACE_LEFT, NULL); break;
    case ']' : add_token(TOKEN_SQUARE_BRACE_RIGHT, NULL); break;
    case '{' : add_token(TOKEN_CURLY_BRACE_LEFT, NULL); break;
    case '}' : add_token(TOKEN_CURLY_BRACE_RIGHT, NULL); break;
    case '.' : add_token(TOKEN_DOT, NULL); break;
    case ':' : add_token(TOKEN_COLON, NULL); break;
    case ';' : add_token(TOKEN_SEMICOLON, NULL); break;
    case ',' : add_token(TOKEN_COMMA, NULL); break;
        
    default: error(line_number, "Unexpected character"); break;
    }
}



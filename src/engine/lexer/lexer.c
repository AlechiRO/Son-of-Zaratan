#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lexer.h"
#include "token.h"
#include "error.h"
#include "string_util.h"
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
Verify the next chacacter and match the token with either type 1, 2 or 3
used for 2 character tokens with same starting character
@param lctx Pointer to lexer context struct
@param expected_1 First expected character
@param type_1 Type corresponding to the first expected character
@param expected_2 Second expected character
@param type_2 Type corresponding to the second expected character
@param type_3 Type corresponding to the previously scanned character followed by some other character
*/
void match_two_tokens(lexer_context_s* lctx, char expected_1, token_type_e type_1, char expected_2, token_type_e type_2, token_type_e type_3) {
    if(match(lctx, expected_1)) 
        add_token(lctx, type_1, NULL);
    else if(match(lctx, expected_2))
        add_token(lctx, type_2, NULL);
    else 
        add_token(lctx, type_3, NULL);
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

    // Add this token to mark the end of the source code
    add_token(lctx, TOKEN_EOF, NULL);
    
    return lctx->tokens;
}

/*
Scan the current token
*/
void scan_token(lexer_context_s* lctx) {
    char c = advance(lctx);
    switch(c) {
    // Single character tokens
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
    case '/' : add_token(lctx, TOKEN_SLASH, NULL); break;
    // Double character tokens
    case '!' : add_token(lctx, match(lctx, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG, NULL); break;
    case '=' : add_token(lctx, match(lctx,'=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL, NULL); break;
    case '+' : add_token(lctx, match(lctx,'+') ? TOKEN_INCREMENT : TOKEN_PLUS, NULL); break;
    case '|' : add_token(lctx, match(lctx,'|') ? TOKEN_OR : TOKEN_PIPE, NULL); break;
    case '*' : add_token(lctx, match(lctx,'*') ? TOKEN_POW : TOKEN_STAR, NULL); break;
    // Duble character, multiple options tokens
    case '<' : 
        match_two_tokens(lctx, '=', TOKEN_LESS_EQUAL, '<', TOKEN_HEREDOC_REDIRECT, TOKEN_LESS);
        break;
    case '>' : 
        match_two_tokens(lctx, '=', TOKEN_GREATER_EQUAL, '>', TOKEN_APPEND_REDIRECT, TOKEN_GREATER);
        break;
    case '-' : 
        match_two_tokens(lctx, '-', TOKEN_DECREMENT, '>', TOKEN_ARROW, TOKEN_MINUS);
        break;
    // Ignoring comments
    case '~':
        while(peek(lctx) != '\n' && !is_at_end(lctx)) 
            advance(lctx);
        break;
    // Ignore whitespace
    case '\r':
    case '\t':
        break;
    // Handle terminator
    case '\n':
        add_token(lctx, TOKEN_TERMINATOR, NULL);
        lctx->line_number++;
        break;
        
    default: error(lctx->line_number, "Unexpected character"); break;
    }
}



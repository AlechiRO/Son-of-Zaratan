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
Verify the next chacacter and match the token with either type 1, 2 or 3
used for 2 character tokens with same starting character
Adds the token after matching the correct type
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

void number(lexer_context_s* lctx) {
    while(is_digit(peek(lctx)))
        advance(lctx);
    
    if(peek(lctx) == '.' && is_digit(peek_next(lctx))) {
        // Consume the "."
        advance(lctx);
        while(is_digit(peek(lctx)))
            advance(lctx);

        // Add the token to the list
        double result;
        int success = parse_double(substring(lctx->source, lctx->start, lctx->current), &result);
        if(success) {
            literal_s* literal = initialize_literal(LITERAL_DOUBLE);
            (literal->value).double_value = result;
            add_token(lctx, TOKEN_NUMBER, literal);
        }
    }
}

/*
Scan a string token and add it to the list of tokens
@param lctx Pointer to lexer context struct
@param glob Boolean denoting whether the string allows globbing
*/
void string(lexer_context_s* lctx, int glob) {
    char string_terminator = glob ? '"' : '\'';
    while(peek(lctx) != string_terminator && !is_at_end(lctx)) {
        if(peek(lctx) == '\n') 
            lctx->line_number++;
        advance(lctx);
    }

    if(is_at_end(lctx)) {
        error(lctx->line_number, "Unterminated String");
        return;
    }
    // Consume the closing "
    advance(lctx);
    
    // Add the token to the list
    char* value = substring(lctx->source, lctx->start + 1, lctx->current-1);
    literal_s* literal = initialize_literal(LITERAL_STRING);
    (literal->value).string_value = value;

    if(glob)
        add_token(lctx, TOKEN_STRING_GLOB, literal);
    else 
        add_token(lctx, TOKEN_STRING_DEFAULT, literal);
}

/*
Main Lexer Loop
@param line The line of code currently being scanned
*/
token_list* lex(line_s* line) {
    lexer_context_s* lctx = initialize_lexer_context();
    append_to_source(lctx, line);
    int whitespace = 0;
    int token_count = 0;
    while(!is_at_end(lctx)) {
        lctx->start = lctx->current;
        // Handle and checck for whitespace
        whitespace = check_leading_whitespace(lctx);
        // Compute token count before adding a token
        token_count = token_list_get_size(lctx->tokens);
        // Scan the current token
        if(!is_at_end(lctx)) {
            scan_token(lctx);
            // Compute leading whitespace flag for scanned token
            mark_token_whitespace(lctx, whitespace, token_count);
        }
    }
    // Compute token count before adding EOF token
    token_count = token_list_get_size(lctx->tokens);
    // Add this token to mark the end of the source code
    add_token(lctx, TOKEN_EOF, NULL);
    // Compute leading whitespace flag for EOF token
    mark_token_whitespace(lctx, whitespace, token_count);
    return lctx->tokens;
}

/*
Scan the current token
@param lctx Pointer to lexer context struct
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
    // Handle terminator
    case '\n':
        add_token(lctx, TOKEN_TERMINATOR, NULL);
        lctx->line_number++;
        break;
    // Handle strings
    case '"': 
        string(lctx, 1);
        break;
    case '\'':
        string(lctx, 0);
        break;
        
    default: 
        if(is_digit(c))
            number(lctx);
        else
            error(lctx->line_number, "Unexpected character"); break;
    }
}



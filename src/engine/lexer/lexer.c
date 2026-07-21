#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lexer.h"
#include "token.h"
#include "error.h"
#include "string_util.h"
#include "lexer_util.h"


/*
Scan a number token and add it to the list of tokens
@param lctx Pointer to lexer context struct
*/
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
Add an identifier token to the list
@param lctx Pointer to lexer context struct
*/
void identifier(lexer_context_s* lctx) {
    while(is_alphanumeric(peek(lctx)))
        advance(lctx);

    add_token(lctx, TOKEN_IDENTIFIER, NULL);
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
        match_multiple_tokens(lctx, (char[]){'=', '<', '&'}, (token_type_e[]){TOKEN_LESS_EQUAL, TOKEN_HEREDOC_REDIRECT, TOKEN_DUP_IN}, 3, TOKEN_LESS);
        break;
    case '>' : 
        match_multiple_tokens(lctx, (char[]){'=', '>', '&'}, (token_type_e[]){TOKEN_GREATER_EQUAL, TOKEN_APPEND_REDIRECT, TOKEN_DUP_OUT}, 3, TOKEN_GREATER);
        break;
    case '-' : 
        match_multiple_tokens(lctx, (char[]){'-', '>', 'a', 'd', 's'},
        (token_type_e[]){TOKEN_DECREMENT, TOKEN_ARROW, TOKEN_FILE_EXIST, TOKEN_FILE_IS_DIR, TOKEN_FILE_IS_NOT_EMPTY}, 5, TOKEN_MINUS);
        break;
    case '$' :
        match_multiple_tokens(lctx, (char[]){'#', '@'}, (token_type_e[]){TOKEN_ARGUMENT_NUMBER, TOKEN_ARGUMENT_ARRAY}, 2, TOKEN_DOLLAR);
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
        else if(is_alpha(c))
            identifier(lctx);
        else
            error(lctx->line_number, "Unexpected character"); break;
    }
}



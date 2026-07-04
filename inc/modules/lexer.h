#ifndef LEXER_H
#define LEXER_H

#include "lexer_globals.h"
#include "lexer_util.h"
#include "token.h"

// Lexer config struct definition
typedef struct Lexer_context {
    char* source;           // String containing the script
    token_list* tokens;     // Token Array List
    int start;              // Start index of the current lexeme
    int current;            // Index of the currently scanned character in the lexeme
    int line_number;        // Tracks the line current is on 
    int source_length;      // The length of the source code
} lexer_context_s;

void add_token(token_type_e type, literal_s* literal);
char advance(void);
token_list* lex(line_s* line);
void scan_token(void);

#endif
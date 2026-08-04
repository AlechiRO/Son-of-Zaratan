#ifndef LEXER_H
#define LEXER_H

#include "lexer_generics.h"
#include "lexer_util.h"
#include "token.h"
// Setup for Token Array List


//Lexer state enum
typedef enum Lexer_state {
    STATE_DEFAULT,            // Programming expressions (math, vars, control flow)
    STATE_COMMAND,            // Shell commands and bare-word arguments
    STATE_STRING_GLOB,          // Inside "...", allows globbing/expansion
    STATE_STRING_SIMPLE,      // Inside '...', raw text, no expansion
    STATE_HEREDOC_BODY        // Processing lines until reaching the EOF marker
} lexer_state_e;

// Lexer config struct definition
typedef struct Lexer_context {
    char* source;           // String containing the script
    token_list* tokens;     // Token Array List
    lexer_state_e state;    // Current state of the lexer
    int start;              // Start index of the current lexeme
    int current;            // Index of the currently scanned character in the lexeme
    int line_number;        // Tracks the line current is on 
    int source_length;      // The length of the source code
} lexer_context_s;

lexer_context_s* initialize_lexer_context(void);
void destroy_lexer_context(lexer_context_s** lctx);
void add_token(lexer_context_s* lctx, token_type_e type, literal_s* literal);
char advance(lexer_context_s* lctx);
token_list* lex(line_s* line, string_token_type_hashmap* token_map);
void scan_token(lexer_context_s* lctx, string_token_type_hashmap* token_map);
void load_keywords(string_token_type_hashmap* token_map);
void number(lexer_context_s* lctx);
void string(lexer_context_s* lctx, int glob);
void identifier(lexer_context_s* lctx, string_token_type_hashmap* token_map);

#endif
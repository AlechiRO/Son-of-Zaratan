#ifndef LEXER_H
#define LEXER_H

#include "lexer_util.h"
#include "token.h"
// Setup for Token Array List
#define ARRAY_LIST_ITEM_TYPE token_s*
#define ARRAY_LIST_TAG token_list
#include "array_list.h"

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
int is_at_end(lexer_context_s* lctx);
void add_token(lexer_context_s* lctx, token_type_e type, literal_s* literal);
char advance(lexer_context_s* lctx);
token_list* lex(line_s* line);
void scan_token(lexer_context_s* lctx);

#endif
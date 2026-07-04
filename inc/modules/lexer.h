#ifndef LEXER_H
#define LEXER_H

#include "lexer_globals.h"
#include "lexer_util.h"
#include "token.h"

void add_token(token_type_e type, literal_s* literal);
char advance(void);
token_list* lex(line_s* line);
void scan_token(void);

#endif
#ifndef ERROR_H
#define ERROR_H
#include "lexer.h"

void report(int line, char* where, char* message);
void error(lexer_context_s* lctx, char* message);
int check_error(lexer_context_s* lctx);

#endif
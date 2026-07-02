#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lexer.h"
#include "token.h"
#include "lexer_globals.h"

/*
Main Lexer Loop
@param line The line of code currently being scanned
*/
void lex(line_s* line) {
    tokens = token_list_initialize();
}

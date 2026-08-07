#include <stdio.h>
#include "printer.h"
#include "error.h"

/*
Report an error
@param line Number of the line where the error occured
@param where
@param message Error message
*/
void report(int line, char* where, char* message) {
    printf("[line %d] Error %s: %s\n", line, where, message);
}

/*
Catch syntax error
@param lctx Pointer to lexer context struct
@param message Error message
*/
void error(lexer_context_s* lctx, char* message) {
    report(lctx->line_number, "", message);
    lctx->had_error = 1;
}

/*
Checks whether there is an error and resets the error status
@param lctx Pointer to lexer context struct
@return 1 if there is an error or 0 if the script has no errors
*/
int check_error(lexer_context_s* lctx) {
    int error = lctx->had_error;
    lctx->had_error = 0;
    return error;
}


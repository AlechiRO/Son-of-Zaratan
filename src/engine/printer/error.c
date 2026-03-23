#include <stdio.h>
#include "printer.h"

int had_error = 0;

/*
Report an error
@param line Number of the line where the error occured
@param where
@param message Error message
*/
void report(int line, char* where, char* message) {
    printf("[line %d] Error %s: %s", line, where, message);
    had_error = 1;
}

/*
Catch syntax error
@param line Number of the line of the syntax error
@param message Error message
*/
void error(int line, char* message) {
    report(line, "", message);
}

/*
Checks whether there is an error and resets the error status
@return 1 if there is an error or 0 if the script has no errors
*/
int check_error() {
    int error = had_error;
    had_error = 0;
    return error;
}


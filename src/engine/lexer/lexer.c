#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

// Setup for Token Array List
#define ARRAY_LIST_ITEM_TYPE token_s
#define ARRAY_LIST_TAG token_list
#include "array_list.h"

/* Global Variables 
source - String containing the script
tokens - Token Array List
start -
current -
line -
*/

char* source;
token_list* tokens;
int start;
int current;
int line;

void lex(line_s* line) {

}

#include "token.h"

// Setup for Token Array List
#define ARRAY_LIST_ITEM_TYPE token_s
#define ARRAY_LIST_TAG token_list
#include "array_list.h"

/* Global Variables 
source - String containing the script
tokens - Token Array List
start - points to the start index of the lexeme
current - points to the character currently being considered
line - tracks the line current is on 
*/
extern char* source;
extern token_list* tokens;
extern int start;
extern int current;
extern int line;
extern int source_length;
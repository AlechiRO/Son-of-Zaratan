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
line_number - tracks the line current is on 
source_length - the length of the source code
*/

char* source = NULL;
token_list* tokens;
int start = 0;
int current = 0;
int line_number = 1;
int source_length = 0;
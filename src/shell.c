#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lexer.h"
#include "printer.h"
#include "lexer_util.h"


int main(void) {
    line_s* line = initialize_line();
    string_token_type_hashmap* token_map = string_token_type_hashmap_initialize();
    load_keywords(token_map);
    // REPL State
    while(1) {
        default_prompt();
        
        // Read line from stdin 
        if(getline_wrap(line, stdin) == -1)
            break;

        token_list* tokens = lex(line, token_map);
        for(int i = 0; i < token_list_get_size(tokens); i++) {
            token_s* token = token_list_get(tokens, i);
            printf("---------\nLexeme: %s | Type: %d\n", token->lexeme, token->type);
        }
        token_list_destroy(&tokens);
        if(check_error())  
            break;
    }
    destroy_line(&line);
    string_token_type_hashmap_destroy(&token_map);
    return 0;
}
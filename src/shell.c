#include <stdio.h>
#include <stdlib.h>
#include <bits/local_lim.h>
#include <unistd.h>
#include <string.h>
#include "lexer.h"
#include "printer.h"
#include "lexer_util.h"



int main(void) {
    line_s* line = initialize_line();
    // REPL State
    while(1) {
        default_prompt();
        
        // Read line from stdin 
        if(getline_wrap(line, stdin) == -1)
            break;

        lex(line);    
        if(check_error())  
            break;
    }
    destroy_line(&line);
    return 0;
}
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include <stdlib.h>

/*
Check if the command ends in a pipe operator

*/
void parse_line(char* lineptr, command_data* command_details) {
    if(lineptr[command_details->char_number-1] == '|')
        command_details->piped = 1;
    char* copy = malloc(command_details->char_number);
    strcpy(copy, lineptr);
    char* token = strtok(copy, " \n");
    while(token) {

        //Get the heredoc delimiter
        if(command_details->has_heredoc && !command_details->delimiter) 
            get_heredoc_delimiter(command_details, token);
        
        // Check if command has redirection operator "<<" and ignore tab hyphen "-"
        if(strcmp(token, "<<") == 0)
            command_details->has_heredoc = 1;
        else if(strcmp(token, "<<-") == 0) {
            command_details->has_heredoc = 1;
            command_details->ignore_tab = 1;
        }

        token = strtok(NULL, " \n");
    }
    free(copy);
}

/*
Get the heredoc delimiter and check for shell expansion
@param command_details Pointer to command data struct
@param token Pointer to the delimiter token
*/
void get_heredoc_delimiter(command_data* command_details, char* token) {
    command_details->delimiter = malloc(sizeof(token) + 1);
    int length = strlen(token);
    if(token[0] == '\'' && token[length-1] == '\''){
        command_details->shell_expansion = 1;
        strncpy(command_details->delimiter, token+1, length-2);
    } else {
        strcpy(command_details->delimiter, token);
    }

    strcat(command_details->delimiter, "\n");
}
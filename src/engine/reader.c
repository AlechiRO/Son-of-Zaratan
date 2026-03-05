#include <stdio.h>
#include <bits/local_lim.h>
#include "../../inc/parser.h"
#include "../../inc/reader.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
Display the default prompt of the format: "[USER]@[HOST]:~[cwd]$"
*/
void default_prompt(void) {
    const char* user = getenv("USER");
    char hostname[HOST_NAME_MAX];
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));
    gethostname(hostname, HOST_NAME_MAX);
    printf("%s@%s:~%s$ ", user, hostname, cwd);
}

/*
Read the command
@return Pointer to the command read
*/
char* read_command(void) {

    command_data* command_details = malloc(sizeof(command_data));

    // Set command details fields
    set_command_data_fields(command_details);

    // Read the first line
    char* lineptr = NULL;
    size_t length = 0;
    command_details->char_number = getline(&lineptr, &length, stdin);

    // Parse the first line
    parse_line(lineptr, command_details);
    
    // Capture the command
    char* command = malloc(command_details->char_number);
    strcpy(command, lineptr);

    
    if(command_details->has_heredoc == 1)
        command = capture_heredoc(lineptr, length, command_details, command);

    return command;
}

/*
Captures a command in a heredoc
@param lineptr Pointer to the first line of the command
@param length How much to read from each line
@param command_details Pointer to command data struct
@param command Pointer to the created command
@return Pointer to the created command
*/
char* capture_heredoc(char* lineptr, size_t length, command_data* command_details, char* command) {
    ssize_t char_number_line = 0;
    while(char_number_line != -1 && strcmp(lineptr, command_details->delimiter)){
        printf("> ");
        char_number_line = getline(&lineptr, &length, stdin);
        command_details->char_number += char_number_line;
        command = realloc(command, command_details->char_number);
        strcat(command, lineptr);
    } 
        
    
    return command;
}
/*
Set the fields of the command_data struct to the default values
*/
void set_command_data_fields(command_data* command_details) {
    command_details-> has_heredoc = 0;
    command_details->delimiter = NULL;
    command_details->shell_expansion = 0;
    command_details->ignore_tab = 0;
    command_details->char_number = 0;
    command_details->piped = 0;
}
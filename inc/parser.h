
#ifndef PARSER_H
#define PARSER_H
#include <unistd.h>
typedef struct command {
    int has_heredoc;         // contains <<
    char* delimiter;        // e.g EOF, END
    int shell_expansion;    // Quoted delimiter (e.g. 'EOF') 
    int ignore_tab;         // - after the refirection operator << !!!!Deal with this when you implement tab completion
    ssize_t char_number;    // Number of characters
    int piped;              // command ends in |
} command_data;

void parse_line(char* lineptr, command_data* command_details);
void get_heredoc_delimiter(command_data* command_details, char* token);
void set_command_data_fields(command_data* command_details);

#endif
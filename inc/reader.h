#ifndef READER_H
#define READER_H

char* read_command(void);
void default_prompt(void);
char* capture_heredoc(char* lineptr, size_t length, command_data* command_details, char* command);

#endif
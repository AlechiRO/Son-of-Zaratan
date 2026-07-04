#ifndef ERROR_H
#define ERROR_H

void report(int line, char* where, char* message);
void error(int line, char* message);
int check_error(void);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <bits/local_lim.h>
#include <unistd.h>
#include <string.h>
#include "../inc/parser.h"
#include "../inc/reader.h"



int main(void) {
    default_prompt();
    char* command = read_command();
    printf("%s", command);
    free(command);
    return 0;
}
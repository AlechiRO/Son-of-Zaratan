#include <stdio.h>
#include <bits/local_lim.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_DEFAULT "\033[39m"


/*
Display the default prompt of the format: "[USER]@[HOST]:~[cwd]$"
*/
void default_prompt(void) {
    const char* user = getenv("USER");
    char hostname[HOST_NAME_MAX];
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));
    gethostname(hostname, HOST_NAME_MAX); 
    printf(ANSI_COLOR_GREEN "%s@%s:" ANSI_COLOR_MAGENTA"~%s" ANSI_COLOR_DEFAULT "$ ", user, hostname, cwd);
}
#ifndef LEXER_UTIL_h
#define LEXER_UTIL_h

typedef struct line {
    char* buffer;
    size_t length;
    FILE* input;
}line_s;

line_s* initialize_line(void);
int getline_wrap(line_s* line, FILE* input);
void destroy_line(line_s** line);

#endif
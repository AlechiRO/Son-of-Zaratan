#ifndef LEXER_UTIL_h
#define LEXER_UTIL_h

typedef struct line {
    char* buffer;
    size_t length;
    FILE* input;
}line_s;

#endif
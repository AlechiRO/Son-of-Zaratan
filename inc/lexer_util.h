#ifndef LEXER_UTIL_h
#define LEXER_UTIL_h

typedef struct line {
    char* buffer;
    size_t length;
    FILE* input;
}line_s;


enum token_type{

    // Literal Tokens
    TOKEN_NUMBER,               // 123.33 or 43
    TOKEN_STRING,               // "Jon Snow"
    TOKEN_SPECIAL_STRING,       // 'Baratheon' 
    TOKEN_IDENTIFIER,           // Literal that will be checked against the command and environment var HashMaps
    TOKEN_VAR,                  // var
    
    // Single Charecter Tokens
    TOKEN_PLUS,                 // +
    TOKEN_MINUS,                // -
    TOKEN_SLASH,                // /
    TOKEN_STAR,                 // *
    TOKEN_EQUAL,                // =
    TOKEN_DOT,                  // .
    TOKEN_PIPE,                 // | 
    TOKEN_SEMICOLON,            // ;
    TOKEN_COLON,                // :
    TOKEN_COMMA,                // ,
    TOKEN_NEGATE,               // !
    TOKEN_WILDCARD,             // ?
    TOKEN_LESS_THAN,            // <
    TOKEN_GREATER_THAN,         // >

    // Double Character Tokens
    TOKEN_ARROW,                // ->
    TOKEN_APPEND_REDIRECT,      // >>
    TOKEN_HEREDOC_REDIRECT,     // <<
    TOKEN_POW,                  // **
    TOKEN_INCREMENT,            // ++
    TOKEN_DECREMENT,            // --
    TOKEN_AND,                  // &&
    TOKEN_OR,                   // ||
    TOKEN_ARGUMENT_NUMBER,      // $#
    TOKEN_ARGUMENT_ARRAY,       // $@
    TOKEN_FILE_DESCRIPTOR,      // &0 or &1 or &2

    // Braces Tokens
    TOKEN_ROUND_BRACE_LEFT,     // (
    TOKEN_ROUND_BRACE_RIGHT,    // )
    TOKEN_SQUARE_BRACE_LEFT,    // [
    TOKEN_SQUARE_BRACE_RIGHT,   // ]
    TOKEN_CURLY_BRACE_LEFT,     // {
    TOKEN_CURLY_BRACE_RIGHT,    // }

    // Dollar Tokens
    TOKEN_ARG_POSITION,         // $1 or $2 or $3 ...
    TOKEN_ARG_COUNT,            // $#
    TOKEN_ARG_ARRAY,            // $@
    TOKEN_INTERPOLATION_START,  // ${
    TOKEN_VARIABLE_REFERENCE,   // $myVar

    // Key Word Tokens
    TOKEN_IF,                   // if
    TOKEN_ELSE,                 // else
    TOKEN_WHILE,                // while
    TOKEN_FOR,                  // for
    TOKEN_TRUE,                 // true
    TOKEN_FALSE,                // false
    TOKEN_NULL,                 // null
    TOKEN_FUNC,                 // func
    TOKEN_RETURN,               // return

    // Terminator Tokens
    TOKEN_EOF,                  // END OF FILE
    TOKEN_ERROR,                // (If sequence of chars can't be turned into a token)
};


line_s* initialize_line(void);
int getline_wrap(line_s* line, FILE* input);
void destroy_line(line_s** line);

#endif
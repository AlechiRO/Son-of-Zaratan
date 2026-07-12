#ifndef TOKEN_H
#define TOKEN_H

#define ARRAY_LIST_ITEM_TYPE int
#define ARRAY_LIST_TAG int_list
#include "array_list.h"

#define ARRAY_LIST_ITEM_TYPE double
#define ARRAY_LIST_TAG double_list
#include "array_list.h"

#define ARRAY_LIST_ITEM_TYPE char*
#define ARRAY_LIST_TAG string_list
#include "array_list.h"

/* Token Enum */
typedef enum token_type {

    // Literal Tokens
    TOKEN_NUMBER,               // 123.33 or 43
    TOKEN_STRING,               // "Baratheon"  can be surrounded by "" or '' 
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
    TOKEN_BANG,                 // !
    TOKEN_WILDCARD,             // ?
    TOKEN_LESS,            // <
    TOKEN_GREATER,         // >

    // Double Character Tokens
    TOKEN_ARROW,                // ->
    TOKEN_APPEND_REDIRECT,      // >>
    TOKEN_HEREDOC_REDIRECT,     // <<
    TOKEN_POW,                  // **
    TOKEN_INCREMENT,            // ++
    TOKEN_DECREMENT,            // --
    TOKEN_AND,                  // &&
    TOKEN_OR,                   // ||
    TOKEN_BANG_EQUAL,           // !=
    TOKEN_EQUAL_EQUAL,          // ==
    TOKEN_LESS_EQUAL,           // <=
    TOKEN_GREATER_EQUAL,        // >=

    // Braces Tokens
    TOKEN_ROUND_BRACE_LEFT,     // (
    TOKEN_ROUND_BRACE_RIGHT,    // )
    TOKEN_SQUARE_BRACE_LEFT,    // [
    TOKEN_SQUARE_BRACE_RIGHT,   // ]
    TOKEN_CURLY_BRACE_LEFT,     // {
    TOKEN_CURLY_BRACE_RIGHT,    // }

    // Special Tokens
    TOKEN_DOLLAR,               // $
    TOKEN_ARGUMENT_NUMBER,      // $#
    TOKEN_ARGUMENT_ARRAY,       // $@
    TOKEN_FILE_DESCRIPTOR,      // &0 or &1 or &2
    TOKEN_COMMENT,              // ~ before writing a comment

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
} token_type_e;

typedef enum Literal_type {
    // Primitives
    LITERAL_INT,
    LITERAL_DOUBLE,
    LITERAL_NULL,
    LITERAL_BOOLEAN,
    LITERAL_STRING,

    // Array Lists
    LITERAL_INT_LIST,
    LITERAL_DOUBLE_LIST,
    LITERAL_STRING_LIST,

    // Dictionaries - Int Keys
    DICT_INT_INT,
    DICT_INT_DOUBLE,
    DICT_INT_STRING,
    
    // Dictionaries - Double Keys
    DICT_DOUBLE_INT,
    DICT_DOUBLE_DOUBLE,
    DICT_DOUBLE_STRING,
    
    // Dictionaries - String Keys
    DICT_STRING_INT,
    DICT_STRING_DOUBLE,
    DICT_STRING_STRING,

} literal_type_e;

/*Lexeme struct*/
typedef struct Literal {
    literal_type_e type;
    union Value {
        // Typed Primitives
        int int_value;
        double double_value;
        void* null_value;
        int boolean_value;
        char* string_value;

        // Typed Array Lists
        int_list* int_list_reference;
        double_list* double_list_reference;
        string_list* string_list_reference;

        // Typed Dictionaries - Keys: int

        //int_int_dict*    int_int_dict_reference;
        //int_double_dict* int_double_dict_reference;
        //int_string_dict* int_string_dict_reference;
        
        // Typed Dictionaries - Keys: double

        //double_int_dict*    double_int_dict_reference;
        //double_double_dict* double_double_dict_reference;
        //double_string_dict* double_string_dict_reference;
        
        // Typed Dictionaries - Keys: char*

        //string_int_dict*    string_int_dict_reference;
        //string_double_dict* string_double_dict_reference;
        //string_string_dict* string_string_dict_reference;
    } value;
} literal_s;


/* Token struct */
typedef struct Token {
    token_type_e type;
    char* lexeme;
    literal_s* literal; 
    int line;
} token_s;

/* Literal Functions */

literal_s* initialize_literal(literal_type_e type);
void destroy_literal(literal_s** literal);

/* Token Functions */

token_s* initialize_token(token_type_e type, char* lexeme, literal_s* literal, int line);
void destroy_token(token_s** token);

#endif
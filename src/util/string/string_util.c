#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*
Extract a substring from a source string
@param source Source string
@param begin Start index for substring (included)
@param end End index for substring (excluded)
@return Pointer to a new string
*/
char* substring(char* source, int begin, int end) {
    int substring_size = end - begin;
    if(source == NULL) return NULL;
    
    if(substring_size <= 0 || strlen(source) < end) {
        fprintf(stderr, "ERROR: Invalid intexes for substring extraction\n");
        return NULL;
    }

    char* substr = malloc(substring_size);

    if(substr == NULL) {
        fprintf(stderr, "FATAL: could not allocate memory for substring extraction\n");
        exit(EXIT_FAILURE);
    }

    strncpy(substr, source + begin, substring_size);
}

/*
Check if a character represents a digit
@param c The character in question
@return 1 if the character is a digit and 0 otherwise
*/
int is_digit(char c) {
    return '0' <= c && c <= '9';
}

/*
Check if character is a leter from the English alphabet or '_'
@param c The character in question
@return 1 if the character is alpha and 0 otherwise
*/
int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

/*
Check if character is a letter from the English alphabet or '_' or a digit
@param c The character in question
@return 1 if the character is alphanumeric and 0 otherwise
*/
int is_alphanumeric(char c) {
    return is_alpha(c) || is_digit(c); 
}

/*
Convert a string into a double
@param Source string
@param result Pointer to the result
@return 1 if parsing was successful and 0 otherwised
*/
int parse_double(char* source, double* result) {
    if(source == NULL) return 0;
    
    // Check the sign of the number 
    int has_sign = source[0] == '-' ? 1 : 0;
    int index = has_sign ? 1 : 0;
    int sign = has_sign ? -1 : 1;
    
    double integer = 0;
    double floating = 0;
    size_t length = strlen(source);

    while(index < length && is_digit(index)) {
        char c = source[index];
        if(c == '.')
            break;
        integer = integer * 10 + (int)c;
        index++;
    }
    int precision = 1;
    while(index < length && is_digit(index)) {
        char c = source[index];
        floating = floating + precision * (int)c;
        precision *= 10;
        index++;
    }
    *result = (integer + floating) * sign;
    // Return boolean denoting if string was parsed to the end
    return index == length;
}
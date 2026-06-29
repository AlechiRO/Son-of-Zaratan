#include <stdio.h>
#include "token.h"
#include <stdlib.h>

token_s* initialize_token(token_type_e type, char* lexeme, void* literal, int line) {
    token_s* token = malloc(sizeof(token_s));
    if(token == NULL)
        return NULL;
    token->type = type;
    token->lexeme = lexeme;
    token->literal = literal;
    token->line = line;
    return token;
}

void destroy_token(token_s** token) {
    if((*token) == NULL || token == NULL) {
        printf("Invalid token pointer!\n");
        return;
    }
    printf("Token %p was destroyed!\n", *token);
    free((*token));
}
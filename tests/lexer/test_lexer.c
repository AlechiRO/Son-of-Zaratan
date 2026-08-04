#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include "lexer_util.h"
#include "lexer.h"
#include "string_util.h"
#include "token.h"


// Global stack struct pointer
line_s* line;
lexer_context_s* lctx;
string_token_type_hashmap* token_map;

/*
Helper function to free the memory for the dependencies
*/
static void clean_up(void) {
    destroy_lexer_context(&lctx);
    string_token_type_hashmap_destroy(&token_map);
    
}

/*
Helper function to initialize the dependencies
*/
static void set_up(void) {
    lctx = initialize_lexer_context();
    token_map = string_token_type_hashmap_initialize();
    load_keywords(token_map);
}

/*
Helper function to set up the source string
@param val String containing source value
*/
static void set_source(char* val) {
    size_t length = strlen(val);
    lctx->source = malloc(length);
    strcpy(lctx->source, val);
    lctx->source_length = length;
}

/* 
Helper function to create a suite
@param name Pointer to the name of the suite
@return CUnit suite object
*/
static CU_pSuite create_suite(const char* name,void(*set_up)(), void(*tear)()) {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown(name, NULL, NULL, set_up, tear); 
    if (CU_get_error() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    return suite;
}

void test_number_integer(void) {
    set_source("456");
    number(lctx);
    token_s* token = token_list_get(lctx->tokens, 0);
    CU_ASSERT_TRUE(strcmp(token->lexeme, "456") == 0);
    CU_ASSERT_EQUAL(token->line, 1);
    CU_ASSERT_EQUAL(token->type, TOKEN_NUMBER);
    CU_ASSERT_EQUAL(token->literal->type, LITERAL_DOUBLE);
    CU_ASSERT_EQUAL(token->literal->value.double_value, 456);
}

void test_number_double_left_dot(void) {
    set_source("4.56");
    number(lctx);
    token_s* token = token_list_get(lctx->tokens, 0);
    CU_ASSERT_TRUE(strcmp(token->lexeme, "4.56") == 0);
    CU_ASSERT_EQUAL(token->line, 1);
    CU_ASSERT_EQUAL(token->type, TOKEN_NUMBER);
    CU_ASSERT_EQUAL(token->literal->type, LITERAL_DOUBLE);
    CU_ASSERT_DOUBLE_EQUAL(token->literal->value.double_value, 4.56, 0.0001);
}

void test_number_double_right_dot(void) {
    set_source("45.6");
    number(lctx);
    token_s* token = token_list_get(lctx->tokens, 0);
    CU_ASSERT_TRUE(strcmp(token->lexeme, "45.6") == 0);
    CU_ASSERT_EQUAL(token->line, 1);
    CU_ASSERT_EQUAL(token->type, TOKEN_NUMBER);
    CU_ASSERT_EQUAL(token->literal->type, LITERAL_DOUBLE);
    CU_ASSERT_DOUBLE_EQUAL(token->literal->value.double_value, 45.6, 0.0001);
}

void test_string_glob(void) {
    set_source("\"*.txt\"");
    string(lctx, 1);
    token_s* token = token_list_get(lctx->tokens, 0);
    CU_ASSERT_TRUE(strcmp(token->lexeme, "\"*.txt\"") == 0);
    CU_ASSERT_TRUE(strcmp(token->literal->value.string_value, "*.txt") == 0);
    CU_ASSERT_EQUAL(token->line, 1);
    CU_ASSERT_EQUAL(token->type, TOKEN_STRING_GLOB);
    CU_ASSERT_EQUAL(token->literal->type, LITERAL_STRING);
}

void test_string_default(void) {
    set_source("\'*.txt\'");
    string(lctx, 0);
    token_s* token = token_list_get(lctx->tokens, 0);
    CU_ASSERT_TRUE(strcmp(token->lexeme, "\'*.txt\'") == 0);
    CU_ASSERT_TRUE(strcmp(token->literal->value.string_value, "*.txt") == 0);
    CU_ASSERT_EQUAL(token->line, 1);
    CU_ASSERT_EQUAL(token->type, TOKEN_STRING_DEFAULT);
    CU_ASSERT_EQUAL(token->literal->type, LITERAL_STRING);
}

void test_string_unterminated(void) {
    set_source("\"*.txt");
    string(lctx, 1);
    CU_ASSERT_EQUAL(token_list_get_size(lctx->tokens), 0);
}

void test_indentifier_not_keyword(void) {
    set_source("needle");
    identifier(lctx, token_map);
    token_s* token = token_list_get(lctx->tokens, 0);
    CU_ASSERT_TRUE(strcmp(token->lexeme, "needle") == 0);
    CU_ASSERT_EQUAL(token->type, TOKEN_IDENTIFIER);
}

void test_identifier_keyword(void) {
    set_source("if(1 == 2)");
    identifier(lctx, token_map);
    token_s* token = token_list_get(lctx->tokens, 0);
    CU_ASSERT_TRUE(strcmp(token->lexeme, "if") == 0);
    CU_ASSERT_EQUAL(token->type, TOKEN_IF);
}


int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");

    /* Number suite */
    CU_pSuite number_suite = create_suite("number suite", set_up, clean_up);
    CU_add_test(number_suite, "number integer", test_number_integer);
    CU_add_test(number_suite, "number double left dot", test_number_double_left_dot);
    CU_add_test(number_suite, "number double right dot", test_number_double_right_dot);

    /* String suite */
    CU_pSuite string_suite = create_suite("string_suite", set_up, clean_up);
    CU_add_test(string_suite, "string glob", test_string_glob);
    CU_add_test(string_suite, "string default", test_string_default);
    CU_add_test(string_suite, "string unterminated", test_string_unterminated);

    /* Identifier suite */
    CU_pSuite identifier_suite = create_suite("identifier suite", set_up, clean_up);
    CU_add_test(identifier_suite, "identifier not a keyword", test_indentifier_not_keyword);
    CU_add_test(identifier_suite, "identifier keyword", test_identifier_keyword);
    
    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
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
Helper function to check if the right token was added to the token list
@param lctx Pointer to lexer context struct
@param index The index of the token in the list
@param type The type of the token
@param lexeme The lexeme of the token
*/
static void check_token(lexer_context_s* lctx, int index, token_type_e type, char* lexeme) {
    token_s* token = token_list_get(lctx->tokens, index);
    CU_ASSERT_EQUAL(token->type, type);
    CU_ASSERT_TRUE(strcmp(token->lexeme, lexeme) == 0);
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
    check_token(lctx, 0, TOKEN_IDENTIFIER, "needle");
}

void test_identifier_keyword(void) {
    set_source("if(1 == 2)");
    identifier(lctx, token_map);
    check_token(lctx, 0, TOKEN_IF, "if");
}

void test_scan_single_char_token(void) {
    set_source("(");
    scan_token(lctx, token_map);
    check_token(lctx, 0, TOKEN_ROUND_BRACE_LEFT, "(");
}

void test_scan_double_char_token_default(void) {
    set_source("!");
    scan_token(lctx, token_map);
    check_token(lctx, 0, TOKEN_BANG, "!");
}

void test_scan_double_char_token(void) {
    set_source("!=");
    scan_token(lctx, token_map);
    check_token(lctx, 0, TOKEN_BANG_EQUAL, "!=");
}

void test_scan_double_char_multiple_options_default(void) {
    set_source("<");
    scan_token(lctx, token_map);
    check_token(lctx, 0, TOKEN_LESS, "<");
}

void test_scan_double_char_multiple_options(void) {
    set_source("<&");
    scan_token(lctx, token_map);
    check_token(lctx, 0, TOKEN_DUP_IN, "<&");
}

void test_scan_comment(void) {
    set_source("~This is a comment");
    scan_token(lctx, token_map);
    CU_ASSERT_TRUE(token_list_is_empty(lctx->tokens));
}

void test_scan_terminator(void) {
    set_source("\n");
    scan_token(lctx, token_map);
    check_token(lctx, 0, TOKEN_TERMINATOR, "\n");
}

void test_scan_string_default(void) {
    set_source("\'default\'");
    scan_token(lctx, token_map);
    check_token(lctx, 0, TOKEN_STRING_DEFAULT, "\'default\'");
}

void test_scan_string_glob(void) {
    set_source("\"glob\"");
    scan_token(lctx, token_map);
    check_token(lctx, 0, TOKEN_STRING_GLOB, "\"glob\"");
}

void test_scan_number(void) {
    set_source("452.33");
    scan_token(lctx, token_map);
    check_token(lctx, 0, TOKEN_NUMBER, "452.33");
}

void test_scan_identifier(void) {
    set_source("identifier");
    scan_token(lctx, token_map);
    check_token(lctx, 0, TOKEN_IDENTIFIER, "identifier");
}

void test_scan_keyword(void) {
    set_source("return");
    scan_token(lctx, token_map);
    check_token(lctx, 0, TOKEN_RETURN, "return");
}

void test_scan_unexpected_char(void) {
    set_source("^");
    scan_token(lctx, token_map);
    CU_ASSERT_TRUE(token_list_is_empty(lctx->tokens));
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
    
    /* Scan_token suite */
    CU_pSuite scan_token_suite = create_suite("scan_token suite", set_up, clean_up);
    CU_add_test(scan_token_suite, "scan single char token", test_scan_single_char_token);
    CU_add_test(scan_token_suite, "scan double char token default", test_scan_double_char_token_default);
    CU_add_test(scan_token_suite, "scan double character token", test_scan_double_char_token);
    CU_add_test(scan_token_suite, "scan double char multiple options defaul", test_scan_double_char_multiple_options_default);
    CU_add_test(scan_token_suite, "scan double char tokens multiple options", test_scan_double_char_multiple_options);
    CU_add_test(scan_token_suite, "scan comment", test_scan_comment);
    CU_add_test(scan_token_suite, "scan terminator", test_scan_terminator);
    CU_add_test(scan_token_suite, "scan string default", test_scan_string_default);
    CU_add_test(scan_token_suite, "scan string glob", test_scan_string_glob);
    CU_add_test(scan_token_suite, "scan number", test_scan_number);
    CU_add_test(scan_token_suite, "scan identifier", test_scan_identifier);
    CU_add_test(scan_token_suite, "scan keyword", test_scan_keyword);
    CU_add_test(scan_token_suite, "scan unexpected char", test_scan_unexpected_char);
    

    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
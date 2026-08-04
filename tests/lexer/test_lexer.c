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

/*
Helper function to free the memory for the dependencies
*/
static void clean_up(void) {
    destroy_lexer_context(&lctx);
}

/*
Helper function to initialize the dependencies
*/
static void set_up(void) {
    lctx = initialize_lexer_context();
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


int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");

    /* Number suite */
    CU_pSuite number_suite = create_suite("number suite", set_up, clean_up);
    CU_add_test(number_suite, "number integer", test_number_integer);

    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
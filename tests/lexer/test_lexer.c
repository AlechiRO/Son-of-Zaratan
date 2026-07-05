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


void test_initialize_lexer_context(void) {
    lctx = initialize_lexer_context();
    CU_ASSERT_PTR_NULL(lctx->source);
    CU_ASSERT_PTR_NOT_NULL(lctx->tokens);
    CU_ASSERT_EQUAL(lctx->start, 0);
    CU_ASSERT_EQUAL(lctx->current, 0);
    CU_ASSERT_EQUAL(lctx->line_number, 1);
    CU_ASSERT_EQUAL(lctx->source_length, 0);
}

void test_destroy_lexer_context(void) {
    lctx->source = malloc(sizeof("Iron Islands"));
    strcpy(lctx->source, "Iron Islands");
    destroy_lexer_context(&lctx);
    CU_ASSERT_PTR_NULL(lctx);
}

void test_is_at_end(void) {
    lctx->current = 10;
    lctx->source_length = 11;
    CU_ASSERT_FALSE(is_at_end(lctx));
}

void test_is_not_at_end(void) {
    lctx->current = 5;
    lctx->source_length = 5;
    CU_ASSERT_TRUE(is_at_end(lctx));
}


int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");

    /* Lexer context initialize suite */
    CU_pSuite lexer_context_initialize_suite = create_suite("lexer context initialize suite", NULL, clean_up);
    CU_add_test(lexer_context_initialize_suite, "initialize lexer context", test_initialize_lexer_context);
    
    /* Lexer context destroy suite */
    CU_pSuite lexer_context_destroy_suite = create_suite("lexer context destroy suite", set_up, NULL);
    CU_add_test(lexer_context_destroy_suite, "destroy lexer context", test_destroy_lexer_context);

    /* Is at end suite */
    CU_pSuite is_at_end_suite = create_suite("is at end suite", set_up, clean_up);
    CU_add_test(is_at_end_suite, "is at end", test_is_at_end);
    CU_add_test(is_at_end_suite, "is not at end", test_is_not_at_end);
    
    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
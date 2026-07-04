#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <err.h>

#define STACK_TAG string_stack
#define STACK_ITEM_TYPE char*
#include "stack.h"


// Global stack struct pointer
string_stack* stack;

/*
Helper function to free the memory for the stack
*/
static void clean_up(void) {
    string_stack_destroy(&stack);
}

/*
Helper function to initialize the stack
*/
static void set_up(void) {
    stack = string_stack_initialize();
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

void test_initialize(void) {
    stack = string_stack_initialize();
    CU_ASSERT_EQUAL(stack->size, sizeof(char*)*10);
    CU_ASSERT_EQUAL(stack->top, -1);
}

void test_is_empty(void) {
    CU_ASSERT_TRUE(string_stack_is_empty(stack));
}

void test_is_empty_push(void) {
    string_stack_push(stack, "obj");
    CU_ASSERT_FALSE(string_stack_is_empty(stack));
}

void test_size_3(void) {
    string_stack_push(stack, "obj");
    string_stack_push(stack, "obj");
    string_stack_push(stack, "obj");
    CU_ASSERT_EQUAL(string_stack_size(stack), 3);
}

void test_peek_empty(void) {
    CU_ASSERT_PTR_NULL(string_stack_peek(stack));
}

void test_peek_element(void) {
    char* word = "the hound";
    string_stack_push(stack, word);
    CU_ASSERT_EQUAL(word, string_stack_peek(stack));
}

void test_push(void) {
    string_stack_push(stack, "arya");
    CU_ASSERT_FALSE(string_stack_is_empty(stack));
}

void test_push_NULL(void) {
    string_stack_push(stack, NULL);
    CU_ASSERT_FALSE(string_stack_is_empty(stack));
}

void test_pop_empty(void) {
    CU_ASSERT_PTR_NULL(string_stack_pop(stack));
}

void test_pop_two_elements(void) {
    string_stack_push(stack, "baelish");
    string_stack_push(stack, "sansa");
    CU_ASSERT_EQUAL(string_stack_pop(stack), "sansa");
    CU_ASSERT_EQUAL(string_stack_pop(stack), "baelish");
    CU_ASSERT_TRUE(string_stack_is_empty(stack));
}

void test_destroy_empty_stack(void) {
    string_stack_destroy(&stack);
    CU_ASSERT_PTR_NULL(stack);
}




int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");
    /* Initialize suite */
    CU_pSuite initialize_suite = create_suite("initialize suite", NULL, clean_up);
    CU_add_test(initialize_suite, "initialize", test_initialize);

    /* Empty suite */
    CU_pSuite is_empty_suite = create_suite("is empty suite", set_up, clean_up);
    CU_add_test(is_empty_suite, "is empty", test_is_empty);
    CU_add_test(is_empty_suite, "is empty push", test_is_empty_push);

    /* Size suite */
    CU_pSuite size_suite = create_suite("size suite", set_up, clean_up);
    CU_add_test(size_suite, "size 3", test_size_3);

    /* Peek suite */
    CU_pSuite peek_suite = create_suite("peek suite", set_up, clean_up);
    CU_add_test(peek_suite, "peek empty", test_peek_empty);
    CU_add_test(peek_suite, "peek element", test_peek_element);

    /* Push suite */
    CU_pSuite push_suite = create_suite("push suite", set_up, clean_up);
    CU_add_test(push_suite, "push", test_push);
    CU_add_test(push_suite, "push NULL", test_push_NULL);

    /* Pop suite */
    CU_pSuite pop_suite = create_suite("pop suite", set_up, clean_up);
    CU_add_test(pop_suite, "pop empty", test_pop_empty);
    CU_add_test(pop_suite, "pop two elements", test_pop_two_elements);

    /* Destroy suite */
    CU_pSuite destroy_suite = create_suite("destroy suite", set_up, NULL);
    CU_add_test(destroy_suite, "destroy empty stack", test_destroy_empty_stack);
    
    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
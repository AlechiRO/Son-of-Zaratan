#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <err.h>
#include "stack.h"


// Global stack struct pointer
stack_s* stack;

/*
Helper function to free the memory for the stack
*/
static void clean_up() {
    destroy_stack(&stack);
}
/* 
Helper function to create a suite
@param name Pointer to the name of the suite
@return CUnit suite object
*/
static CU_pSuite create_suite(const char* name, void(*tear)()) {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown(name, NULL, NULL, NULL, tear); 
    if (CU_get_error() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    return suite;
}

void test_initialize(void) {
    stack = initialize_stack();
    CU_ASSERT_EQUAL(stack->size, sizeof(void**)*100);
    CU_ASSERT_EQUAL(stack->top, -1);
}

void test_is_empty(void) {
    stack = initialize_stack();
    CU_ASSERT_TRUE(is_empty_stack(stack));
}

void test_is_empty_push(void) {
    stack = initialize_stack();
    push_stack(stack, "obj");
    CU_ASSERT_FALSE(is_empty_stack(stack));
}

void test_size_2(void) {
    stack = initialize_stack();
    push_stack(stack, "obj");
    push_stack(stack, "obj");
    push_stack(stack, "obj");
    CU_ASSERT_EQUAL(size_stack(stack), 2);
}

void test_peek_empty(void) {
    stack = initialize_stack();
    CU_ASSERT_PTR_NULL(peek_stack(stack));
}

void test_peek_element(void) {
    stack = initialize_stack();
    char* word = "the hound";
    push_stack(stack, word);
    CU_ASSERT_EQUAL(word, peek_stack(stack));
}

void test_push(void) {
    stack = initialize_stack();
    push_stack(stack, "arya");
    CU_ASSERT_FALSE(is_empty_stack(stack));
}

void test_push_NULL(void) {
    stack = initialize_stack(); 
    push_stack(stack, NULL);
    CU_ASSERT_FALSE(is_empty_stack(stack));
}

void test_pop_empty(void) {
    stack = initialize_stack();
    CU_ASSERT_PTR_NULL(pop_stack(stack));
}

void test_pop_two_elements(void) {
    stack = initialize_stack();
    push_stack(stack, "baelish");
    push_stack(stack, "sansa");
    CU_ASSERT_EQUAL(pop_stack(stack), "sansa");
    CU_ASSERT_EQUAL(pop_stack(stack), "baelish");
    CU_ASSERT_TRUE(is_empty_stack(stack));
}

void test_destroy_empty_stack(void) {
    stack = initialize_stack();
    destroy_stack(&stack);
    CU_ASSERT_PTR_NULL(stack);
}




int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");
    /* Initialize suite */
    CU_pSuite initialize_suite = create_suite("initialize suite", clean_up);
    CU_add_test(initialize_suite, "initialize", test_initialize);

    /* Empty suite */
    CU_pSuite is_empty_suite = create_suite("is empty suite", clean_up);
    CU_add_test(is_empty_suite, "is_empty", test_is_empty);
    CU_add_test(is_empty_suite, "is_empty_push", test_is_empty_push);

    /* Size suite */
    CU_pSuite size_suite = create_suite("size suite", clean_up);
    CU_add_test(size_suite, "size_3", test_size_2);

    /* Peek suite */
    CU_pSuite peek_suite = create_suite("peek suite", clean_up);
    CU_add_test(peek_suite, "peek_empty", test_peek_empty);
    CU_add_test(peek_suite, "peek_element", test_peek_element);

    /* Push suite */
    CU_pSuite push_suite = create_suite("push suite", clean_up);
    CU_add_test(push_suite, "push", test_push);
    CU_add_test(push_suite, "push_NULL", test_push_NULL);

    /* Pop suite */
    CU_pSuite pop_suite = create_suite("pop suite", clean_up);
    CU_add_test(pop_suite, "pop_empty", test_pop_empty);
    CU_add_test(pop_suite, "pop_two_elements", test_pop_two_elements);

    /* Destroy suite */
    CU_pSuite destroy_suite = create_suite("destroy suite", NULL);
    CU_add_test(destroy_suite, "destroy_empty_stack", test_destroy_empty_stack);
    
    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
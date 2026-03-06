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
    destroy_stack(stack);
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


int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");
    /* Initialize stack suite */
    CU_pSuite initialize_suite = create_suite("initialize suite", clean_up);
    CU_add_test(initialize_suite, "initialize", test_initialize);

    CU_pSuite is_empty_suite = create_suite("is empty suite", clean_up);
    CU_add_test(is_empty_suite, "is empty", test_is_empty);

    
    // run the tests
    CU_basic_run_tests();

    // clean the registry
    CU_cleanup_registry();
    return 0;
}
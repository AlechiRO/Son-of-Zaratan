#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <err.h>
#include "stack.h"
#include "stack_test.h"

// Global stack struct pointer
stack_s *stack;

/*
Helper function to free the memory for the stack
*/
static void clean_up() {
    destroy(stack);
}
/* 
Helper function to create a suite
@param name Pointer to the name of the suite
@return CUnit suite object
*/
static CU_pSuite create_suite(const char* name) {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown(name, NULL, NULL, NULL, clean_up); 
    if (CU_get_error() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    return suite;
}

void test_initialize(void) {
    initialize(stack);
    CU_ASSERT_EQUAL(sizeof(stack->array), sizeof(void**)*100);
    CU_ASSERT_EQUAL(stack->top, -1);
}


int test_stack(int argc, char *argv[]) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");

    CU_pSuite stack_suite = create_suite("stack suite");
    CU_add_test(stack_suite, "initialize", test_initialize);

    
    // run the tests
    CU_basic_run_tests();

    // clean the registry
    CU_cleanup_registry();
    return 0;
}
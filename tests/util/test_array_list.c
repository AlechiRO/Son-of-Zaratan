#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "array_list.h"

// Global dll struct pointer
array_list_s* list;

/*
Helper function to free the memory for the dll
*/
static void clean_up(void) {
    destroy_array_list(&list);
}
/*
Helper function to initialize the dll
*/
static void set_up(void) {
    list = initialize_array_list();
}

/* 
Helper function to create a suite
@param name Pointer to the name of the suite
@return CUnit suite object
*/
static CU_pSuite create_suite(const char* name,  void(*set_up)(),  void(*tear)()) {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown(name, NULL, NULL, set_up, tear); 
    if (CU_get_error() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    return suite;
}

void test_initialize_array_list(void) {
    
}


int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");

    CU_pSuite initialize_array_list_suite = create_suite("initialize array list suite", NULL, clean_up);
    CU_add_test(initialize_array_list_suite, "initialize array list", test_initialize_array_list);

    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
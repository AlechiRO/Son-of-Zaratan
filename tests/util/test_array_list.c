#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#define ARRAY_LIST_ITEM_TYPE int
#define ARRAY_LIST_TAG int_list
#include "array_list.h"




int_list* list;

/*
Helper function to free the memory for the Array List
*/
static void clean_up(void) {
    int_list_destroy(&list);
}
/*
Helper function to initialize the Array List
*/
static void set_up(void) {
    list = int_list_initialize();
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

void test_initialize(void) {
    list = int_list_initialize();
    CU_ASSERT_PTR_NOT_NULL(list);
    CU_ASSERT_PTR_NOT_NULL(list->array);
    CU_ASSERT_EQUAL(list->size, 0);
    CU_ASSERT_EQUAL(list->capacity, 10);
}

void test_is_empty(void) {
    CU_ASSERT_TRUE(int_list_is_empty(list));
}




int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");

    // initialize suite
    CU_pSuite initialize_suite = create_suite("initialize suite", NULL, clean_up);
    CU_add_test(initialize_suite, "initialize", test_initialize);

    // is_empty suite
    CU_pSuite is_empty_suite = create_suite("is empty suite", set_up, clean_up);
    CU_add_test(is_empty_suite, "is empty", test_is_empty);

    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
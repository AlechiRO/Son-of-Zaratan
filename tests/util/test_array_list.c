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

void test_is_not_empty(void) {
    int_list_add(list, 1);
    CU_ASSERT_FALSE(int_list_is_empty(list));
}

void test_get_size_zero_elements(void) {
    CU_ASSERT_EQUAL(int_list_get_size(list), 0);
}

void test_get_size_one_element(void) {
    int_list_add(list, 1);
    CU_ASSERT_EQUAL(int_list_get_size(list), 1);
}

void test_get_size_two_elements(void) {
    int_list_add(list, 1);
    int_list_add(list, 2);
    CU_ASSERT_EQUAL(int_list_get_size(list), 2);
}

void test_add_at_index_zero(void) {
    int_list_add_at_index(list, 0, 1);
    CU_ASSERT_EQUAL(int_list_get(list, 0), 1);
}

void test_add_at_index_one_on_position_zero(void) {
    int_list_add_at_index(list, 1, 1);
    CU_ASSERT_EQUAL(int_list_get(list, 0), 1);
}

void test_add_at_index_between(void) {
    int_list_add(list, 1);
    int_list_add(list, 3);
    int_list_add_at_index(list, 1, 2);
    CU_ASSERT_EQUAL(int_list_get(list, 1), 2);
    CU_ASSERT_EQUAL(int_list_get(list, 0), 1);
    CU_ASSERT_EQUAL(int_list_get(list, 2), 3);
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
    CU_add_test(is_empty_suite, "is not empty", test_is_not_empty);

    // get_size suite
    CU_pSuite get_size_suite = create_suite("get size suite", set_up, clean_up);
    CU_add_test(get_size_suite, "get size zero elements", test_get_size_zero_elements);
    CU_add_test(get_size_suite, "get size one element", test_get_size_one_element);

    // add_at_index suite
    CU_pSuite add_at_index_suite = create_suite("add at index suite", set_up, clean_up);
    CU_add_test(add_at_index_suite, "add at index 0", test_add_at_index_zero);
    CU_add_test(add_at_index_suite, "add at index 1 on position 0", test_add_at_index_one_on_position_zero);
    CU_add_test(add_at_index_suite, "add at index 1 between 2 elements", test_add_at_index_between);
    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
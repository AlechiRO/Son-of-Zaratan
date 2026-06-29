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

void test_add_one_element(void) {
    int_list_add(list, 1);
    CU_ASSERT_EQUAL(int_list_get(list, 0), 1);
}

void test_add_two_elements(void) {
    int_list_add(list, 1);
    int_list_add(list, 2);
    CU_ASSERT_EQUAL(int_list_get(list, 0), 1);
    CU_ASSERT_EQUAL(int_list_get(list, 1), 2);
}
// triggers the increase capacity function 
void test_add_eleven_elements(void) {
    for(int i = 0; i < 11; i++)
        int_list_add(list, i);
    CU_ASSERT_EQUAL(int_list_get(list, 10), 10);
}

void test_add_first_one_element(void) {
    int_list_add_first(list, 1);
    CU_ASSERT_EQUAL(int_list_get(list, 0), 1);
}

void test_add_first_non_empty_list(void) {
    int_list_add(list, 1);
    int_list_add_first(list, 0);
    CU_ASSERT_EQUAL(int_list_get(list, 0), 0);
    CU_ASSERT_EQUAL(int_list_get(list, 1), 1);
}

void test_set_one_element(void) {
    int_list_add(list, 1);
    int_list_set(list, 0, 0);
    CU_ASSERT_EQUAL(int_list_get(list, 0), 0);
}

void test_set_two_elements(void) {
    int_list_add(list, 1);
    int_list_add(list, 2);
    int_list_set(list, 0, 10);
    int_list_set(list, 1, 11);
    CU_ASSERT_EQUAL(int_list_get(list, 0), 10);
    CU_ASSERT_EQUAL(int_list_get(list, 1), 11);
}

void test_get_one_element(void) {
    int_list_add(list, 1);
    CU_ASSERT_EQUAL(int_list_get(list, 0), 1);
}

void test_get_two_elements(void) {
    int_list_add(list, 1);
    CU_ASSERT_EQUAL(int_list_get(list, 0), 1);
    int_list_add(list, 2);
    CU_ASSERT_EQUAL(int_list_get(list, 1), 2);
}

void test_remove_one_element(void) {
    int_list_add(list, 1);
    int removed = int_list_remove(list, 0);
    CU_ASSERT_EQUAL(removed, 1);
    CU_ASSERT_TRUE(int_list_is_empty(list));
}

void test_remove_first_element(void) {
    int_list_add(list, 1);
    int_list_add(list, 2);
    int removed = int_list_remove(list, 0);
    CU_ASSERT_EQUAL(removed, 1);
    CU_ASSERT_EQUAL(int_list_get(list, 0), 2);
}

void test_remove_two_elements(void) {
    int_list_add(list, 1);
    int_list_add(list, 2);
    int_list_add(list, 3);
    int removed1 = int_list_remove(list, 0);
    int removed2 = int_list_remove(list, 1);
    CU_ASSERT_EQUAL(removed1, 1);
    CU_ASSERT_EQUAL(removed2, 3);
    CU_ASSERT_EQUAL(int_list_get(list, 0), 2);
}

void test_destroy(void) {
    int_list_destroy(&list);
    CU_ASSERT_PTR_NULL(list);
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

    // add suite 
    CU_pSuite add_suite = create_suite("add suite", set_up, clean_up);
    CU_add_test(add_suite, "add one element", test_add_one_element);
    CU_add_test(add_suite, "add two elements", test_add_two_elements);
    CU_add_test(add_suite, "add 11 elements", test_add_eleven_elements);

    // add_first suite
    CU_pSuite add_first_suite = create_suite("add_first suite", set_up, clean_up);
    CU_add_test(add_first_suite, "add first one element", test_add_first_one_element);
    CU_add_test(add_first_suite, "add first non-empty list", test_add_first_non_empty_list);    

    // set suite
    CU_pSuite set_suite = create_suite("set suite", set_up, clean_up);
    CU_add_test(set_suite, "set one element", test_set_one_element);
    CU_add_test(set_suite, "set two elements", test_set_two_elements);

    // get suite
    CU_pSuite get_suite = create_suite("get suite", set_up, clean_up);
    CU_add_test(get_suite, "get one element", test_get_one_element);
    CU_add_test(get_suite, "get two elements", test_get_two_elements);

    // remove suite
    CU_pSuite remove_suite = create_suite("remove suite", set_up, clean_up);
    CU_add_test(remove_suite, "remove one element", test_remove_one_element);
    CU_add_test(remove_suite, "remove first element", test_remove_first_element);
    CU_add_test(remove_suite, "remove two elements", test_remove_two_elements);
    
    // destroy suite
    CU_pSuite destroy_suite = create_suite("destroy suite", set_up, NULL);
    CU_add_test(destroy_suite, "destroy", test_destroy);
    

    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#define DLL_ITEM_TYPE int
#define DLL_TAG int_dll
#include "dll.h"

// Global dll struct pointer
int_dll* dll;

/*
Helper function to free the memory for the dll
*/
static void clean_up(void) {
    int_dll_destroy(&dll);
}
/*
Helper function to initialize the dll
*/
static void set_up(void) {
    dll = int_dll_initialize();
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
    dll = int_dll_initialize();
    CU_ASSERT_PTR_NOT_NULL(dll);
    CU_ASSERT_PTR_NOT_NULL(dll->head);
    CU_ASSERT_PTR_NOT_NULL(dll->tail);
    CU_ASSERT_EQUAL(dll->size, 0);
}

void test_get_size(void) {
    CU_ASSERT_EQUAL(int_dll_get_size(dll), 0);
    int a = 1;
    int_dll_insert_first(dll, a);
    CU_ASSERT_EQUAL(int_dll_get_size(dll), 1);
}

void test_insert_between_head_tail(void) {
    int a = 10;
    int_dll_insert_between(dll, a, dll->head, dll->tail);
    int_dll_node* node = int_dll_get_next(dll->head);
    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node, int_dll_get_prev(dll->tail));
    CU_ASSERT_EQUAL(int_dll_get_payload(node), a);
}

void test_insert_between_3_nodes(void) {
    int a = 1;
    int b = 2;
    int c = 3;
    int_dll_insert_between(dll, a, dll->head, dll->tail);
    int_dll_insert_between(dll, b, dll->head->next, dll->tail);
    int_dll_insert_between(dll, c, dll->head->next->next, dll->tail);
    CU_ASSERT_PTR_NOT_NULL(dll->head->next);
    CU_ASSERT_PTR_NOT_NULL(dll->head->next->next);
    CU_ASSERT_PTR_NOT_NULL(dll->head->next->next->next);
    CU_ASSERT_EQUAL(int_dll_get_payload(dll->head->next), 1);
    CU_ASSERT_EQUAL(int_dll_get_payload(dll->head->next->next), 2);
    CU_ASSERT_EQUAL(int_dll_get_payload(dll->head->next->next->next), 3);
}

void test_insert_first_simple(void) {
    int a = 1;
    int_dll_insert_first(dll, a);
    CU_ASSERT_PTR_NOT_NULL(dll->head->next);
    CU_ASSERT_EQUAL(int_dll_get_payload(dll->head->next), 1);
}

void test_insert_first_2(void) {
    int a = 1;
    int b = 2;
    int_dll_insert_first(dll, a);
    int_dll_insert_first(dll, b);
    CU_ASSERT_PTR_NOT_NULL(dll->head->next);
    CU_ASSERT_PTR_NOT_NULL(dll->head->next->next);
    CU_ASSERT_EQUAL(int_dll_get_payload(dll->head->next), 2);
    CU_ASSERT_EQUAL(int_dll_get_payload(dll->head->next->next), 1);
}

void test_insert_last_2(void) {
    int a = 1;
    int b = 2;
    int_dll_insert_last(dll, a);
    int_dll_insert_last(dll, b);
    CU_ASSERT_PTR_NOT_NULL(dll->head->next);
    CU_ASSERT_PTR_NOT_NULL(dll->head->next->next);
    CU_ASSERT_EQUAL(int_dll_get_payload(dll->head->next), 1);
    CU_ASSERT_EQUAL(int_dll_get_payload(dll->head->next->next), 2);
}

void test_remove_node(void) {
    int a = 1;
    int b = 2;
    int c = 3;
    int_dll_insert_between(dll, a, dll->head, dll->tail);
    int_dll_insert_between(dll, b, dll->head->next, dll->tail);
    int_dll_insert_between(dll, c, dll->head->next->next, dll->tail);
    int_dll_remove_node(dll, dll->head->next);
    int_dll_remove_node(dll, dll->head->next->next);
    CU_ASSERT_EQUAL(int_dll_get_size(dll), 1);
    CU_ASSERT_EQUAL(int_dll_get_payload(dll->head->next),2);
}

void test_remove_first(void) {
    int a = 1;
    int b = 2;
    int_dll_insert_between(dll, a, dll->head, dll->tail);
    int_dll_insert_between(dll, b, dll->head->next, dll->tail);
    int_dll_remove_first(dll);
    CU_ASSERT_PTR_NOT_NULL(dll->head->next);
    CU_ASSERT_EQUAL(int_dll_get_payload(dll->head->next), 2);
}

void test_remove_last(void) {
    int a = 1;
    int b = 2;
    int_dll_insert_between(dll, a, dll->head, dll->tail);
    int_dll_insert_between(dll, b, dll->head->next, dll->tail);
    int_dll_remove_last(dll);
    CU_ASSERT_PTR_NOT_NULL(dll->head->next);
    CU_ASSERT_EQUAL(int_dll_get_payload(dll->head->next), 1);
}

void test_get_first(void) {
    int a = 1;
    int b = 2;
    int_dll_insert_between(dll, a, dll->head, dll->tail);
    int_dll_insert_between(dll, b, dll->head->next, dll->tail);
    CU_ASSERT_EQUAL(int_dll_get_first(dll), 1);
}

void test_get_last(void) {
    int a = 1;
    int b = 2;
    int_dll_insert_between(dll, a, dll->head, dll->tail);
    int_dll_insert_between(dll, b, dll->head->next, dll->tail);
    CU_ASSERT_EQUAL(int_dll_get_last(dll), 2);
}

void test_get_first_empty(void) {
    CU_ASSERT_EQUAL(int_dll_get_first(dll), 0);
}

void test_get_last_empty(void) {
    CU_ASSERT_EQUAL(int_dll_get_last(dll), 0);
}

void test_destroy(void) {
    int a = 1;
    int b = 2;
    int_dll_insert_between(dll, a, dll->head, dll->tail);
    int_dll_insert_between(dll, b, dll->head->next, dll->tail);
    int_dll_destroy(&dll);
    CU_ASSERT_PTR_NULL(dll);
}


int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");

    /* Initialize DLL suite */
    CU_pSuite initialize_suite = create_suite("initialize dll suite", NULL, clean_up);
    CU_add_test(initialize_suite, "initialize dll", test_initialize);

    /* Get Size suite */
    CU_pSuite get_size_suite = create_suite("get size suite", set_up, clean_up);
    CU_add_test(get_size_suite, "get size", test_get_size);

    /* Insert Between suite */
    CU_pSuite insert_between_suite = create_suite("insert between dll suite", set_up, clean_up);
    CU_add_test(insert_between_suite, "insert between head tail", test_insert_between_head_tail);
    CU_add_test(insert_between_suite, "insert between 3 nodes", test_insert_between_3_nodes);

    /* Insert First suite */
    CU_pSuite insert_first_suite = create_suite("insert first suite", set_up, clean_up);
    CU_add_test(insert_first_suite, "insert first simple", test_insert_first_simple);
    CU_add_test(insert_first_suite, "insert first 2", test_insert_first_2);
    
    /* Insert Last suite */
    CU_pSuite insert_last_suite = create_suite("insert last suite", set_up, clean_up);
    CU_add_test(insert_last_suite, "insert last 2", test_insert_last_2);

    /* Remove Node suite */
    CU_pSuite remove_DLL_node_suite = create_suite("remove node suite", set_up, clean_up);
    CU_add_test(remove_DLL_node_suite, "remove node", test_remove_node);
    
    /* Remove First suite */
    CU_pSuite remove_first_suite = create_suite("remove first suite", set_up, clean_up);
    CU_add_test(remove_first_suite, "remove first", test_remove_first);

    /* Remove Last suite */
    CU_pSuite remove_last_suite = create_suite("remove last suite", set_up, clean_up);
    CU_add_test(remove_last_suite, "remove last", test_remove_last);

    /* Get First suite */
    CU_pSuite get_first_suite = create_suite("get first suite", set_up, clean_up);
    CU_add_test(get_first_suite, "get first", test_get_first);
    CU_add_test(get_first_suite, "get first empty", test_get_first_empty);
    
    /* Get Last suite */
    CU_pSuite get_last_suite = create_suite("get last suite", set_up, clean_up);
    CU_add_test(get_last_suite, "get last", test_get_last);
    CU_add_test(get_last_suite, "get last empty", test_get_last_empty);
    

    /* Destroy DLL suite */
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
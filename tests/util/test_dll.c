#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "dll.h"

// Global dll struct pointer
dll_s* dll;

/*
Helper function to free the memory for the dll
*/
static void clean_up() {
    destroy_dll(&dll);
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

void test_initialize_dll(void) {
    dll = initialize_dll();
    CU_ASSERT_PTR_NOT_NULL(dll);
    CU_ASSERT_PTR_NOT_NULL(dll->head);
    CU_ASSERT_PTR_NOT_NULL(dll->tail);
    CU_ASSERT_EQUAL(dll->size, 0);
}

void test_insert_between_head_tail(void) {
    dll = initialize_dll();
    char* word = malloc(6);
    strcpy(word, "Aaron");
    insert_between_dll(dll, word, dll->head, dll->tail);
    node_s* node = get_next_dll(dll->head);
    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node, get_prev_dll(dll->tail));
    CU_ASSERT_EQUAL(get_payload_dll(node), word);
}

int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");

    /* Initialize DLL suite */
    CU_pSuite initialize_dll_suite = create_suite("initialize dll suite", clean_up);
    CU_add_test(initialize_dll_suite, "initialize dll", test_initialize_dll);

    /* Insert Between suite */
    CU_pSuite insert_between_dll_suite = create_suite("insert between dll suite", clean_up);
    CU_add_test(insert_between_dll_suite, "insert between head tail", test_insert_between_head_tail);
    
    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <err.h>
#include <string.h>
#include "string_util.h"


char* string;

/*
Helper function to free the memory for the Token
*/
static void clean_up(void) {
    //...implementation if needed
}
/*
Helper function to initialize the Token
*/
static void set_up(void) {
    free(string);
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

void test_substring_null(void) {
    string = substring(NULL, 0, 10);
    CU_ASSERT_PTR_NULL(string);
}

void test_substring_invalid_indexes_on_point(void) {
    string = substring("Helena knows he is scared", 7, 7);
    CU_ASSERT_PTR_NULL(string);
}

void test_substring_invalid_indexes(void) {
    string = substring("Syrax will not be injured", 7, 2);
    CU_ASSERT_PTR_NULL(string);
}

void test_substring(void) {
    string = substring("Daemon sacrificed himself", 7, 17);
    CU_ASSERT_TRUE(strcmp(string, "sacrificed") == 0);
    printf("%s/n", string);
}
int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry"); 

    CU_pSuite substring_suite = create_suite("substring suite", NULL, clean_up);
    CU_add_test(substring_suite, "substring null", test_substring_null);
    CU_add_test(substring_suite, "substring invalid indexes on point", test_substring_invalid_indexes_on_point);
    CU_add_test(substring_suite, "substring invalid indexes", test_substring_invalid_indexes);
    CU_add_test(substring_suite, "substring valid index", test_substring);

    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
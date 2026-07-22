#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <err.h>
#include <string.h>
#include "string_util.h"


char* string = NULL;

/*
Helper function to free the memory for the Token
*/
static void clean_up(void) {
    if(string != NULL)
        free(string);
}
/*
Helper function to initialize the Token
*/
static void set_up(void) {
    // Implementation if needed...
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

void test_is_digit(void) {
    CU_ASSERT_TRUE(is_digit('0'));
    CU_ASSERT_TRUE(is_digit('9'));
    CU_ASSERT_TRUE(is_digit('5'));
}

void test_is_not_digit(void) {
    CU_ASSERT_FALSE(is_digit('\0'));
    CU_ASSERT_FALSE(is_digit('!'));
    CU_ASSERT_FALSE(is_digit('v'));
}

void test_is_alpha(void) {
    CU_ASSERT_TRUE(is_alpha('_'));
    CU_ASSERT_TRUE(is_alpha('a'));
    CU_ASSERT_TRUE(is_alpha('z'));
    CU_ASSERT_TRUE(is_alpha('A'));
    CU_ASSERT_TRUE(is_alpha('Z'));
    CU_ASSERT_TRUE(is_alpha('J'));
}

void test_is_not_alpha(void) {
    CU_ASSERT_FALSE(is_alpha('0'));
    CU_ASSERT_FALSE(is_alpha('9'));
    CU_ASSERT_FALSE(is_alpha('$'));
    CU_ASSERT_FALSE(is_alpha('-'));
}

void test_is_alphanumeric(void) {
    CU_ASSERT_TRUE(is_alphanumeric('a'));
    CU_ASSERT_TRUE(is_alphanumeric('A'));
    CU_ASSERT_TRUE(is_alphanumeric('z'));
    CU_ASSERT_TRUE(is_alphanumeric('Z'));
    CU_ASSERT_TRUE(is_alphanumeric('_'));
    CU_ASSERT_TRUE(is_alphanumeric('0'));
    CU_ASSERT_TRUE(is_alphanumeric('9'));
}

void test_is_not_alphanumeric(void) {
    CU_ASSERT_FALSE(is_alphanumeric('-'));
    CU_ASSERT_FALSE(is_alphanumeric(')'));
    CU_ASSERT_FALSE(is_alphanumeric('('));
    CU_ASSERT_FALSE(is_alphanumeric('\''));
    CU_ASSERT_FALSE(is_alphanumeric('@'));
    CU_ASSERT_FALSE(is_alphanumeric('~'));
}

void test_parse_double_valid(void) {
    double result;
    int success = parse_double("1.001", &result);
    CU_ASSERT_TRUE(success);
}

int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry"); 
    /* Substring suite */
    CU_pSuite substring_suite = create_suite("substring suite", NULL, clean_up);
    CU_add_test(substring_suite, "substring null", test_substring_null);
    CU_add_test(substring_suite, "substring invalid indexes on point", test_substring_invalid_indexes_on_point);
    CU_add_test(substring_suite, "substring invalid indexes", test_substring_invalid_indexes);
    CU_add_test(substring_suite, "substring valid index", test_substring);

    /* is_digit suite */
    CU_pSuite is_digit_suite = create_suite("is_digit suite", NULL, NULL);
    CU_add_test(is_digit_suite, "is digit", test_is_digit);
    CU_add_test(is_digit_suite, "is not digit", test_is_not_digit);

    /* is_alpha suite */
    CU_pSuite is_alpha_suite = create_suite("is_alpha suite", NULL, NULL);
    CU_add_test(is_alpha_suite, "is alpha", test_is_alpha);
    CU_add_test(is_alpha_suite, "is not alpha", test_is_not_alpha);

    /* is_alphanumeric suite */
    CU_pSuite is_alphanumeric_suite = create_suite("is_alphanumeric suite", NULL, NULL);
    CU_add_test(is_alphanumeric_suite, "is alphanumeric", test_is_alphanumeric);
    CU_add_test(is_alphanumeric_suite, "is not alphanumeric", test_is_not_alphanumeric);

    /* parse_double suite */
    CU_pSuite parse_double_suite = create_suite("parse_double suite", NULL, NULL);
    CU_add_test(parse_double_suite, "parse_double valid", test_parse_double_valid);

    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
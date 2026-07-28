#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <stdint.h>


#define HASHMAP_FREE_KEY(k) free(k)
#define HASHMAP_KEY_TYPE char*
#define HASHMAP_VALUE_TYPE int
#define HASHMAP_TAG string_int_hashmap
#include "hashmap.h"

// Global dll struct pointer
string_int_hashmap* map;

/*
Helper function to free the memory for the dll
*/
static void clean_up(void) {
    
}
/*
Helper function to initialize the dll
*/
static void set_up(void) {
    
}

/*
Helper function to set up a string
@param result The address of the resulting string
@param value The literal value of a string
@return Pointer to a newly allocated string
*/
static size_t set_string(char** result, char* value) {
    size_t size = strlen(value);
    *result = malloc(size);
    strcpy(*result, value);
    return size;
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

void test_hash_value() {
    char* key;
    size_t size = set_string(&key, "Ormund");
    uint32_t position = hash(key, 100, size); 
    CU_ASSERT_EQUAL(position, 76);
}

void test_hash_equal_keys() {
    char* key_1;
    char* key_2;
    size_t size_1 = set_string(&key_1, "faceless");  
    size_t size_2 = set_string(&key_2, "faceless");
    uint32_t pos_1 = hash(key_1, 100, size_1);
    uint32_t pos_2 = hash(key_2, 100, size_2);
    CU_ASSERT_EQUAL(pos_1, pos_2);
}

void test_hash_different_keys() {
    char* key_1;
    char* key_2;
    size_t size_1 = set_string(&key_1, "nomad");  
    size_t size_2 = set_string(&key_2, "Nomad");
    uint32_t pos_1 = hash(key_1, 100, size_1);
    uint32_t pos_2 = hash(key_2, 100, size_2);
    CU_ASSERT_NOT_EQUAL(pos_1, pos_2);
}

void test_initialize_entry() {
     string_int_hashmap_entry entry = string_int_hashmap_initialize_entry();
     CU_ASSERT_EQUAL(entry.occupied, 0);
     CU_ASSERT_EQUAL(entry.tombstone, 0);
     CU_ASSERT_EQUAL(entry.key_size, 0);
     CU_ASSERT_EQUAL(entry.value_size, 0);
}

void test_destroy_entry() {
    string_int_hashmap_entry entry = string_int_hashmap_initialize_entry();
    entry.occupied = 1;
    char* key;
    int key_size = set_string(&key, "nobody");
    entry.key = key;
    entry.key_size = key_size;
    entry.value = 1;
    entry.value_size = sizeof(1);
    string_int_hashmap_destroy_entry(&entry);
    CU_ASSERT_EQUAL(entry.key_size, 0);
    CU_ASSERT_EQUAL(entry.value_size, 0);
    CU_ASSERT_EQUAL(entry.occupied, 0);
    CU_ASSERT_EQUAL(entry.tombstone, 1);
}



int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");

    /* Hash suite */
    CU_pSuite hash_suite = create_suite("hash suite", NULL, NULL);
    CU_add_test(hash_suite, "hash value", test_hash_value);
    CU_add_test(hash_suite, "hash equal keys", test_hash_equal_keys);
    CU_add_test(hash_suite, "hash different keys", test_hash_different_keys);

    CU_pSuite initialize_entry_suite = create_suite("initialize_entry suite", NULL, NULL);
    CU_add_test(initialize_entry_suite, "initialize entry", test_initialize_entry);
    CU_add_test(initialize_entry_suite, "destroy entry", test_destroy_entry);
    
    
    /* Initialize entry suite */
    // run the tests
    CU_basic_run_tests();
    

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
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
Helper function to free the memory for the hashmap
*/
static void clean_up(void) {
    string_int_hashmap_destroy(&map);
}
/*
Helper function to initialize the hashmap
*/
static void set_up(void) {
    map = string_int_hashmap_initialize();
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

void test_hash_value(void) {
    char* key;
    size_t size = set_string(&key, "Ormund");
    uint32_t position = string_int_hashmap_hash(key, 100, size); 
    CU_ASSERT_EQUAL(position, 76);
}

void test_hash_equal_keys(void) {
    char* key_1;
    char* key_2;
    size_t size_1 = set_string(&key_1, "faceless");  
    size_t size_2 = set_string(&key_2, "faceless");
    uint32_t pos_1 = string_int_hashmap_hash(key_1, 100, size_1);
    uint32_t pos_2 = string_int_hashmap_hash(key_2, 100, size_2);
    CU_ASSERT_EQUAL(pos_1, pos_2);
}

void test_hash_different_keys(void) {
    char* key_1;
    char* key_2;
    size_t size_1 = set_string(&key_1, "nomad");  
    size_t size_2 = set_string(&key_2, "Nomad");
    uint32_t pos_1 = string_int_hashmap_hash(key_1, 100, size_1);
    uint32_t pos_2 = string_int_hashmap_hash(key_2, 100, size_2);
    CU_ASSERT_NOT_EQUAL(pos_1, pos_2);
}


void test_kill_entry(void) {
    string_int_hashmap_entry* entries = string_int_hashmap_initialize_entry_array(1);
    string_int_hashmap_entry entry = entries[0];
    entry.occupied = 1;
    char* key;
    int key_size = set_string(&key, "nobody");
    entry.key = key;
    entry.key_size = key_size;
    entry.value = 1;
    entry.value_size = sizeof(1);
    string_int_hashmap_kill_entry(&entry);
    CU_ASSERT_EQUAL(entry.key_size, 0);
    CU_ASSERT_EQUAL(entry.value_size, 0);
    CU_ASSERT_EQUAL(entry.occupied, 0);
    CU_ASSERT_EQUAL(entry.tombstone, 1);
}

void test_equal_keys(void) {
    int res = string_int_hashmap_equal_keys("steel", 6, "steel", 6);
    CU_ASSERT_TRUE(res);
}

void test_equal_keys_unequal_size(void) {
    int res = string_int_hashmap_equal_keys("last", 5, "first", 6);
    CU_ASSERT_FALSE(res);
}

void test_equal_keys_unequal_same_size(void) {
    int res = string_int_hashmap_equal_keys("take", 5, "bake", 5);
    CU_ASSERT_FALSE(res);
}

void test_initialize_entry_array(void) {
    string_int_hashmap_entry* entries = string_int_hashmap_initialize_entry_array(16);
    CU_ASSERT_PTR_NOT_NULL(entries);
    for(int i = 0; i < 16; i++) {
        string_int_hashmap_entry entry = entries[i];
        CU_ASSERT_EQUAL(entry.key, (char*)0);
        CU_ASSERT_EQUAL(entry.key_size, 0);
        CU_ASSERT_EQUAL(entry.value, 0);
        CU_ASSERT_EQUAL(entry.value_size, 0);
        CU_ASSERT_EQUAL(entry.occupied, 0);
        CU_ASSERT_EQUAL(entry.tombstone, 0);
    }
    free(entries);
}

void test_initialize_hashmap(void) {
    string_int_hashmap* map = string_int_hashmap_initialize();
    CU_ASSERT_EQUAL(map->size, 0);
    CU_ASSERT_EQUAL(map->capacity, 16);
    CU_ASSERT_PTR_NOT_NULL(map->entries);
}

void test_destroy_hashmap(void) {
    string_int_hashmap_destroy(&map);
    CU_ASSERT_PTR_NULL(map);
}

void test_destroy_null_hashmap(void) {
    map = NULL;
    string_int_hashmap_destroy(&map);
    CU_ASSERT_PTR_NULL(map);
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

    /* Kill_entry suite */
    CU_pSuite kill_entry_suite = create_suite("initialize_kill_entry suite", NULL, NULL);
    CU_add_test(kill_entry_suite, "kill entry", test_kill_entry);

    /* Equal_key suite */
    CU_pSuite equal_key_suite = create_suite("equal_value suite", NULL, NULL);
    CU_add_test(equal_key_suite, "equal keys", test_equal_keys);
    CU_add_test(equal_key_suite, "unequal size keys", test_equal_keys_unequal_size);
    CU_add_test(equal_key_suite, "unequal keys same size", test_equal_keys_unequal_same_size);

    /* Initialize_entry_array suite */
    CU_pSuite initialize_entry_array_suite = create_suite("initialize_entry_array suite", NULL, NULL);
    CU_add_test(initialize_entry_array_suite, "initialize entry array", test_initialize_entry_array);

    /* Initialize_hashmap_suite */
    CU_pSuite initialize_hashmap_suite = create_suite("initialize_hashmap suite", NULL, clean_up);
    CU_add_test(initialize_hashmap_suite, "initialize hashmap", test_initialize_hashmap);

    /*  */
    CU_pSuite destroy_hashmap_suite = create_suite("destroy_hashmap suite", set_up, NULL);
    CU_add_test(destroy_hashmap_suite, "destroy hashmap", test_destroy_hashmap);
    CU_add_test(destroy_hashmap_suite, "destroy NULL hashmap", test_destroy_null_hashmap);

    CU_pSuite insert_suite = create_suite("insert suite", NULL, NULL);


    
    
    /* Initialize entry suite */
    // run the tests
    CU_basic_run_tests();
    

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
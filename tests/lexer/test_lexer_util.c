#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <err.h>
#include "lexer_util.h"
#include <unistd.h>
#include <string.h>


// Global stack struct pointer
line_s* line;

/*
Helper function to free the memory for the stack
*/
static void clean_up(void) {
    destroy_line(&line);
}

/*
Helper function to initialize the stack
*/
static void set_up(void) {
    line = initialize_line();
}

/*
Helper function to emulate writing to stdin
@param input Text to write to stdin
@return Number of the old stdin fd
*/
static int emulate_stdin(char* input) {
    int p[2];
    int old_stdin = dup(STDIN_FILENO);

    if(pipe(p) == -1) {
        printf("Pipe failed! Could not write to stdin! \n");
        return -1;
    }

    if(dup2(p[0], STDIN_FILENO) == -1) {
        printf("Stdin duplication failed");
        close(p[0]);
        close(p[1]);
        return -1;
    }

    FILE *stdin_writer = fdopen(p[1], "w");
    fprintf(stdin_writer, "%s", input);
    fclose(stdin_writer);   // Tell the OS you are no longer writing to stdin
    close(p[0]);            // Tell the OS you no longer need this fd pointer
    return old_stdin;
}

/*
Helper function to restore stdin fd
@param old_stdin Integer of the old stdin fd
*/
static void restore_stdin(int old_stdin) {
    if(dup2(old_stdin, STDIN_FILENO) == -1) {
        printf("Failed to restore stdin fd!\n");
        return;
    }
    close(old_stdin);
}

/* 
Helper function to create a suite
@param name Pointer to the name of the suite
@return CUnit suite object
*/
static CU_pSuite create_suite(const char* name,void(*set_up)(), void(*tear)()) {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown(name, NULL, NULL, set_up, tear); 
    if (CU_get_error() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    return suite;
}

void test_initialize(void) {
    line = initialize_line();
    CU_ASSERT_PTR_NOT_NULL(line);
    CU_ASSERT_PTR_NULL(line->buffer);
    CU_ASSERT_EQUAL(line->length, 0);
    CU_ASSERT_EQUAL(line->input, stdin);
}

void test_destroy(void) {
    destroy_line(&line);
    CU_ASSERT_PTR_NULL(line);
}

void test_getline_wrap(void) {
    char* in = "Shae is dead!";
    int old_stdin = emulate_stdin(in);
    int number_of_characters = getline_wrap(line, stdin);  
    printf("%s\n", line->buffer);
    CU_ASSERT_EQUAL(strcmp(line->buffer, in), 0);
    CU_ASSERT_EQUAL(number_of_characters, 13);
    restore_stdin(old_stdin);
}



int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");

    /* Initialize suite */
    CU_pSuite initialize_suite = create_suite("initialize suite", NULL, clean_up);
    CU_add_test(initialize_suite, "initialize", test_initialize);

    /* Destroy suite */
    CU_pSuite destroy_suite = create_suite("destroy suite", set_up, NULL);
    CU_add_test(destroy_suite, "destroy", test_destroy);

    /* Getline wrap suite*/
    CU_pSuite getline_wrap_suite = create_suite("getline wrap suite", set_up, clean_up);
    CU_add_test(getline_wrap_suite, "getline wrap test", test_getline_wrap);


    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
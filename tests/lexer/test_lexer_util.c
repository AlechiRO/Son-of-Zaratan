#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include "lexer_util.h"
#include "lexer.h"


// Global line and lexer context struct pointers
line_s* line;
lexer_context_s* lctx;

/*
Helper function to free the memory for the dependencies
*/
static void clean_up(void) {
    destroy_line(&line);
    destroy_lexer_context(&lctx);
}

/*
Helper function to initialize the dependencies
*/
static void set_up(void) {
    line = initialize_line();
    lctx = initialize_lexer_context();
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

void test_append_string_to_line(void) {
    append_to_line(line, "A small man can cast a very large shadow!");
    CU_ASSERT_TRUE(strcmp(line->buffer, "A small man can cast a very large shadow!") == 0);
}

void test_append_line_to_source(void) {
    append_to_line(line, "They were betrayed by Otto");
    append_to_source(lctx, line);
    CU_ASSERT_TRUE(strcmp(lctx->source, "They were betrayed by Otto") == 0);
}

void test_initialize_lexer_context(void) {
    lctx = initialize_lexer_context();
    CU_ASSERT_PTR_NULL(lctx->source);
    CU_ASSERT_PTR_NOT_NULL(lctx->tokens);
    CU_ASSERT_EQUAL(lctx->start, 0);
    CU_ASSERT_EQUAL(lctx->current, 0);
    CU_ASSERT_EQUAL(lctx->line_number, 1);
    CU_ASSERT_EQUAL(lctx->source_length, 0);
}

void test_destroy_lexer_context(void) {
    lctx->source = malloc(sizeof("Iron Islands"));
    strcpy(lctx->source, "Iron Islands");
    destroy_lexer_context(&lctx);
    CU_ASSERT_PTR_NULL(lctx);
}

void test_is_at_end(void) {
    lctx->current = 10;
    lctx->source_length = 11;
    CU_ASSERT_FALSE(is_at_end(lctx));
}

void test_is_not_at_end(void) {
    lctx->current = 5;
    lctx->source_length = 5;
    CU_ASSERT_TRUE(is_at_end(lctx));
}

void test_add_one_token(void) {
    lctx->source = malloc(sizeof("Drogon died in vain"));
    strcpy(lctx->source, "Drogon died in vain");
    lctx->start = 7;
    lctx->current = 19;

    literal_s* literal = initialize_literal(LITERAL_STRING);
    (literal->value).string_value = "died in vain"; 
    add_token(lctx, TOKEN_IDENTIFIER, literal);
    token_s* token = token_list_get(lctx->tokens, 0);
    CU_ASSERT_EQUAL(token->literal, literal);
    CU_ASSERT_TRUE(strcmp(token->lexeme, "died in vain") == 0);
    CU_ASSERT_EQUAL(token->line, 1);
    CU_ASSERT_EQUAL(token->type, TOKEN_IDENTIFIER);

    destroy_literal(&literal);
}

void test_add_two_tokens(void) {
    lctx->source = malloc(sizeof("The pie is poisoned"));
    strcpy(lctx->source, "The pie is poisoned");
    lctx->start = 4;
    lctx->current = 7;

    literal_s* literal = initialize_literal(LITERAL_STRING);
    (literal->value).string_value = "pie";
    add_token(lctx, TOKEN_STRING_GLOB, literal);
    lctx->start = 8;
    lctx->current = 10; 
    add_token(lctx, TOKEN_IDENTIFIER, NULL);
    
    token_s* token_0 = token_list_get(lctx->tokens, 0);
    token_s* token_1 = token_list_get(lctx->tokens, 1);

    CU_ASSERT_EQUAL(token_0->literal, literal);
    CU_ASSERT_PTR_NULL(token_1->literal);
    CU_ASSERT_TRUE(strcmp(token_0->lexeme, "pie") == 0);
    CU_ASSERT_TRUE(strcmp(token_1->lexeme, "is") == 0);
    CU_ASSERT_EQUAL(token_0->line, 1);
    CU_ASSERT_EQUAL(token_1->line, 1);
    CU_ASSERT_EQUAL(token_0->type, TOKEN_STRING_GLOB);
    CU_ASSERT_EQUAL(token_1->type, TOKEN_IDENTIFIER);
}

void test_advance(void) {
    lctx->source = malloc(sizeof("Jace made a mistake"));
    strcpy(lctx->source, "Jace made a mistake");

    lctx->current = 5;

    char chr = advance(lctx);
    CU_ASSERT_EQUAL(lctx->current, 6);
    CU_ASSERT_EQUAL(chr, 'm');

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

    /* Getline wrap suite */
    CU_pSuite getline_wrap_suite = create_suite("getline wrap suite", set_up, clean_up);
    CU_add_test(getline_wrap_suite, "getline wrap test", test_getline_wrap);

    /* Append_to_line suite */
    CU_pSuite append_to_line_suite = create_suite("append_to_line suite", set_up, clean_up);
    CU_add_test(append_to_line_suite, "append string to line", test_append_string_to_line);

    /* Append_to_source suite */
    CU_pSuite append_to_source_suite = create_suite("append_to_source suite", set_up, clean_up);
    CU_add_test(append_to_source_suite, "append line to source", test_append_line_to_source);

    /* Lexer_context_initialize suite */
    CU_pSuite lexer_context_initialize_suite = create_suite("lexer_context_initialize suite", NULL, clean_up);
    CU_add_test(lexer_context_initialize_suite, "initialize lexer context", test_initialize_lexer_context);
    
    /* Lexer_context_destroy suite */
    CU_pSuite lexer_context_destroy_suite = create_suite("lexer_context_destroy suite", set_up, NULL);
    CU_add_test(lexer_context_destroy_suite, "destroy lexer context", test_destroy_lexer_context);

    /* Is_at_end suite */
    CU_pSuite is_at_end_suite = create_suite("is_at_end suite", set_up, clean_up);
    CU_add_test(is_at_end_suite, "is at end", test_is_at_end);
    CU_add_test(is_at_end_suite, "is not at end", test_is_not_at_end);

    /* Add_token suite */
    CU_pSuite add_token_suite = create_suite("add_token suite", set_up, clean_up);
    CU_add_test(add_token_suite, "add one token test", test_add_one_token);
    CU_add_test(add_token_suite, "add one two tokens test", test_add_two_tokens);

    /* Advance suite */
    CU_pSuite advance_suite = create_suite("advance suite", set_up, clean_up);
    CU_add_test(advance_suite, "advance test", test_advance);

    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}
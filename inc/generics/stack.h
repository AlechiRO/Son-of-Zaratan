#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(STACK_TAG) || !defined(STACK_ITEM_TYPE)
#error "Missing tag or type definition"
#endif

#ifndef STACK_FREE_ITEM
    #define STACK_FREE_ITEM(item) ((void)(item))
#endif

#define STACK_CONCAT(TAG, METHOD) TAG##_##METHOD
#define STACK_CONCAT_EXP(TAG, METHOD) STACK_CONCAT(TAG, METHOD)
#define STACK_FN(METHOD) STACK_CONCAT_EXP(STACK_TAG, METHOD)

// Generic stack struct containing pointers to pointers
typedef struct STACK_TAG {
    int top;
    STACK_ITEM_TYPE* array;
    int capacity;
} STACK_TAG;

/*
Constructor
Allocate memory for the stack buffer and set the initial size
@param stack Pointer to a stack struct
*/
static inline STACK_TAG* STACK_FN(initialize)(void) {
    STACK_TAG* stack = malloc(sizeof(STACK_TAG));

    if(stack == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for stack!");
        exit(EXIT_FAILURE);
    }

    stack->top = -1;
    stack->capacity = 10; 
    stack->array = malloc(sizeof(STACK_ITEM_TYPE) * 10);

    if(stack->array == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for stack buffer!");
        exit(EXIT_FAILURE);
    }
    return stack;
}

/*
Check if the stack is empty
@param stack Pointer to stack struct
@return Integer 1 if true and 0 if false
*/
static inline int STACK_FN(is_empty)(STACK_TAG* stack) {
    return stack->top == -1;
}

/*
Get the number of objects stored in the stack
@param stack Pointer to stack struct
@return number of objects stored in the stack
*/
static inline int STACK_FN(size)(STACK_TAG* stack) {
    return stack->top+1;
}

/*
Check if the stack is full
@param stack Pointer to stack struct
@return Integer 1 if true and 0 if false
*/
static inline int STACK_FN(is_full)(STACK_TAG* stack) {
    return stack->top == (stack->capacity-1);
}

/*
Helper function
Double the capacity of the stack
@param stack Pointer to stack struct
*/
static inline void STACK_FN(increase_capacity)(STACK_TAG* stack) {
    stack->capacity *= 2;
    stack->array = realloc(stack->array, stack->capacity * sizeof(STACK_ITEM_TYPE));
}

/*
View the element at the top of the stack
@param stack Pointer to stack struct
@return Pointer to the element at the top of the stack
*/
static inline STACK_ITEM_TYPE STACK_FN(peek)(STACK_TAG* stack) {
    if(STACK_FN(is_empty)(stack)) {
        fprintf(stderr, "The stack is empty, there is nothing to retrieve!");
        STACK_ITEM_TYPE empty;
        memset(&empty, 0, sizeof(STACK_ITEM_TYPE));
        return empty;
    }
    return stack->array[stack->top];
}

/*
Push an element on the stack
@param stack Pointer to stack struct
@param object Pointer to data
*/
static inline void STACK_FN(push)(STACK_TAG* stack, STACK_ITEM_TYPE object) {
    if(STACK_FN(is_full)(stack))
        STACK_FN(increase_capacity)(stack);
    stack->array[++stack->top] = object;
}

/*
Pop and element from the stack
@param stack Pointer to stack struct
@return Pointer to the element at the top of the stack
*/
static inline STACK_ITEM_TYPE STACK_FN(pop)(STACK_TAG* stack) {
    if(STACK_FN(is_empty)(stack)) {
        fprintf(stderr, "ERROR: The stack is empty,there is nothing to retrieve!");
        STACK_ITEM_TYPE empty;
        memset(&empty, 0, sizeof(STACK_ITEM_TYPE));
        return empty;
    }
    return stack->array[stack->top--];
}

/*
Destructor
Free the memory allocated for the stack buffer
@param stack Pointer to a pointer to a stack struct
*/
static inline void STACK_FN(destroy)(STACK_TAG** stack) {
    if ((*stack) == NULL || stack == NULL) 
        return;
    
    if ((*stack)->array != NULL) {
        STACK_ITEM_TYPE* array = (*stack)->array;
        for(int i = 0; i <= (*stack)->top; i++)
            STACK_FREE_ITEM(array[i]);

        free((*stack)->array);
        (*stack)->array = NULL;
    }
    free((*stack));
    *stack = NULL;
}

#undef STACK_ITEM_TYPE
#undef STACK_TAG
#undef STACK_CONCAT
#undef STACK_FREE_ITEM
#undef STACK_CONCAT_EXP
#undef STACK_FN

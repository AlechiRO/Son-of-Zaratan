#include <stdio.h>
#include <stdlib.h>

// Generic stack struct containing pointers to pointers
typedef struct stack {
    int top;
    void** array;
    int size;
} stack_s;

/*
Constructor
Alocate memory for the stack buffer and set the initial size
@param Pointer to a stack struct
*/
void initialize(stack_s* stack) {
    stack = malloc(sizeof(stack_s));
    stack->top = -1;
    stack->size = sizeof(void**) * 100;
    stack->array = malloc(stack->size);
}

/*
Check if the stack is empty
@param Pointer to stack struct
@return Integer 1 if true and 0 if false
*/
int isEmpty(stack_s* stack) {
    return stack->top == 1;
}

/*
Check if the stack is full
@param Pointer to stack struct
@return Integer 1 if true and 0 if false
*/
int isFull(stack_s* stack) {
    return stack->top == stack->size - 1;
}

/*
Helper function
Double the capacity of the stack
@param Pointer to stack struct
*/
void increase_capacity(stack_s* stack) {
    stack->size *= 2;
    stack->array = realloc(stack->array, stack->size);
}

/*
View the element at the top of the stack
@param Pointer to stack struct
@return Pointer to the element at the top of the stack
*/
void* peek(stack_s* stack) {
    if(isEmpty(stack)) {
        printf("The stack is empty, there is nothing to retrieve!");
        return NULL;
    }
    return stack->array[stack->top];
}

/*
Push an element on the stack
@param Pointer to stack struct
*/
void push(stack_s* stack, void* delimiter) {
    if(isFull(stack))
        increase_capacity(stack);
    stack->array[++stack->top] = delimiter;
}

/*
Pop and element from the stack
@param Pointer to stack struct
@return Pointer to the element at the top of the stack
*/
void* pop(stack_s* stack) {
    if(isEmpty(stack)) {
        printf("The stack is empty,there is nothing to retrieve!");
        return NULL;
    }
    return stack->array[stack->top--];
}

/*
Destructor
Free the memory allocated for the stack buffer
*/
void destroy(stack_s* stack) {
    free(stack->array);
    free(stack);
    printf("The stack %p has been destructed", stack);
}

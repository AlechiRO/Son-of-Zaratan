#include <stdio.h>
#include <stdlib.h>

// Generic stack struct containing pointers to pointers
typedef struct stack {
    int top;
    void** array;
    size_t size;
} stack_s;

/*
Constructor
Alocate memory for the stack buffer and set the initial size
@param stack Pointer to a stack struct
*/
stack_s* initialize_stack() {
    stack_s* stack = malloc(sizeof(stack_s));
    stack->top = -1;
    stack->size = sizeof(void**) * 100;
    stack->array = malloc(stack->size);
    return stack;
}

/*
Check if the stack is empty
@param stack Pointer to stack struct
@return Integer 1 if true and 0 if false
*/
int isEmpty_stack(stack_s* stack) {
    return stack->top == -1;
}

/*
Check if the stack is full
@param stack Pointer to stack struct
@return Integer 1 if true and 0 if false
*/
int isFull_stack(stack_s* stack) {
    return stack->top ==  stack->size-1;
}

/*
Helper function
Double the capacity of the stack
@param stack Pointer to stack struct
*/
void static increase_capacity(stack_s* stack) {
    stack->size *= 2;
    stack->array = realloc(stack->array, stack->size);
}

/*
View the element at the top of the stack
@param stack Pointer to stack struct
@return Pointer to the element at the top of the stack
*/
void* peek_stack(stack_s* stack) {
    if(isEmpty_stack(stack)) {
        printf("The stack is empty, there is nothing to retrieve!");
        return NULL;
    }
    return stack->array[stack->top];
}

/*
Push an element on the stack
@param stack Pointer to stack struct
@param object Pointer to data
*/
void push_stack(stack_s* stack, void* object) {
    if(isFull_stack(stack))
        increase_capacity(stack);
    stack->array[++stack->top] = object;
}

/*
Pop and element from the stack
@param stack Pointer to stack struct
@return Pointer to the element at the top of the stack
*/
void* pop_stack(stack_s* stack) {
    if(isEmpty_stack(stack)) {
        printf("The stack is empty,there is nothing to retrieve!");
        return NULL;
    }
    return stack->array[stack->top--];
}

/*
Destructor
Free the memory allocated for the stack buffer
@param stack Pointer to a stack struct
*/
void destroy_stack(stack_s* stack) {
    if (stack == NULL) {
        return;
    }
    if (stack->array != NULL) {
        free(stack->array);
        stack->array = NULL;
    }
    free(stack);
    stack = NULL;
    printf("The stack %p has been destructed", stack);
}

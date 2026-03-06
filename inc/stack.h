#ifndef READER_H
#define READER_H

typedef struct stack {
    int top;
    void** array;
    int size;
} stack_s;

stack_s* initialize_stack();
int is_empty_stack(stack_s* stack);
void* peek_stack(stack_s* stack);
void push_stack(stack_s* stack, void* delimiter);
void* pop_stack(stack_s* stack);
void destroy_stack(stack_s* stack);
int size_stack(stack_s* stack);

#endif
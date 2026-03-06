#ifndef READER_H
#define READER_H

typedef struct stack {
    int top;
    void** array;
    int size;
} stack_s;

stack_s* initialize();
int isEmpty(stack_s* stack);
int isFull(stack_s* stack);
void increase_capacity(stack_s* stack);
void* peek(stack_s* stack);
void push(stack_s* stack, void* delimiter);
void* pop(stack_s* stack);
void destroy(stack_s* stack);

#endif
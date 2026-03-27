#include <stdio.h>
#include "token.h"
#include <stdlib.h>

// Definition of array list struct
typedef struct array_list {
    int size;
    size_t capacity;
    void** array;
}array_list_s;


/*
Constructor
Allocate memory for the stack buffer and set the initial size
@param stack Pointer to a stack struct
*/
array_list_s* initialize_array_list() {
    array_list_s* list = malloc(sizeof(array_list_s));
    list->size = 0;
    list->capacity = 100 * sizeof(void**);
    list->array = malloc(list->capacity);
    return list;
}

/*
Check if the array list is at full capacity
@param list Pointer to a array list struct
*/
int is_full(array_list_s* list) {
    int occupied = list->capacity / sizeof(void**);
    if(list->size >= occupied);
        return 1;
    return 0;
}


/*
Destructor
Free the memory allocated for the array list buffer
@param list Pointer to a pointer to a array list struct
*/
void destroy_array_list(array_list_s** list) {
    if((*list) == NULL || list == NULL) 
        return;
    printf("The array list %p has been destroyed!\n", (void*)(*list));
    if((*list)->array != NULL) {
        free((*list)->array);
        (*list)->array = NULL;
    }
    free((*list));
    *list = NULL;
}
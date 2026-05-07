#include <stdio.h>
#include "token.h"
#include <stdlib.h>
#include "array_list.h"





// #define DECLARE_LIST(type, name) \
//     typedef struct {

//     }
/*
Constructor
Allocate memory for the stack buffer and set the initial size
@param stack Pointer to a stack struct
@return Pointer to a new array list struct
*/
array_list_s* initialize_array_list() {
    array_list_s* list = malloc(sizeof(array_list_s));
    list->size = 0;
    list->capacity = 10;
    list->array = malloc(list->capacity * sizeof(void*));
    return list;
}

/*
Private function used to check if the array list is at full capacity
@param list Pointer to an array list struct
@return true if the array list is full and false otherwise
*/
static int is_full(array_list_s* list) {
    if(list->size >= list->capacity)
        return 1;
    return 0;
}

/*
Private function used to double the capacity of the array
@param list Pointer to an array list struct
*/
static void increase_capacity(array_list_s* list) {
    size_t new_capacity = list->capacity * 2;
    void** new_array = realloc(list->array, new_capacity * sizeof(void*));
    if (new_array == NULL) {
        printf("Could not increase capacity of array list");
        return; 
    }
    list->array = new_array;
    list->capacity = new_capacity;
}

/*
Check if the array list is empty
@param list Pointer to a array list struct
@return true if the array list is empty and false otherwise
*/
int is_empty_array_list(array_list_s* list) {
    return list->size == 0;
}

/*
Adds an element after a specific index
@param list Pointer to array list struct
@param index Index in the array list to insert the element
@param element Element to store in the array list
*/
void add_at_index_array_list(array_list_s* list, int index, void* element) {
    if(index < 0) {
        printf("Array list index out of bounds!");
        return;
    }
    
    if(index > list->size) 
        index = list->size;
    
    if(is_full(list))
        increase_capacity(list);
    
    void** arr = list->array;
    for(int i = list->size; i > index; i--) 
        arr[i] = arr[i-1];

    arr[index] = element;
    list->size++;
}

/*
Add an element at the end of the list
@param list Pointer to an array list struct
@param element Element to store in the array list
*/
void add_array_array_list(array_list_s* list, void* element) {
    add_at_index_array_list(list, list->size, element);
}

/*
Add an element at the begining of the list
@param list Pointer to an array list struct
@param element Element to store in the array list
*/
void add_first_array_list(array_list_s* list, void* element) {
    add_at_index_array_list(list, 0, element);
}

/*
Set the value at any specific index to another value
@param list Pointer to an array list struct
@param index Index of the element to change
@element Element to be placed in the array list
*/
void set_array_list(array_list_s* list, int index, void* element) {
    if(index < 0 || index >= list->size) {
        printf("Invalid index array list!");
        return;
    }
    (list->array)[index] = element;
}

/*
Get the element at a specific index
@param list Pointer to an array list struct
@param index Index of the element to get
@return Pointer to the element in the array list
*/
void* get_array_list(array_list_s* list, int index) {
    if(index < 0 || index >= list->size) {
        printf("Invalid index array list!");
        return NULL;
    }
    return (list->array)[index];
}


/*
Remove an element from the array list
@param list Pointer to an array list struct
@param index Index of the element to remove
@return Pointer to the removed element
*/
void* remove_array_list(array_list_s* list, int index) {
    if(index < 0 || index >= list->size) {
        printf("Invalid index array list!");
        return NULL;
    }
    void** arr = list->array;
    void* result = (list->array)[index];
    for(int i = index; i < list->size - 1; i++) 
        arr[i] = arr[i+1];
    list->size--;
    return result;
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

    void** arr = (*list)->array;

    for(int i = 0; i < (*list)->size; i++) 
        if(arr[i] != NULL)
            free(arr[i]);

    if((*list)->array != NULL) {
        free((*list)->array);
        (*list)->array = NULL;
    }
    free((*list));
    *list = NULL;
}
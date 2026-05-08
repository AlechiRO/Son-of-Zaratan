#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
#include <stdio.h>
#include <stdlib.h>

// Definition of array list struct
#define DECLARE_LIST(type, name) \
    typedef struct {             \
        int size;                \
        int capacity;            \
        type* array;             \
    } name;                      \
                                 \
/*                                                              \               
Constructor                                                     \
Allocate memory for the stack buffer and set the initial size   \
@return Pointer to a new array list struct                  \
*/                                                          \
static inline name* name##_initialize() {                   \
    name* list = malloc(sizeof(name));                      \
    list->size = 0;                                         \
    list->capacity = 10;                                    \
    list->array = malloc(list->capacity * sizeof(type));    \
    return list;                                            \
}                                                           \
                                                            \
/*
Private function to check if the array list is at full capacity  \
@param list Pointer to an array list struct                 \
@return true if the array list is full and false otherwise  \
*/                                                          \
static inline int name##_is_full(name* list) {              \
    if(list->size >= list->capacity)                        \
        return 1;                                           \
    return 0;                                               \
}                                                           \
                                                            \
/*                                                          \
Private function used to double the capacity of the array   \
@param list Pointer to an array list struct                 \
*/                                                          \
static inline void name##_increase_capacity(name* list) {          \
    size_t new_capacity = list->capacity * 2;                      \
    type* new_array = realloc(list->array, new_capacity * sizeof(type));    \
    if (new_array == NULL) {                                                \
        printf("Could not increase capacity of array list");                \
        return;                                                             \
    }                                                                       \
    list->array = new_array;                                                \
    list->capacity = new_capacity;                                          \
}                                                                           \
                                                                    \
/*                                                                  \
Check if the array list is empty                                    \
@param list Pointer to a array list struct                          \
@return true if the array list is empty and false otherwise         \
*/                                                                  \
static inline int name##_is_empty(name* list) {          \
    return list->size == 0;                                         \
}                                                                   \
                                                                    \
/*                                                                  \
Adds an element after a specific index                              \
@param list Pointer to array list struct                            \
@param index Index in the array list to insert the element          \
@param element Element to store in the array list                   \
*/                                                                  \
static inline void name##_add_at_index(name* list, int index, type element) {  \
    if(index < 0) {                                                 \
        printf("Array list index out of bounds!");                  \
        return;                                                     \
    }                                                               \
                                                                    \
    if(index > list->size)                                          \
        index = list->size;                                         \
                                                                    \
    if(name##_is_full(list))                                        \
        name##_increase_capacity(list);                             \
                                                                    \
    type* arr = list->array;                                        \
    for(int i = list->size; i > index; i--)                         \
        arr[i] = arr[i-1];                                          \
                                                                    \
    arr[index] = element;                                           \
    list->size++;                                                   \
}                                                                   \
                                                                    \
/*                                                                  \
Add an element at the end of the list                               \
@param list Pointer to an array list struct                         \
@param element Element to store in the array list                   \
*/                                                                  \
static inline void name##_add(name* list, type element) {       \
    name##_add_at_index(list, list->size, element);         \
}                                                               \
                                                                \
/*                                                              \
Add an element at the begining of the list                      \
@param list Pointer to an array list struct                     \
@param element Element to store in the array list               \
*/                                                              \
static inline void name##_add_first(name* list, type element) { \
    name##_add_at_index(list, 0, element);                      \
}                                                               \
                                                                \
/*                                                              \
Set the value at any specific index to another value            \
@param list Pointer to an array list struct                     \
@param index Index of the element to change                     \
@element Element to be placed in the array list                 \
*/                                                              \
static inline void name##_set(name* list, int index, type element) {    \
    if(index < 0 || index >= list->size) {                              \
        printf("Invalid index array list!");                            \
        return;                                                         \
    }                                                                   \
    (list->array)[index] = element;                                     \
}                                                                       \
                                                                        \
/*                                                  \
Get the element at a specific index                 \
@param list Pointer to an array list struct         \
@param index Index of the element to get            \
@return Pointer to the element in the array list    \
*/                                                  \
static inline type name##_get(name* list, int index) {  \
    if(index < 0 || index >= list->size) {              \
        printf("Invalid index array list!");            \
        exit(1);                                        \
    }                                                   \
    return (list->array)[index];                        \
}                                                       \
                                                        \
/*                                              \
Remove an element from the array list           \
@param list Pointer to an array list struct     \
@param index Index of the element to remove     \
@return Pointer to the removed element          \
*/                                              \
static inline type name##_remove(name* list, int index) {   \
    if(index < 0 || index >= list->size) {                  \
        printf("Invalid index array list!");                \
        exit(1);                                            \
    }                                                       \
    type* arr = list->array;                                \
    type result = (list->array)[index];                     \
    for(int i = index; i < list->size - 1; i++)             \
        arr[i] = arr[i+1];                                  \
    list->size--;                                           \
    return result;                                          \
}                                                           \
                                                            \
/*                                                          \
Destructor                                                  \
Free the memory allocated for the array list buffer         \
@param list Pointer to a pointer to a array list struct     \
*/                                                          \
static inline void name##_destroy(name** list) {            \
    if((*list) == NULL || list == NULL)                     \
        return;                                             \
    printf("The array list %p has been destroyed!\n", (void*)(*list));  \
                                                                        \
    if((*list)->array != NULL) {                                        \
        free((*list)->array);                                           \
        (*list)->array = NULL;                                          \
    }                                                                   \
    free((*list));                                                      \
    *list = NULL;                                                       \
}

#endif
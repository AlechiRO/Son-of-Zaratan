#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
#include <stdio.h>

// Definition of array list struct
typedef struct array_list {
    int size;
    size_t capacity;
    void** array;
}array_list_s;

array_list_s* initialize_array_list();
int is_empty_array_list(array_list_s* list);
void add_at_index_array_list(array_list_s* list, int index, void* element);
void add_array_array_list(array_list_s* list, void* element);
void add_first_array_list(array_list_s* list, void* element);
void set_array_list(array_list_s* list, int index, void* element);
void* get_array_list(array_list_s* list, int index);
void* remove_array_list(array_list_s* list, int index);
void destroy_array_list(array_list_s** list);


#endif
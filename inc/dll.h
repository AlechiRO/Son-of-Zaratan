#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DECLARE_DLL(type, name)             \
/* Node definition for dll implementation*/ \
typedef struct name##_node {                       \
    struct node* next;                      \
    struct node* prev;                      \
    type payload;                           \
} name##_node_s;                                   \
                                            \
/*DLL definition with sentinel head and tail*/  \
typedef struct dll {                            \
    dll_node_s* head;                               \
    dll_node_s* tail;                               \
    int size;                                   \
} name;                                         \
                                                \
/*                                              \
Create a new node                               \
@param payload Pointer to the node payload      \
@param prev The previous node                   \
@param next The next node                       \
@return Pointer to the node                     \
*/                                              \
static inline dll_node_s* name##_initialize_node(type payload, dll_node_s* prev, dll_node_s* next) {    \
    dll_node_s* node = malloc(sizeof(dll_node_s));              \
    if(!node)                                                   \
        return NULL;                                            \ 
    node->payload = payload;                                    \  
    name##_set_next(node, next);                                   \
    name##_set_prev(node, prev);                                   \
                                                                \
    return node;                                                \
}                                                               \
                                                                \
/*                                      \
Get the node data payload               \
@param node Node struct                 \
@return The data payload from the node  \
*/                                      \
static inline type name##_get_payload(dll_node_s* node) {   \
    return node->payload;                                   \
}                                                           \
                                                            \
/*                                                          \
Get the next node                                           \
@param node The reference node                              \
@return The next node                                       \
*/                                                          \
static inline dll_node_s* name##_get_next(dll_node_s* node) {   \
    return node->next;                                          \
}                                                               \
                                                                \
/*                                  \
Get the prev node                   \
@param node The reference node      \
@return The prev node               \
*/                                  \
static inline dll_node_s* name##_get_prev(dll_node_s* node) {       \
    return node->prev;                                              \
}                                                                   \
                                                                    \
/*                                                      \
Set the next node                                       \
@param node Node that has its next parameter set        \
@param next Node that is pointed to by the attribute    \
*/                                                      \
static inline void name##_set_next(dll_node_s* node, dll_node_s* next) {    \
    node->next = next;                                                      \
}                                                                           \
                                                                            \
/*                                                          \
Set the prev node                                           \
@param node Node that has its prev parameter set            \
@param prev Node that is pointed to by the attribute        \
*/                                                          \
static inline void name##_set_prev(dll_node_s* node, dll_node_s* prev) {    \
    node->prev = prev;                                                      \
}                                                                           \
                                                                            \
/*                                              \
Destroy a node                                  \
@param Pointer to a pointer of a node struct    \
*/                                              \
static inline void name##_destroy_node(dll_node_s** node) {     \
    if((*node) == NULL || node == NULL)                             \
        return;                                                     \
                                                                    \
    free((*node));                                                  \
    (*node) = NULL;                                                 \
}                                                                   \
                                                                    \
/*                                  \
Initialize the dll                  \
@return Pointer to a dll struct     \
*/                                  \
static inline name* name##_initialize(void) {   \
    name* dll = malloc(sizeof(name));           \
    if(dll == NULL)                             \
        return NULL;                            \
                                                \
    type dummy_payload;                         \
    memset(&dummy_payload, 0, sizeof(type));                            \
    dll->head = name##_initialize_node(dummy_payload, NULL, NULL);      \
    dll->tail = name##_initialize_node(dummy_payload, NULL, NULL);      \
                                                                \
    if (!dll->head || !dll->tail)                               \
        return NULL;                                            \
                                                                \
    name##_set_prev(dll->tail, dll->head);      \
    name##_set_next(dll->head, dll->tail);      \
    dll->size = 0;                              \
    return dll;                                 \
}                                               \
                                                \
/*                              \
Get the size of the DLL         \
@param Pointer to DLL struct    \
@return size of DLL             \
*/                              \
static inline int name##_get_size(name* dll) {  \
    return dll->size;           \
}                               \
                                \
/*                                              \
Insert a node between two nodes                 \
@param dll Pointer to dll struct                \
@param paylaod Pointer to the node payload      \
@param prev The node before the inserted node   \
@param next The node after the inserted node    \
*/                                              \
static inline void name##_insert_between(name* dll, type payload, dll_node_s* prev, dll_node_s* next) {     \
    dll_node_s* node = name##_initialize_node(payload, prev, next);                                            \
                                                                                                            \
    name##_set_prev(next, node);                                                                            \
    name##_set_next(prev, node);                                                                            \
    dll->size++;                                                                                            \
}                                                                                                           \
                                                                                                            \
/*                                              \
Insert a node right after the sentinel head     \
@param dll Pointer to the DLL struct            \
@payload Pointer to the node payload            \
*/                                              \
static inline void name##_insert_first(name* dll, type payload) {             \
    name##_insert_between(dll, payload, dll->head, name##_get_next(dll->head)); \
}                                                                               \
                                                                                \
/*                                                                              \
Insert a node right before the sentinel tail                                    \
@param dll Pointer to the DLL struct                                            \
@payload Pointer to the node payload                                            \
*/                                                                              \
static inline void name##_insert_last(name* dll, type payload) {                      \
    name##_insert_between(dll, payload, name##_get_prev(dll->tail), dll->tail);         \
}                                                                                       \
                                                                                        \
/*                                  \
Remove a node from the list         \
@param dll Pointer to DLL struct    \
@param node Node that is removed    \
*/                                  \
static inline type name##_remove_node(name* dll, dll_node_s* node) {   \
    if(dll->size == 0) {                                            \
        printf("DLL is empty, node can't be removed!");             \
        return NULL;                                                \
    }                                                               \
    dll_node_s* prev = name##_get_prev(node);                       \
    dll_node_s* next = name##_get_next(node);                       \
    type payload = name##_get_payload(node);                           \
    name##_set_next(prev, next);                                    \
    name##_set_prev(next, prev);                                    \
    name##_destroy_node(&node);                                     \
    dll->size--;                                                    \
    return payload;                                                 \
}                                                                   \
                                                    \
/*                                                  \
Remove the first node after the DLL sentinel head   \
@param dll Pointer to DLL struct                    \
*/                                                  \
static inline type name##_remove_first(name* dll) {             \
    return name##_remove_node(dll, name##_get_next(dll->head));    \
}                                                               \
                                                                \
/*                                                              \
Remove the first node before the DLL sentinel tail              \
@param dll Pointer to DLL struct                                \
*/                                                              \
static inline type name##_remove_last(name* dll) {              \
    return name##_remove_node(dll, name##_get_prev(dll->tail));    \
}                                                               \
                                                                \
/*                                                              \
Get the payload of the first node in the DLL                    \
@param dll Pointer to DLL struct                                \
*/                                                              \
static inline type name##_get_first(name* dll) {                   \
    if(dll->size != 0)                                          \
        return name##_get_payload(name##_get_next(dll->head));  \
    printf("DLL is empty, can't retrieve payload!");            \
    return NULL;                                                \
}                                                               \
                                                                \
/*                                                              \
Get the payload of the first node in the DLL                    \
@param dll Pointer to DLL struct                                \
*/                                                              \
static inline type name##_get_last(name* dll) {                 \
    if(dll->size != 0)                                          \
        return name##_get_payload(name##_get_prev(dll->tail));     \
    printf("DLL is empty, can't retrieve payload!");            \
    return NULL;                                                \
}                                                               \
                                                                \
/*                                                              \
Destroy the DLL and all associated nodes                        \
@param dll Pointer to a pointer of DLL struct                   \
*/                                                              \
static inline void name##_destroy(name** dll) {                 \
    if (dll == NULL || (*dll) == NULL)                          \
        return;                                                 \
                                                                \
    dll_node_s* current = (*dll)->head;                         \
    while(current != NULL) {                                    \
        dll_node_s* next = name##_get_next(current);            \
        name##_destroy_node(&current);                          \
        current = next;                                         \
    }                                                           \
    printf("The DLL %p has been destructed!\n", (type)*dll);    \
    free((*dll));                                               \
    (*dll) = NULL;                                              \
}

#endif
#include <stdio.h>


// Node definition for dll implementation
typedef struct node {
    struct node* next;
    struct node* prev;
    void* payload;
} node_s;

/*
Get the next node
@param node The reference node
@return The next node
*/
node_s* get_next_dll(node_s* node) {
    return node->next;
}

/*
Get the prev node
@param node The reference node
@return The prev node
*/
node_s* get_prev_dll(node_s* node) {
    return node->prev;
}

/*
Set the next node
@param node Node that has its next parameter set
@param next Node that is pointed to by the attribute
*/
void set_next_dll(node_s* node, node_s* next) {
    node->next = next;
}

/*
Set the prev node
@param node Node that has its prev parameter set
@param prev Node that is pointed to by the attribute
*/
void set_prev_dll(node_s* node, node_s* prev) {
    node->prev = prev;
}


/*
Create a new node 
@param prev The previous node
@param next The next node
@return Pointer to the node
*/
node_s* create_node_dll(void* payload, node_s* prev, node_s* next) {
    node_s* node = malloc(sizeof(node));
    node->payload = payload;
    set_next_dll(node, next);
    set_prev_dll(node, prev);
}

// DLL definition with sentinel head and tail
typedef struct dll {
    node_s* head;
    node_s* tail;
    int size;
} dll_s;

/*
Initialize the dll
@return Pointer to a dll struct
*/
dll_s* initialize_dll(void) {
    dll_s* dll = malloc(sizeof(dll_s));
    dll->head = create_node_dll(NULL, NULL, NULL);
    dll->tail = create_node_dll(NULL, NULL, NULL);

    set_prev_dll(dll->tail, dll->head);
    set_next_dll(dll->head, dll->tail);
    dll->size = 0;
    return dll;
}
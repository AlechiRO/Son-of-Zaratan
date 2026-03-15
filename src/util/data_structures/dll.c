#include <stdio.h>
#include <stdlib.h>

// Node definition for dll implementation
typedef struct node {
    struct node* next;
    struct node* prev;
    void* payload;
} node_s;


/*
Create a new node 
@param payload Pointer to the node payload
@param prev The previous node
@param next The next node
@return Pointer to the node
*/
node_s* create_node_dll(void* payload, node_s* prev, node_s* next) {
    node_s* node = malloc(sizeof(node_s));
    if(!node)
        return NULL;
    node->payload = payload;
    set_next_dll(node, next);
    set_prev_dll(node, prev);

    return node;
}

/*
Get the node data payload
@param node Node struct
@return The data payload from the node
*/
void* get_payload_dll(node_s* node) {
    return node->payload;
}

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
Destroy a node 
@param Pointer to a pointer of a node struct
*/
void destroy_node_dll(node_s** node) {
    if((*node) == NULL || node == NULL) {
        return;
    }
    free((*node));
    (*node) = NULL;
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
    if(dll == NULL) 
        return NULL;
    dll->head = create_node_dll(NULL, NULL, NULL);
    dll->tail = create_node_dll(NULL, NULL, NULL);

    if (!dll->head || !dll->tail) 
        return NULL;
    
    set_prev_dll(dll->tail, dll->head);
    set_next_dll(dll->head, dll->tail);
    dll->size = 0;
    return dll;
}
/*
Insert a node between two nodes
@param dll Pointer to dll struct
@param paylaod Pointer to the node payload
@param prev The node before the inserted node
@param next The node after the inserted node
*/
void insert_between_dll(dll_s* dll, void* payload, node_s* prev, node_s* next) {
    node_s* node = create_node_dll(payload, prev, next);

    set_prev_dll(next, node);
    set_next_dll(prev, node);
    dll->size++;
}

/*
Insert a node right after the sentinel head
@param dll Pointer to the DLL struct
@payload Pointer to the node payload
*/
void insert_first_dll(dll_s* dll, void* payload) {
    insert_between_dll(dll, payload, dll->head, get_next_dll(dll->head));
}

/*
Insert a node right before the sentinel tail
@param dll Pointer to the DLL struct
@payload Pointer to the node payload
*/
void insert_last_dll(dll_s* dll, void* payload) {
    insert_between_dll(dll, payload, get_prev_dll(dll->tail), dll->tail);
}

/*
Remove a node from the list
@param dll Pointer to DLL struct
@param node Node that is removed
*/
void* remove_node_dll(dll_s* dll, node_s* node) {
    if(dll->size == 0) {
        printf("DLL is empty, node can't be removed!");
        return NULL;
    }
    node_s* prev = get_prev_dll(node);
    node_s* next = get_next_dll(node);
    void* payload = get_payload_dll(node);
    set_next_dll(prev, next);
    set_prev_dll(next, prev);
    destroy_node_dll(&node);
    dll->size--;
    return payload;
}

/*
Remove the first node after the DLL sentinel head
@param dll Pointer to DLL struct
*/
void* remove_first_dll(dll_s* dll) {
    return remove_node_dll(dll, get_next_dll(dll->head));
}

/*
Remove the first node before the DLL sentinel tail
@param dll Pointer to DLL struct
*/
void* remove_last_dll(dll_s* dll) {
    return remove_node_dll(dll, get_prev_dll(dll->tail));
}

/*
Get the payload of the first node in the DLL
@param dll Pointer to DLL struct
*/
void* get_first_dll(dll_s* dll) {
    if(dll->size != 0)
        return get_payload_dll(get_next_dll(dll->head));
    printf("DLL is empty, can't retrieve payload!");
    return NULL;
}

/*
Get the payload of the first node in the DLL
@param dll Pointer to DLL struct
*/
void* get_last_dll(dll_s* dll) {
    if(dll->size != 0)
        return get_payload_dll(get_prev_dll(dll->tail));
    printf("DLL is empty, can't retrieve payload!");
    return NULL;
}

/*
Destroy the DLL and all associated nodes
@param dll Pointer to a pointer of DLL struct
*/
void destroy_dll(dll_s** dll) {
    node_s* current = (*dll)->head;
    while(current != NULL) {
        node_s* next = get_next_dll(current);
        destroy_node_dll(&current);
        current = next;
    }
    printf("The DLL %p has been destructed!\n", (void*)*dll);
    free((*dll));
    (*dll) = NULL;
}

#ifndef LEXER_H
#define LEXER_H

// Node definition for dll implementation
typedef struct node {
    struct node* next;
    struct node* prev;
    void* payload;
} node_s;

// DLL definition with sentinel head and tail
typedef struct dll {
    node_s* head;
    node_s* tail;
    int size;
} dll_s;


node_s* initialize_node_dll(void* payload, node_s* prev, node_s* next);
void* get_payload_dll(node_s* node);
node_s* get_next_dll(node_s* node);
node_s* get_prev_dll(node_s* node);
void set_next_dll(node_s* node, node_s* next);
void set_prev_dll(node_s* node, node_s* prev);
void destroy_node_dll(node_s** node);
dll_s* initialize_dll(void);
void insert_between_dll(dll_s* dll, void* payload, node_s* prev, node_s* next);
void insert_first_dll(dll_s* dll, void* payload);
void insert_last_dll(dll_s* dll, void* payload);
void* remove_node_dll(dll_s* dll, node_s* node);
void* remove_first_dll(dll_s* dll);
void* remove_last_dll(dll_s* dll);
void* get_first_dll(dll_s* dll);
void* get_last_dll(dll_s* dll);
void destroy_dll(dll_s** dll);

#endif
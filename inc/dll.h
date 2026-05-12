#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(DLL_TAG) || !defined(DLL_ITEM_TYPE)
#error "Missing type or tag definition"
#endif

#define DLL_CONCAT(TAG, METHOD) TAG##_##METHOD
#define DLL_CONCAT_EXP(TAG, METHOD) DLL_CONCAT(TAG, METHOD)
#define DLL_FN(METHOD) DLL_CONCAT_EXP(DLL_TAG, METHOD)
            
/* Node definition for dll implementation*/ 
typedef struct dll_node {                       
    struct dll_node* next;                      
    struct dll_node* prev;                      
    DLL_ITEM_TYPE payload;                           
} dll_node;                                   
                                            
/*DLL definition with sentinel head and tail*/  
typedef struct DLL_TAG {                            
     dll_node* head;                               
     dll_node* tail;                               
    int size;                                   
} DLL_TAG;                                         
                                                
/*                                              
Create a new node                               
@param payload Pointer to the node payload      
@param prev The previous node                   
@param next The next node                       
@return Pointer to the node                     
*/                                              
static dll_node* initialize_node(DLL_ITEM_TYPE payload,  dll_node* prev,  dll_node* next) {    
     dll_node* node = malloc(sizeof( dll_node));              
    if(!node) {     
        fprintf(stderr, "FATAL: Could not allocate memory for DLL node!");                                             
        return NULL;
    }                                            
    node->payload = payload;                                       
    set_next(node, next);                                    
    set_prev(node, prev);                                    
                                                                 
    return node;                                                 
}                                                                
                                                                 
/*                                       
Get the node data payload                
@param node Node struct                  
@return The data payload from the node   
*/                                       
static DLL_ITEM_TYPE get_payload(dll_node* node) {    
    return node->payload;                                    
}                                                            
                                                             
/*                                                           
Get the next node                                            
@param node The reference node                               
@return The next node                                        
*/                                                           
static dll_node* get_next( dll_node* node) {    
    return node->next;                                           
}                                                                
                                                                 
/*                                   
Get the prev node                    
@param node The reference node       
@return The prev node                
*/                                   
static dll_node* get_prev( dll_node* node) {        
    return node->prev;                                               
}                                                                    
                                                                     
/*                                                       
Set the next node                                        
@param node Node that has its next parameter set         
@param next Node that is pointed to by the attribute     
*/                                                       
static void set_next( dll_node* node,  dll_node* next) {     
    node->next = next;                                                       
}                                                                            
                                                                             
/*                                                           
Set the prev node                                            
@param node Node that has its prev parameter set             
@param prev Node that is pointed to by the attribute         
*/                                                           
static void set_prev( dll_node* node,  dll_node* prev) {     
    node->prev = prev;                                                       
}                                                                            
                                                                             
/*                                               
Destroy a node                                   
@param Pointer to a pointer of a node struct     
*/                                               
static void destroy_node( dll_node** node) {      
    if((*node) == NULL || node == NULL)                              
        return;                                                      
                                                                     
    free((*node));                                                   
    (*node) = NULL;                                                  
}                                                                    
                                                                     
/*                                   
Initialize the dll                   
@return Pointer to a dll struct      
*/                                   
DLL_TAG* DLL_FN(initialize)(void) {    
    DLL_TAG* dll = malloc(sizeof(DLL_TAG));            
    if(dll == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for DLL!");                             
        return NULL; 
    }                            
                                                 
    DLL_ITEM_TYPE dummy_payload;                          
    memset(&dummy_payload, 0, sizeof(DLL_ITEM_TYPE));                             
    dll->head = initialize_node(dummy_payload, NULL, NULL);       
    dll->tail = initialize_node(dummy_payload, NULL, NULL);       
                                                                 
    if (!dll->head || !dll->tail)                                
        return NULL;                                             
                                                                 
    set_prev(dll->tail, dll->head);       
    set_next(dll->head, dll->tail);       
    dll->size = 0;                               
    return dll;                                  
}                                                
                                                 
/*                               
Get the size of the DLL          
@param Pointer to DLL struct     
@return size of DLL              
*/                               
int DLL_FN(get_size)(DLL_TAG* dll) {   
    return dll->size;            
}                                
                                 
/*                                               
Insert a node between two nodes                  
@param dll Pointer to dll struct                 
@param paylaod Pointer to the node payload       
@param prev The node before the inserted node    
@param next The node after the inserted node     
*/                                               
void  DLL_FN(insert_between)(DLL_TAG* dll, DLL_ITEM_TYPE payload,  dll_node* prev,  dll_node* next) {      
     dll_node* node = initialize_node(payload, prev, next);                                             
                                                                                                             
    set_prev(next, node);                                                                             
    set_next(prev, node);                                                                             
    dll->size++;                                                                                             
}                                                                                                            
                                                                                                             
/*                                               
Insert a node right after the sentinel head      
@param dll Pointer to the DLL struct             
@payload Pointer to the node payload             
*/                                               
void DLL_FN(insert_first)(DLL_TAG* dll, DLL_ITEM_TYPE payload) {              
    DLL_FN(insert_between)(dll, payload, dll->head, get_next(dll->head));  
}                                                                                
                                                                                 
/*                                                                               
Insert a node right before the sentinel tail                                     
@param dll Pointer to the DLL struct                                             
@payload Pointer to the node payload                                             
*/                                                                               
void DLL_FN(insert_last)(DLL_TAG* dll, DLL_ITEM_TYPE payload) {                       
    DLL_FN(insert_between)(dll, payload, get_prev(dll->tail), dll->tail);          
}                                                                                        
                                                                                         
/*                                   
Remove a node from the list          
@param dll Pointer to DLL struct     
@param node Node that is removed     
*/                                   
DLL_ITEM_TYPE DLL_FN(remove_node)(DLL_TAG* dll,  dll_node* node) {    
    if(dll->size == 0) {                                             
        fprintf(stderr, "ERROR: DLL is empty, node can't be removed!");              
        return NULL;                                                 
    }                                                                
     dll_node* prev = get_prev(node);                        
     dll_node* next = get_next(node);                        
    DLL_ITEM_TYPE payload = get_payload(node);                            
    set_next(prev, next);                                     
    set_prev(next, prev);                                     
    destroy_node(&node);                                      
    dll->size--;                                                     
    return payload;                                                  
}                                                                    
                                                     
/*                                                   
Remove the first node after the DLL sentinel head    
@param dll Pointer to DLL struct                     
*/                                                   
DLL_ITEM_TYPE DLL_FN(remove_first)(DLL_TAG* dll) {              
    return DLL_FN(remove_node)(dll, get_next(dll->head));     
}                                                                
                                                                 
/*                                                               
Remove the first node before the DLL sentinel tail               
@param dll Pointer to DLL struct                                 
*/                                                               
DLL_ITEM_TYPE DLL_FN(remove_last)(DLL_TAG* dll) {               
    return DLL_FN(remove_node)(dll, get_prev(dll->tail));     
}                                                                
                                                                 
/*                                                               
Get the payload of the first node in the DLL                     
@param dll Pointer to DLL struct                                 
*/                                                               
DLL_ITEM_TYPE DLL_FN(get_first)(DLL_TAG* dll) {                    
    if(dll->size != 0)                                           
        return get_payload(get_next(dll->head));   
    fprintf(stderr, "ERROR: DLL is empty, can't retrieve payload!");             
    return NULL;                                                 
}                                                                
                                                                 
/*                                                               
Get the payload of the first node in the DLL                     
@param dll Pointer to DLL struct                                 
*/                                                               
DLL_ITEM_TYPE DLL_FN(get_last)(DLL_TAG* dll) {                  
    if(dll->size != 0)                                           
        return get_payload(get_prev(dll->tail));      
    fprintf(stderr, "ERROR: DLL is empty, can't retrieve payload!");             
    return NULL;                                                 
}                                                                
                                                                 
/*                                                               
Destroy the DLL and all associated nodes                         
@param dll Pointer to a pointer of DLL struct                    
*/                                                               
void DLL_FN(destroy)(DLL_TAG** dll) {                  
    if (dll == NULL || (*dll) == NULL)                           
        return;                                                  
                                                                 
     dll_node* current = (*dll)->head;                          
    while(current != NULL) {                                     
         dll_node* next = get_next(current);             
        destroy_node(&current);                           
        current = next;                                          
    }                                                                 
    free((*dll));                                                
    (*dll) = NULL;                                               
}

#undef DLL_TAG
#undef DLL_ITEM_TYPE
#undef DLL_CONCAT
#undef DLL_CONCAT_EXP
#undef DLL_FN

#endif
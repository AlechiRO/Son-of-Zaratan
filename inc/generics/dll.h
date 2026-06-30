#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(DLL_TAG) || !defined(DLL_ITEM_TYPE)
#error "Missing type or tag definition"
#endif

#define DLL_CONCAT(TAG, METHOD) TAG##_##METHOD
#define DLL_CONCAT_EXP(TAG, METHOD) DLL_CONCAT(TAG, METHOD)
#define DLL_FN(METHOD) DLL_CONCAT_EXP(DLL_TAG, METHOD)
#define NODE_TAG DLL_CONCAT_EXP(DLL_TAG, node)

            
/* Node definition for dll implementation*/ 
typedef struct NODE_TAG {                       
    struct NODE_TAG* next;                      
    struct NODE_TAG* prev;                      
    DLL_ITEM_TYPE payload;                           
} NODE_TAG;                                   
                                            
/*DLL definition with sentinel head and tail*/  
typedef struct DLL_TAG {                            
     struct NODE_TAG* head;                               
     struct NODE_TAG* tail;                               
    int size;                                   
} DLL_TAG;

/*                                                       
Set the next node                                        
@param node Node that has its next parameter set         
@param next Node that is pointed to by the attribute     
*/                                                       
static inline void DLL_FN(set_next)( NODE_TAG* node,  NODE_TAG* next) {     
    node->next = next;                                                       
}                                                                            
                                                                             
/*                                                           
Set the prev node                                            
@param node Node that has its prev parameter set             
@param prev Node that is pointed to by the attribute         
*/                                                           
static inline void DLL_FN(set_prev)( NODE_TAG* node,  NODE_TAG* prev) {     
    node->prev = prev;                                                       
}   

/*                                              
Create a new node                               
@param payload Pointer to the node payload      
@param prev The previous node                   
@param next The next node                       
@return Pointer to the node                     
*/                                              
static inline NODE_TAG* DLL_FN(initialize_node)(DLL_ITEM_TYPE payload,  NODE_TAG* prev,  NODE_TAG* next) {    
     NODE_TAG* node = malloc(sizeof( NODE_TAG));              
    if(!node) {     
        fprintf(stderr, "FATAL: Could not allocate memory for DLL node!");                                             
        exit(EXIT_FAILURE);
    }                                            
    node->payload = payload;                                       
    DLL_FN(set_next)(node, next);                                    
    DLL_FN(set_prev)(node, prev);                                    
                                                                 
    return node;                                                 
}                                                                
                                                                 
/*                                       
Get the node data payload                
@param node Node struct                  
@return The data payload from the node   
*/                                       
static inline DLL_ITEM_TYPE DLL_FN(get_payload)(NODE_TAG* node) {    
    return node->payload;                                    
}                                                            
                                                             
/*                                                           
Get the next node                                            
@param node The reference node                               
@return The next node                                        
*/                                                           
static inline NODE_TAG* DLL_FN(get_next)( NODE_TAG* node) {    
    return node->next;                                           
}                                                                
                                                                 
/*                                   
Get the prev node                    
@param node The reference node       
@return The prev node                
*/                                   
static inline NODE_TAG* DLL_FN(get_prev)( NODE_TAG* node) {        
    return node->prev;                                               
}                                                                    
                                                                                                                                        
/*                                               
Destroy a node                                   
@param Pointer to a pointer of a node struct     
*/                                               
static inline void DLL_FN(destroy_node)( NODE_TAG** node) {      
    if((*node) == NULL || node == NULL)                              
        return;                                                      
                                                                     
    free((*node));                                                   
    (*node) = NULL;                                                  
}                                                                    
                                                                     
/*                                   
Initialize the dll                   
@return Pointer to a dll struct      
*/                                   
static inline DLL_TAG* DLL_FN(initialize)(void) {    
    DLL_TAG* dll = malloc(sizeof(DLL_TAG));            
    if(dll == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for DLL!");                             
        exit(EXIT_FAILURE); 
    }                            
                                                 
    DLL_ITEM_TYPE dummy_payload;                          
    memset(&dummy_payload, 0, sizeof(DLL_ITEM_TYPE));                             
    dll->head = DLL_FN(initialize_node)(dummy_payload, NULL, NULL);       
    dll->tail = DLL_FN(initialize_node)(dummy_payload, dll->head, NULL);   
    DLL_FN(set_next)(dll->head, dll->tail);    
                                                                 
    if (!dll->head || !dll->tail)                                
        return NULL;                                             
                                                                       
    dll->size = 0;                               
    return dll;                                  
}                                                
                                                 
/*                               
Get the size of the DLL          
@param Pointer to DLL struct     
@return size of DLL              
*/                               
static inline int DLL_FN(get_size)(DLL_TAG* dll) {   
    return dll->size;            
}                                
                                 
/*                                               
Insert a node between two nodes                  
@param dll Pointer to dll struct                 
@param paylaod Pointer to the node payload       
@param prev The node before the inserted node    
@param next The node after the inserted node     
*/                                               
static inline void  DLL_FN(insert_between)(DLL_TAG* dll, DLL_ITEM_TYPE payload,  NODE_TAG* prev,  NODE_TAG* next) {      
     NODE_TAG* node = DLL_FN(initialize_node)(payload, prev, next);                                             
                                                                                                             
    DLL_FN(set_prev)(next, node);                                                                             
    DLL_FN(set_next)(prev, node);                                                                             
    dll->size++;                                                                                             
}                                                                                                            
                                                                                                             
/*                                               
Insert a node right after the sentinel head      
@param dll Pointer to the DLL struct             
@payload Pointer to the node payload             
*/                                               
static inline void DLL_FN(insert_first)(DLL_TAG* dll, DLL_ITEM_TYPE payload) {              
    DLL_FN(insert_between)(dll, payload, dll->head, DLL_FN(get_next)(dll->head));  
}                                                                                
                                                                                 
/*                                                                               
Insert a node right before the sentinel tail                                     
@param dll Pointer to the DLL struct                                             
@payload Pointer to the node payload                                             
*/                                                                               
static inline void DLL_FN(insert_last)(DLL_TAG* dll, DLL_ITEM_TYPE payload) {                       
    DLL_FN(insert_between)(dll, payload, DLL_FN(get_prev)(dll->tail), dll->tail);          
}                                                                                        
                                                                                         
/*                                   
Remove a node from the list          
@param dll Pointer to DLL struct     
@param node Node that is removed     
*/                                   
static inline DLL_ITEM_TYPE DLL_FN(remove_node)(DLL_TAG* dll,  NODE_TAG* node) {    
    if(dll->size == 0) {                                             
        fprintf(stderr, "ERROR: DLL is empty, node can't be removed!");              
        return (DLL_ITEM_TYPE) NULL;                                                 
    }                                                                
     NODE_TAG* prev = DLL_FN(get_prev)(node);                        
     NODE_TAG* next = DLL_FN(get_next)(node);                        
    DLL_ITEM_TYPE payload = DLL_FN(get_payload)(node);                            
    DLL_FN(set_next)(prev, next);                                     
    DLL_FN(set_prev)(next, prev);                                     
    DLL_FN(destroy_node)(&node);                                      
    dll->size--;                                                     
    return payload;                                                  
}                                                                    
                                                     
/*                                                   
Remove the first node after the DLL sentinel head    
@param dll Pointer to DLL struct                     
*/                                                   
static inline DLL_ITEM_TYPE DLL_FN(remove_first)(DLL_TAG* dll) {              
    return DLL_FN(remove_node)(dll, DLL_FN(get_next)(dll->head));     
}                                                                
                                                                 
/*                                                               
Remove the first node before the DLL sentinel tail               
@param dll Pointer to DLL struct                                 
*/                                                               
static inline DLL_ITEM_TYPE DLL_FN(remove_last)(DLL_TAG* dll) {               
    return DLL_FN(remove_node)(dll, DLL_FN(get_prev)(dll->tail));     
}                                                                
                                                                 
/*                                                               
Get the payload of the first node in the DLL                     
@param dll Pointer to DLL struct                                 
*/                                                               
static inline DLL_ITEM_TYPE DLL_FN(get_first)(DLL_TAG* dll) {                    
    if(dll->size != 0)                                           
        return DLL_FN(get_payload)(DLL_FN(get_next)(dll->head));   
    fprintf(stderr, "ERROR: DLL is empty, can't retrieve payload!");             
    return (DLL_ITEM_TYPE) NULL;
}                                                                
                                                                 
/*                                                               
Get the payload of the first node in the DLL                     
@param dll Pointer to DLL struct                                 
*/                                                               
static inline DLL_ITEM_TYPE DLL_FN(get_last)(DLL_TAG* dll) {                  
    if(dll->size != 0)                                           
        return DLL_FN(get_payload)(DLL_FN(get_prev)(dll->tail));      
    fprintf(stderr, "ERROR: DLL is empty, can't retrieve payload!");             
    return (DLL_ITEM_TYPE) NULL;                                                 
}                                                                
                                                                 
/*                                                               
Destroy the DLL and all associated nodes                         
@param dll Pointer to a pointer of DLL struct                    
*/                                                               
static inline void DLL_FN(destroy)(DLL_TAG** dll) {                  
    if (dll == NULL || (*dll) == NULL)                           
        return;                                                  
                                                                 
     NODE_TAG* current = (*dll)->head;                          
    while(current != NULL) {                                     
         NODE_TAG* next = DLL_FN(get_next)(current);             
        DLL_FN(destroy_node)(&current);                           
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
#undef NODE_TAG

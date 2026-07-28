#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
/*
HASHMAP_KEY_TYPE must be a pointer, (e.g. int*, struct node*, char*) so that the hash function works accordingly
*/
#if !defined (HASHMAP_TAG) || !defined(HASHMAP_KEY_TYPE) || !defined(HASHMAP_VALUE_TYPE)
#error "Missing tag or type definition"
#endif

#define HM_CONCAT(TAG, METHOD) TAG##_##METHOD
#define HM_CONCAT_EXP(TAG, METHOD) HM_CONCAT(TAG, METHOD)
#define HM_FN(METHOD) HM_CONCAT_EXP(HASHMAP_LIST_TAG, METHOD)

// Definition of hashmap entry struct
typedef struct {
    HASHMAP_KEY_TYPE key;
    HASHMAP_VALUE_TYPE value;
    size_t key_size;
    size_t value_size;
    short occupied;
    short tombstone;
} HM_CONCAT_EXP(HASHMAP_TAG, entry);

// Definition of hashmap struct
typedef struct HASHMAP_TAG {
    unsigned int size;
    unsigned int capacity;
    HM_CONCAT_EXP(HASHMAP_TAG, entry) entries;
} HASHMAP_TAG;

/*
Table hashing function to map each key to an index
It is about the same as FNV1-a
!! IMPORTANT Structs must be allocated with calloc to avoid hashing slop bytes
@param key Key of a hashmap entry
@return index of the entry in the table
*/
uint32_t hash(HASHMAP_KEY_TYPE key, unsigned int capacity, size_t key_size) {
    if(capacity == 0) 
        return 0;
    const uint8_t* bytes = (const uint8_t*) key;
    uint32_t hash = 0x811C9DC5;

    for(size_t i = 0; i < key_size; i++) {
        hash ^= bytes[i];
        hash *= 0x01000193;
    }

    return (uint32_t)(hash % capacity);
}

/*
Entry Constructor
@return Hashmap entry struct
*/
static inline HM_CONCAT_EXP(HASHMAP_TAG, entry)  HM_FN(initialize_entry)() {
    HM_CONCAT_EXP(HASHMAP_TAG, entry)* entry = malloc(sizeof(HM_CONCAT_EXP(HASHMAP_TAG, entry)));
    entry->key_size = 0;
    entry->value_size = 0;
    entry->occupied = 0;
    entry->tombstone = 0;
    return *entry;
}
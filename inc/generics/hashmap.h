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
@param key Key of a hashmap entry
@return index of the entry in the table
*/
uint32_t hash(HASHMAP_KEY_TYPE* key, unsigned int capacity) {
    if(capacity == 0) 
        return 0;
    const uint8_t* bytes = (const u_int8_t*) key;
    uint32_t hash = 0x811C9DC5;

    for(size_t i = 0; i < sizeof(*key); i++) {
        hash ^= bytes[i];
        hash *= 0x01000193;
    }

    return (uint32_t)(hash % capacity);
}
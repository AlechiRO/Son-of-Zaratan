#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
/*
HASHMAP_KEY_TYPE must be a pointer, (e.g. int*, struct node*, char*) so that the hash function works accordingly
*/
#if !defined (HASHMAP_TAG) || !defined(HASHMAP_KEY_TYPE) || !defined(HASHMAP_VALUE_TYPE)
#error "Missing tag or type definition"
#endif

// Decide if keys or values should be freed
#ifndef HASHMAP_FREE_KEY
  #define HASHMAP_FREE_KEY(k) ((void)(k))
#endif

#ifndef HASHMAP_FREE_VALUE
  #define HASHMAP_FREE_VALUE(v) ((void)(v))
#endif

// Decide how to compare the values | Primitives VS Pointers

#ifndef HASHMAP_EQUAL_VALUE
  // Default comparison between pointers | Structs must be initialized with calloc
  #define HASHMAP_EQUAL_VALUE(a, b, sz) (memcmp((a), (b), (sz)) == 0)
#endif


#define HM_CONCAT(TAG, METHOD) TAG##_##METHOD
#define HM_CONCAT_EXP(TAG, METHOD) HM_CONCAT(TAG, METHOD)
#define HM_FN(METHOD) HM_CONCAT_EXP(HASHMAP_TAG, METHOD)
#define entry_s HM_CONCAT_EXP(HASHMAP_TAG, entry) 

// Definition of hashmap entry struct
typedef struct entry_s {
    HASHMAP_KEY_TYPE key;
    HASHMAP_VALUE_TYPE value;
    size_t key_size;
    size_t value_size;
    short occupied;
    short tombstone;
} entry_s;

// Definition of hashmap struct
typedef struct HASHMAP_TAG {
    unsigned int size;
    unsigned int capacity;
    entry_s* entries;
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
static inline entry_s HM_FN(initialize_entry)() {
    entry_s entry;
    entry.key_size = 0;
    entry.value_size = 0;
    entry.occupied = 0;
    entry.tombstone = 0;
    return entry;
}

/*
Free the fields of an entry and raise a tombstone
@param entry Pointer to a pointer to an entry struct
*/
static inline void HM_FN(kill_entry)(entry_s* entry) {
    if(entry == NULL || !entry->occupied)
        return;

    HASHMAP_FREE_KEY(entry->key);
    HASHMAP_FREE_VALUE(entry->value);

    entry->occupied = 0;
    entry->tombstone = 1;
    entry->key_size = 0;
    entry->value_size = 0;
}

/*
Checks if two values are equal, using byte comparison
@param value_actual Actual value
@param size_actual Size of the actual value
@param value_expected Expected value
@param size_expected Size of the expected value
*/
static inline int HM_FN(equal_values)(HASHMAP_VALUE_TYPE value_actual, size_t size_actual, HASHMAP_VALUE_TYPE value_expected, size_t size_expected) {
    if(size_actual != size_expected)
        return 0;
    return HASHMAP_EQUAL_VALUE(value_actual, value_expected, size_actual);
}

/*
Initialize an array of default entries for the hashmap
@param capacity Capacity of the hashmap
@return Array of entries
*/
static inline entry_s* HM_FN(initialize_entry_array)(size_t capacity) {
    return (entry_s*) calloc(capacity, sizeof(entry_s));
}


/*
Hashmap constructor
@return Pointer to a hashmap struct
*/
static inline HASHMAP_TAG* HM_FN(initialize)() {
    HASHMAP_TAG* map = malloc(sizeof(HASHMAP_TAG));
    map->size = 0;
    map->capacity = 16;
    map->entries = HM_FN(initialize_entry_array)(map->capacity);
    return map;
}

/*
Hashmap destructor
@param map Pointer to a pointer to a hashmap struct
*/
static inline void HM_FN(destroy)(HASHMAP_TAG** map) {
    if(map == NULL || (*map) == NULL)
        return;
    if((*map)->entries != NULL) {
        free((*map)->entries);
        (*map)->entries = NULL;
    }

    free(*map);
    (*map) = NULL;
}

#undef HM_CONCAT
#undef HM_CONCAT_EXP
#undef HM_FN
#undef entry_s
#undef HASHMAP_TAG
#undef HASHMAP_KEY_TYPE
#undef HASHMAP_VALUE_TYPE
#undef HASHMAP_FREE_KEY
#undef HASHMAP_FREE_VALUE
#undef HASHMAP_EQUAL_VALUE
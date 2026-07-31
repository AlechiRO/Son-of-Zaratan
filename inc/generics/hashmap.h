#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

// Decide how to compare the keys | Primitives VS Pointers
#ifndef HASHMAP_EQUAL_KEY
  // Default comparison between memory allocated bytes | Structs must be initialized with calloc
  #define HASHMAP_EQUAL_KEY(a, b, sz) (memcmp((a), (b), (sz)) == 0)
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
    uint32_t size;
    uint32_t capacity;
    entry_s* entries;
} HASHMAP_TAG;

/*
Table hashing function to map each key to an index
It is about the same as FNV1-a
!! IMPORTANT Structs must be allocated with calloc to avoid hashing slop bytes
@param key Key of a hashmap entry
@return index of the entry in the table
*/
static inline uint32_t HM_FN(hash)(HASHMAP_KEY_TYPE key, unsigned int capacity, size_t key_size) {
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
Free the key of an entry and raise a tombstone
@param entry Pointer to a pointer to an entry struct
*/
static inline void HM_FN(kill_entry)(entry_s* entry) {
    if(entry == NULL || !entry->occupied)
        return;

    HASHMAP_FREE_KEY(entry->key);

    entry->occupied = 0;
    entry->tombstone = 1;
    entry->key_size = 0;
    entry->value_size = 0;
}

/*
Checks if two keys are equal, using byte comparison
@param key_actual Actual key
@param size_actual Size of the actual key
@param key_expected Expected key
@param size_expected Size of the expected key
*/
static inline int HM_FN(equal_keys)(HASHMAP_KEY_TYPE key_actual, size_t size_actual, HASHMAP_KEY_TYPE key_expected, size_t size_expected) {
    if(size_actual != size_expected)
        return 0;
    return HASHMAP_EQUAL_KEY(key_actual, key_expected, size_actual);
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
Entry array destructor
@param entries Array of entries
*/
static inline void HM_FN(destroy_entry_array)(entry_s** entries, uint32_t capacity) {
    if(entries == NULL || (*entries) == NULL)
        return;
    
    // Free each key and value from the hashmap
    for(int i = 0; i < capacity; i++) {
        entry_s* entry = &((*entries)[i]);
        if (entry->occupied && !entry->tombstone) {
            HASHMAP_FREE_KEY(entry->key);
            HASHMAP_FREE_VALUE(entry->value);
        }
    }
    // Free the array of entries
    free(*entries);
    *entries = NULL;
}

/*
Hashmap constructor
@return Pointer to a hashmap struct
*/
static inline HASHMAP_TAG* HM_FN(initialize)() {
    HASHMAP_TAG* map = malloc(sizeof(HASHMAP_TAG));

    if(map == NULL){
        fprintf(stderr, "FATAL: could not allocate memory for hashmap struct");
        exit(EXIT_FAILURE);
    }

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

    // Free the keys, values and entire entries from the hashmap
    HM_FN(destroy_entry_array)(&(*map)->entries, (*map)->capacity);

    free(*map);
    (*map) = NULL;
}

/*
Helper function to quickly copy entries when rehashing
This function assumes all keys are unique
@param map Pointer to hashmap struct
@param key Key of the entry
@param value Value of the entry
@param key_size Size of the key
@param value_size Size of the value
*/
static inline void HM_FN(put_rehash)(HASHMAP_TAG* map, HASHMAP_KEY_TYPE key, HASHMAP_VALUE_TYPE value, size_t key_size, size_t value_size) {
    uint32_t index = HM_FN(hash)(key, map->capacity, key_size);
    while((map->entries)[index].occupied)
        index = (index + 1) % map->capacity;
    
    entry_s* entry = &((map->entries)[index]);
    entry->key = key;
    entry->value = value;
    entry->key_size = key_size;
    entry->value_size = value_size;
    entry->occupied = 1;
    entry->tombstone = 0;
}

/*
Increase the hashmap capacity and rehash all entries
@param map Pointer to a hashmap struct
*/
static inline void HM_FN(rehash)(HASHMAP_TAG* map) {
    uint32_t old_capacity = map->capacity;
    // Double the capacity of the map
    map->capacity = old_capacity << 1;

    entry_s* bigger = HM_FN(initialize_entry_array)(map->capacity);
    entry_s* entries = map->entries;
    map->entries = bigger;
    for(int i = 0; i < old_capacity; i++) {
        entry_s entry = entries[i];
        if(entry.occupied && !entry.tombstone) {
            HM_FN(put_rehash)(map, entry.key, entry.value, entry.key_size, entry.value_size);
        }
    }

    free(entries);
}

/*
Put new entry in the hashmap or replace entry value if key already exists in the hashmap
@param map Pointer to hashmap struct
@param key Key of the new entry
@param value Value of the new entry
@param key_size Size of the key
@param value_size Size of the value
*/
static inline void HM_FN(put)(HASHMAP_TAG* map, HASHMAP_KEY_TYPE key, HASHMAP_VALUE_TYPE value, size_t key_size, size_t value_size) {
    // Check if the hashmap would be 75% full with the newly added entry
    if ((map->size + 1) * 4 >= map->capacity * 3)
        HM_FN(rehash)(map);

    uint32_t safety = 0;
    uint32_t index = HM_FN(hash)(key, map->capacity, key_size);
    entry_s* first_tombstone = NULL;
    while(safety < map->capacity) {
        safety++;
        entry_s* entry = &(map->entries[index]);
        if(!entry->occupied) {
            if(first_tombstone != NULL)
                break;
            entry->key = key;
            entry->key_size = key_size;
            entry->value = value;
            entry->value_size = value_size;
            entry->occupied = 1;
            entry->tombstone = 0;
            // Increase the size of the hashmap
            map->size++;
            return;
        }
        else if(entry->occupied && !entry->tombstone && HM_FN(equal_keys)(key, key_size, entry->key, entry->key_size)) {
            // Free old value
            HASHMAP_FREE_VALUE(entry->value);
            // Free new key and keep the old one 
            HASHMAP_FREE_KEY(key);

            entry->value = value;
            entry->value_size = value_size;
            return;
        } else if(entry->tombstone && first_tombstone == NULL) 
            first_tombstone = entry;
        
        index = (index + 1) % map->capacity;
    }
    if(first_tombstone != NULL) {
        first_tombstone->key = key;
        first_tombstone->key_size = key_size;
        first_tombstone->value = value;
        first_tombstone->value_size = value_size;
        first_tombstone->occupied = 1;
        first_tombstone->tombstone = 0;
        // Increase the size of the hashmap
        map->size++;
        return;
    }
    // This scenario should never happen in which there are no tombstones and the hashmap is fully occupied
    return;
}

/*
Helper function to find the index of an entry with a specific key
@param map Pointer to a hashmap struct
@param key Key of the entry we are looking for
@param size Size of the key
@return The position of the entry we are looking for or -1 if the entry was not found
*/
static inline int64_t HM_FN(get_index)(HASHMAP_TAG* map, HASHMAP_KEY_TYPE key, size_t key_size) {
    uint32_t safety = 0;
    uint32_t index = HM_FN(hash)(key, map->capacity, key_size);

    while(safety < map->capacity) {
        safety++;
        entry_s* entry = &(map->entries[index]);

        // If an unoccupied entry is found, then the key-value pair is not in the hashmap
        if(!entry->occupied && !entry->tombstone)
            break;

        if(entry->occupied && !entry->tombstone && HM_FN(equal_keys)(key, key_size, entry->key, entry->key_size))
            return index;
            
        index = (index + 1) % map->capacity;
    }
    // This is only returned if the entry is not found in the hashmap
    return -1;
}

/*
Get the value of the entry with a specific key
@param map Pointer to a hashmap struct
@param key Key corresponding to the value we are looking for
@param key_size Size of the key
@return Value corresponding to the key
*/
static inline HASHMAP_VALUE_TYPE HM_FN(get)(HASHMAP_TAG* map, HASHMAP_KEY_TYPE key, size_t key_size) {
    HASHMAP_VALUE_TYPE empty_value;
    memset(&empty_value, 0, sizeof(HASHMAP_VALUE_TYPE));
    if (map == NULL || map->entries == NULL || map->size == 0) {
        fprintf(stderr, "ERROR: Hashmap is either empty or not yet initialized!\n");
        return empty_value;
    }
    int64_t index = HM_FN(get_index)(map, key, key_size);
    if(index != -1)
        return ((map->entries)[index]).value;
    
    fprintf(stderr, "INFO: Could not find entry in the hashmap!\n");
    return empty_value;
}

/*
Checks if the hashmap contains an entry with a specific key
@param map Pointer to a hashmap struct
@param key Key corresponding to the value we are looking for
@param key_size Size of the key
@return 1 if hashmap contains entry and 0 otherwise
*/
static inline int HM_FN(contains_key)(HASHMAP_TAG* map, HASHMAP_KEY_TYPE key, size_t key_size) {
    return HM_FN(get_index)(map, key, key_size) != -1;
}

/*
Remove an entry from a hashmap, replace it with a tombstone and retrieve the value
@param map Pointer to a hashmap struct
@param key Key corresponding to the value we are looking for
@param key_size Size of the key
*/
static inline HASHMAP_VALUE_TYPE HM_FN(remove)(HASHMAP_TAG* map, HASHMAP_KEY_TYPE key, size_t key_size) {
    HASHMAP_VALUE_TYPE empty_value;
    memset(&empty_value, 0, sizeof(HASHMAP_VALUE_TYPE));
    int64_t index = HM_FN(get_index)(map, key, key_size);
    
    if(index == -1) {
        fprintf(stderr, "INFO: Could not find entry in the hashmap!\n");
        return empty_value;
    }
    entry_s* entry = (&(map->entries)[index]);
    HASHMAP_VALUE_TYPE value = entry->value;

    // Kill the entry and replace with a tombstone
    HM_FN(kill_entry)(entry);
    // Decrease the map size
    map->size--;
    return value;
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
#undef HASHMAP_EQUAL_KEY
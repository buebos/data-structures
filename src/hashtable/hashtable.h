#ifndef __HASH_TABLE_H__

#define __HASH_TABLE_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../util/debug.h"

#ifndef HashTable_ValRef
#define HashTable_ValRef void *
#endif  // !HashTable_ValRef

#ifndef HashTable_KeyRef
#define HashTable_KeyRef void *
#endif  // !HashTable_KeyRef

#define HASHTABLE_DEFAULT_RESIZE_FACTOR 1.5f
#define HASHTABLE_RESIZE_OFFSET 4

typedef struct HashTable HashTable;

typedef size_t (*HashTable_HashFn)(HashTable *table, HashTable_KeyRef key);
typedef void (*HashTable_PrintItemFn)(HashTable_ValRef value);

typedef enum HashTable_KeyType {
    HASHTABLE_KEY_STRING,
    HASHTABLE_KEY_NUMBER,
} HashTable_KeyType;

typedef enum HashTable_SetStatus {
    HASHTABLE_SET_INIT = 0,
    HASHTABLE_SET_PUSHED = 1,
    HASHTABLE_SET_REPLACED,

    HASHTABLE_SET_ERROR,
} HashTable_SetStatus;

typedef enum HashTable_DelStatus {
    HASHTABLE_DEL_ERROR,

    HASHTABLE_DEL_SUCCESS,
    HASHTABLE_DEL_NOT_FOUND,
} HashTable_DelStatus;

typedef enum HashTable_PrintFormat {
    HASHTABLE_PRINT_BUCKETS,
    HASHTABLE_PRINT_FLAT,
} HashTable_PrintFormat;

typedef struct HashTable_Entry {
    HashTable_KeyRef key;
    HashTable_ValRef value;
    struct HashTable_Entry *next;
} HashTable_Entry;

typedef struct HashTable {
    size_t size;

    size_t capacity_initial;
    size_t capacity;

    float resize_factor;
    float lower_load_factor;
    float upper_load_factor;

    HashTable_KeyType key_type;
    bool key_should_free;

    bool value_should_free;

    HashTable_HashFn hash;
    HashTable_PrintItemFn print;

    HashTable_Entry **items;
} HashTable;

HashTable hashtable_new(
    size_t capacity,

    float resize_factor,
    float lower_load_factor,
    float upper_load_factor,

    HashTable_HashFn hash,
    HashTable_PrintItemFn print,
    HashTable_KeyType key_type,
    bool key_should_free,
    bool value_should_free

);

HashTable_SetStatus hashtable_set(HashTable *table, HashTable_KeyRef key, HashTable_ValRef value);

HashTable_DelStatus hashtable_delete(HashTable *table, HashTable_KeyRef key);

HashTable_ValRef hashtable_get(HashTable *table, HashTable_KeyRef key);

bool hashtable_should_resize_grow(HashTable *table);
bool hashtable_should_resize_shrink(HashTable *table);

void hashtable_resize(HashTable *table);

void hashtable_print(HashTable *table, HashTable_PrintFormat format);

void hashtable_empty(HashTable *table);

#endif  // !__HASH_TABLE_H__

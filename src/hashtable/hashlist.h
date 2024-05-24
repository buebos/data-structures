#ifndef __HASH_LIST_H__

#define __HASH_LIST_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../util/debug.h"

#ifndef HashList_ValRef
#define HashList_ValRef void *
#endif  // !HashList_ValRef

#ifndef HashList_KeyRef
#define HashList_KeyRef void *
#endif  // !HashList_KeyRef

#define HASHLIST_DEFAULT_RESIZE_FACTOR 1.5f
#define HASHLIST_RESIZE_OFFSET 4

typedef struct HashList HashList;

typedef size_t (*HashList_HashFn)(HashList *table, HashList_KeyRef key);
typedef void (*HashList_PrintItemFn)(HashList_ValRef value);

typedef enum HashList_KeyType {
    HASHLIST_KEY_STRING,
    HASHLIST_KEY_NUMBER,
} HashList_KeyType;

typedef enum HashList_SetStatus {
    HASHLIST_SET_INIT = 0,
    HASHLIST_SET_PUSHED = 1,
    HASHLIST_SET_REPLACED,

    HASHLIST_SET_ERROR,
} HashList_SetStatus;

typedef enum HashList_DelStatus {
    HASHLIST_DEL_ERROR,

    HASHLIST_DEL_SUCCESS,
    HASHLIST_DEL_NOT_FOUND,
} HashList_DelStatus;

typedef enum HashList_PrintFormat {
    HASHLIST_PRINT_BUCKETS,
    HASHLIST_PRINT_FLAT,
} HashList_PrintFormat;

typedef struct HashList_EntryNode {
    HashList_ValRef value;
    struct HashList_EntryNode *next;
} HashList_EntryNode;

typedef struct HashList_Entry {
    HashList_KeyRef key;
    struct HashList_Entry *next;

    size_t length;
    HashList_EntryNode *head;
    HashList_EntryNode *tail;
} HashList_Entry;

typedef struct HashList {
    size_t size;

    size_t capacity_initial;
    size_t capacity;

    float resize_factor;
    float lower_load_factor;
    float upper_load_factor;

    HashList_KeyType key_type;
    bool key_should_free;

    bool value_should_free;

    HashList_HashFn hash;
    HashList_PrintItemFn print;

    HashList_Entry **items;
} HashList;

HashList hashlist_new(
    size_t capacity,

    float resize_factor,
    float lower_load_factor,
    float upper_load_factor,

    HashList_HashFn hash,
    HashList_PrintItemFn print,
    HashList_KeyType key_type,
    bool key_should_free,
    bool value_should_free

);

HashList_SetStatus hashlist_insert(HashList *table, HashList_KeyRef key, HashList_ValRef value);

HashList_DelStatus hashlist_delete(HashList *table, HashList_KeyRef key);

HashList_Entry *hashlist_get(HashList *hlist, HashList_KeyRef key);

bool hashlist_should_resize_grow(HashList *table);

bool hashlist_should_resize_shrink(HashList *table);

void hashlist_resize(HashList *table);

void hashlist_print(HashList *table, HashList_PrintFormat format);

void hashlist_empty(HashList *table);

#endif  // !__HASH_LIST_H__

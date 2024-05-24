#include "hashlist.h"

void hashlist_print_key(HashList *hlist, HashList_KeyRef key) {
    switch (hlist->key_type) {
        case HASHLIST_KEY_NUMBER:
            printf("%d", *((char *)key));
            break;
        case HASHLIST_KEY_STRING:
        default:
            printf("\"%s\"", key);
            break;
    }
}

bool hashlist_key_is_match(HashList *hlist, HashList_KeyRef a, HashList_KeyRef b) {
    switch (hlist->key_type) {
        case HASHLIST_KEY_NUMBER:
            printdev(
                "Keys matching: %d == %d = %s",
                (*(int *)a),
                (*(int *)b),
                (*(int *)a) == (*(int *)b) ? "TRUE" : "FALSE"

            );

            return (*(int *)a) == (*(int *)b);
        case HASHLIST_KEY_STRING:
        default:
            printdev(
                "Keys matching: %s == %s = %s",
                ((char *)a),
                ((char *)b),
                strcmp(a, b) == 0 ? "TRUE" : "FALSE"

            );

            return strcmp((char *)a, (char *)b) == 0;
    }
}

bool hashlist_should_resize_grow(HashList *hlist) {
    if (!hlist->capacity) {
        return false;
    }

    return hlist->size > hlist->capacity * hlist->upper_load_factor;
}
bool hashlist_should_resize_shrink(HashList *hlist) {
    if (!hlist->capacity) {
        return false;
    }

    return hlist->size < hlist->capacity * hlist->lower_load_factor;
}

size_t hashlist_hash_default(HashList *hlist, HashList_KeyRef key) {
    if (hlist->key_type == HASHLIST_KEY_NUMBER) {
        return (*(int *)key) % hlist->capacity;
    }

    size_t hash = 1;
    char *strkey = (char *)key;

    /** FNV-1a standard prime based folding */
    size_t prime = 0x811c9dc5;
    hash = 0xcbf29ce484222325;

    while (*strkey) {
        hash = hash ^ *strkey;
        hash = hash * prime;
        strkey++;
    }

    hash = hash % hlist->capacity;

    printdev("Index %d", hash);

    return hash;
}

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

) {
    HashList hlist = {
        .size = 0,
        .capacity_initial = capacity,

        .capacity = capacity,

        .resize_factor = resize_factor,
        .lower_load_factor = lower_load_factor,
        .upper_load_factor = upper_load_factor,

        .key_type = key_type,
        .key_should_free = key_should_free,

        .value_should_free = value_should_free,

        .print = print,

        .hash = hash ? hash : hashlist_hash_default,

        .items = calloc(capacity, sizeof(HashList_Entry *)),
    };

    if (
        hashlist_should_resize_grow(&hlist) ||
        hashlist_should_resize_shrink(&hlist)

    ) {
        hashlist_resize(&hlist);
    }

    return hlist;
}

HashList_Entry *hashlist_item_alloc(HashList_KeyRef key, HashList_ValRef value) {
    HashList_Entry *entry = calloc(1, sizeof(HashList_Entry));

    entry->key = key;

    entry->length = 1;
    entry->head = calloc(1, sizeof(HashList_EntryNode));
    entry->tail = entry->head;
    entry->head->value = value;

    return entry;
}

static bool should_resize = true;

HashList_SetStatus hashlist_insert(HashList *hlist, HashList_KeyRef key, HashList_ValRef value) {
    if (!hlist->items || !hlist->capacity) {
        hlist->items = calloc(hlist->resize_factor, sizeof(HashList_Entry *));
        hlist->capacity = hlist->resize_factor;
    }

    size_t index = hlist->hash(hlist, key);

    if (!hlist->items[index]) {
        hlist->items[index] = hashlist_item_alloc(key, value);
        hlist->size += 1;

        if (hashlist_should_resize_grow(hlist) && should_resize) {
            hashlist_resize(hlist);
        }

        debug(
            printf("[DEBUG]: Key set hashlist: ");
            hashlist_print_key(hlist, key);
            printf("\n");

        );

        return HASHLIST_SET_INIT;
    }

    HashList_Entry *prev = NULL;
    HashList_Entry *entry = hlist->items[index];

    while (entry) {
        if (hashlist_key_is_match(hlist, entry->key, key)) {
            entry->tail->next = calloc(1, sizeof(HashList_EntryNode));
            entry->tail = entry->tail->next;
            entry->tail->value = value;

            hlist->size += 1;
            entry->length += 1;

            if (hashlist_should_resize_grow(hlist) && should_resize) {
                hashlist_resize(hlist);
            }

            debug(
                printf("[DEBUG]: Key pushed hashlist: ");
                hashlist_print_key(hlist, key);
                printf("\n");

            );

            return HASHLIST_SET_PUSHED;
        }

        prev = entry;
        entry = entry->next;
    }

    prev->next = hashlist_item_alloc(key, value);
    hlist->size += 1;

    if (hashlist_should_resize_grow(hlist) && should_resize) {
        hashlist_resize(hlist);
    }

    debug(
        printf("[DEBUG]: Key push: ");
        hashlist_print_key(hlist, key);
        printf("\n");

    );

    return HASHLIST_SET_PUSHED;
}

HashList_DelStatus hashlist_delete(HashList *hlist, HashList_KeyRef key) {
    if (!hlist->items || !hlist->capacity) {
        return HASHLIST_DEL_NOT_FOUND;
    }

    size_t index = hlist->hash(hlist, key);

    if (!hlist->items[index]) {
        return HASHLIST_DEL_NOT_FOUND;
    }

    HashList_Entry *prev = NULL;
    HashList_Entry *entry = hlist->items[index];

    while (entry) {
        if (hashlist_key_is_match(hlist, entry->key, key)) {
            debug(
                printf("[DEBUG]: Deleting key: ");
                hashlist_print_key(hlist, entry->key);
                printf("\n");

            );

            if (prev) {
                prev->next = entry->next;
            } else {
                hlist->items[index] = entry->next;
            }

            if (hlist->key_should_free) {
                free(entry->key);
            }

            if (hlist->value_should_free) {
                HashList_EntryNode *node = entry->head;

                while (node) {
                    free(node->value);
                    free(node);

                    hlist->size -= 1;

                    node = node->next;
                }
            }

            free(entry);

            if (hashlist_should_resize_shrink(hlist) && should_resize) {
                hashlist_resize(hlist);
            }

            return HASHLIST_DEL_SUCCESS;
        }

        prev = entry;
        entry = entry->next;
    }

    return HASHLIST_DEL_NOT_FOUND;
};

HashList_Entry *hashlist_get(HashList *hlist, HashList_KeyRef key) {
    if (!hlist->items) {
        return NULL;
    }

    size_t index = hlist->hash(hlist, key);
    HashList_Entry *entry = hlist->items[index];

    while (entry) {
        if (hashlist_key_is_match(hlist, entry->key, key)) {
            printf("[INFO]: Found on index: %zu\n", index);
            return entry;
        }

        entry = entry->next;
    }

    return NULL;
}

void hashlist_resize(HashList *hlist) {
    size_t size_initial = hlist->size;

    size_t capacity_prev = hlist->capacity;
    HashList_Entry **items_prev = hlist->items;

    /**
     * Updating the capacity first so the hashing algorithm
     * gets the correct data.
     */
    hlist->capacity = hlist->size / hlist->upper_load_factor;
    hlist->items = calloc(hlist->capacity, sizeof(HashList_Entry *));

    if (!size_initial) {
        return;
    }

    should_resize = false;

    printdev("Resizing hash hlist: %zu -> %zu\n", capacity_prev, hlist->capacity);

    for (size_t i = 0; i < capacity_prev; i++) {
        HashList_Entry *entry = items_prev[i];

        if (!entry) {
            continue;
        }

        HashList_Entry *prev = NULL;

        while (entry) {
            if (prev) {
                prev->next = NULL;
            }

            HashList_EntryNode *node = entry->head;
            HashList_EntryNode *next = NULL;

            while (node) {
                next = node->next;

                hashlist_insert(hlist, entry->key, node->value);
                hlist->size = size_initial;

                free(node);

                node = next;
            }

            prev = entry;
            entry = entry->next;
        }
    }

    should_resize = true;

    if (items_prev) {
        free(items_prev);
    }
}

void hashlist_print(HashList *hlist, HashList_PrintFormat format) {
    if (!hlist->size) {
        printf("(HashList) { }\n");
        return;
    }

    bool is_first_item = true;

    printf("(HashList) {");

    for (size_t i = 0; i < hlist->capacity; i++) {
        HashList_Entry *entry = hlist->items[i];

        if (!entry) {
            continue;
        }

        if (!is_first_item) {
            printf(",\n\t");
        } else {
            printf("\n\t");
            is_first_item = false;
        }

        while (entry) {
            int count = 1;
            HashList_EntryNode *node = entry->head;

            printf("[%zu]: ", hlist->hash(hlist, entry->key));
            hashlist_print_key(hlist, entry->key);
            printf(" =>\n");

            while (node) {
                printf("\t\t[%d]: ", count);

                hlist->print(node->value);

                if (node->next) {
                    printf("\n");
                }

                count += 1;
                node = node->next;
            }

            if (entry->next) {
                printf(format == HASHLIST_PRINT_BUCKETS ? ", -> " : ",\n\t");
            }

            entry = entry->next;
        }
    }

    printf("\n}\n");
}

void hashlist_empty(HashList *hlist) {
    if (!hlist->size) {
        return;
    }

    for (size_t i = 0; i < hlist->capacity; i++) {
        HashList_Entry *entry = hlist->items[i];

        if (!entry) {
            continue;
        }

        hashlist_delete(hlist, entry->key);
    }

    free(hlist->items);

    hlist->capacity = 0;
}

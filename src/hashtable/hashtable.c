#include "hashtable.h"

void hashtable_print_key(HashTable *table, HashTable_KeyRef key) {
    switch (table->key_type) {
        case HASHTABLE_KEY_NUMBER:
            printf("%d", *((char *)key));
            break;
        case HASHTABLE_KEY_STRING:
        default:
            printf("\"%s\"", key);
            break;
    }
}

bool hashtable_key_is_match(HashTable *table, HashTable_KeyRef a, HashTable_KeyRef b) {
    switch (table->key_type) {
        case HASHTABLE_KEY_NUMBER:
            printdev(
                "Keys matching: %d == %d = %s",
                (*(int *)a),
                (*(int *)b),
                (*(int *)a) == (*(int *)b) ? "TRUE" : "FALSE"

            );

            return (*(int *)a) == (*(int *)b);
        case HASHTABLE_KEY_STRING:
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

bool hashtable_should_resize_grow(HashTable *table) {
    if (!table->capacity) {
        return false;
    }

    return table->size > table->capacity * table->upper_load_factor;
}
bool hashtable_should_resize_shrink(HashTable *table) {
    if (!table->capacity) {
        return false;
    }

    if (table->items[table->capacity - 1] != NULL) {
        return false;
    }

    return table->size < table->capacity * table->lower_load_factor;
}

size_t hashtable_hash_default(HashTable *table, HashTable_KeyRef key) {
    if (table->key_type == HASHTABLE_KEY_NUMBER) {
        return ((*(int *)key) - 1) % table->capacity;
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

    hash = hash % table->capacity;

    printdev("Index %d", hash);

    return hash;
}

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

) {
    HashTable table = {
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

        .hash = hash ? hash : hashtable_hash_default,

        .items = calloc(capacity, sizeof(HashTable_Entry *)),
    };

    if (
        hashtable_should_resize_grow(&table) ||
        hashtable_should_resize_shrink(&table)

    ) {
        hashtable_resize(&table);
    }

    return table;
}

HashTable_Entry *hashtable_item_alloc(HashTable_KeyRef key, HashTable_ValRef value) {
    HashTable_Entry *item = calloc(1, sizeof(HashTable_Entry));

    item->key = key;
    item->value = value;
    return item;
}

static bool should_resize = true;

HashTable_SetStatus hashtable_set(HashTable *table, HashTable_KeyRef key, HashTable_ValRef value) {
    if (!table->items || !table->capacity) {
        table->items = calloc(table->resize_factor, sizeof(HashTable_Entry *));
        table->capacity = table->resize_factor;
    }

    size_t index = table->hash(table, key);

    if (!table->items[index]) {
        table->items[index] = hashtable_item_alloc(key, value);
        table->size += 1;

        if (hashtable_should_resize_grow(table) && should_resize) {
            hashtable_resize(table);
        }

        debug(
            printf("[DEBUG]: Key set hashtable: ");
            hashtable_print_key(table, key);
            printf("\n");

        );

        return HASHTABLE_SET_INIT;
    }

    HashTable_Entry *prev = NULL;
    HashTable_Entry *item = table->items[index];

    while (item) {
        if (hashtable_key_is_match(table, item->key, key)) {
            if (table->key_should_free) {
                free(item->key);
                item->key = key;
            }

            if (table->value_should_free) {
                free(item->value);
            }

            item->value = value;

            debug(
                printf("[DEBUG]: Key update hashtable ");
                hashtable_print_key(table, key);
                printf("\n");

            );

            return HASHTABLE_SET_REPLACED;
        }

        prev = item;
        item = item->next;
    }

    prev->next = hashtable_item_alloc(key, value);
    table->size += 1;

    if (hashtable_should_resize_grow(table) && should_resize) {
        hashtable_resize(table);
    }

    debug(
        printf("[DEBUG]: Key push: ");
        hashtable_print_key(table, key);
        printf("\n");

    );

    return HASHTABLE_SET_PUSHED;
}

HashTable_DelStatus hashtable_delete(HashTable *table, HashTable_KeyRef key) {
    if (!table->items || !table->capacity) {
        return HASHTABLE_DEL_NOT_FOUND;
    }

    size_t index = table->hash(table, key);

    if (!table->items[index]) {
        return HASHTABLE_DEL_NOT_FOUND;
    }

    HashTable_Entry *prev = NULL;
    HashTable_Entry *item = table->items[index];

    while (item) {
        if (hashtable_key_is_match(table, item->key, key)) {
            debug(
                printf("[DEBUG]: Deleting key: ");
                hashtable_print_key(table, item->key);
                printf("\n");

            );

            if (prev) {
                prev->next = item->next;
            } else {
                table->items[index] = item->next;
            }

            if (table->key_should_free) {
                free(item->key);
            }

            if (table->value_should_free) {
                free(item->value);
            }

            free(item);

            table->size -= 1;

            if (hashtable_should_resize_shrink(table) && should_resize) {
                hashtable_resize(table);
            }

            return HASHTABLE_DEL_SUCCESS;
        }

        prev = item;
        item = item->next;
    }

    return HASHTABLE_DEL_NOT_FOUND;
};

HashTable_ValRef hashtable_get(HashTable *table, HashTable_KeyRef key) {
    if (!table->items) {
        return NULL;
    }

    size_t index = table->hash(table, key);
    HashTable_Entry *item = table->items[index];

    while (item) {
        if (hashtable_key_is_match(table, item->key, key)) {
            return item->value;
        }

        item = item->next;
    }

    return NULL;
}

void hashtable_resize(HashTable *table) {
    size_t size_initial = table->size;

    size_t capacity_prev = table->capacity;
    HashTable_Entry **items_prev = table->items;

    /**
     * Updating the capacity first so the hashing algorithm
     * gets the correct data.
     */
    table->capacity = table->size * (1 / table->upper_load_factor);
    table->items = calloc(table->capacity, sizeof(HashTable_Entry *));

    if (!size_initial) {
        return;
    }

    should_resize = false;

    printdev("Resizing hash table: %zu -> %zu\n", capacity_prev, table->capacity);

    for (size_t i = 0; i < capacity_prev; i++) {
        HashTable_Entry *item = items_prev[i];

        if (!item) {
            continue;
        }

        HashTable_Entry *prev = NULL;

        while (item) {
            if (prev) {
                prev->next = NULL;
            }

            hashtable_set(table, item->key, item->value);
            table->size = size_initial;

            prev = item;
            item = item->next;
        }
    }

    should_resize = true;

    if (items_prev) {
        free(items_prev);
    }
}

void hashtable_print(HashTable *table, HashTable_PrintFormat format) {
    if (!table->size) {
        printf("(HashTable) { }\n");
        return;
    }

    bool is_first_item = true;

    printf("(HashTable) {");

    for (size_t i = 0; i < table->capacity; i++) {
        HashTable_Entry *item = table->items[i];

        if (!item) {
            continue;
        }

        if (!is_first_item) {
            printf(",\n\t");
        } else {
            printf("\n\t");
            is_first_item = false;
        }

        while (item) {
            hashtable_print_key(table, item->key);
            printf(" => ");
            table->print(item->value);

            if (item->next) {
                printf(format == HASHTABLE_PRINT_BUCKETS ? ", -> " : ",\n\t");
            }

            item = item->next;
        }
    }

    printf("\n}\n");
}

void hashtable_empty(HashTable *table) {
    if (!table->size) {
        return;
    }

    for (size_t i = 0; i < table->capacity; i++) {
        HashTable_Entry *item = table->items[i];

        if (!item) {
            continue;
        }

        hashtable_delete(table, item->key);
    }

    free(table->items);

    table->capacity = 0;
}

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../util/int.h"
#include "../util/log/log.h"
#include "hashtable.h"

#define APP_HASHTABLE_CAPACITY 20

void test_hashtable_set() {
    HashTable table = hashtable_new(500, NULL, integer_print, HASHTABLE_KEY_STRING, false, true);

    printlog(LOG_DEBUG, "Entering set operation tests");

    HashTable_SetStatus status = hashtable_set(&table, "one", integer(1));
    assert(status == HASHTABLE_SET_INIT);

    status = hashtable_set(&table, "one", integer(2));
    assert(status == HASHTABLE_SET_REPLACED);

    printdev("Table: capacity - size = %zu, capacity * thresh = %zu", table.capacity - table.size, table.capacity * HASHTABLE_RESIZE_LOWER_THRESH);

    for (int i = 0; i < 100; i++) {
        char* key = calloc(10, sizeof(char));

        sprintf(key, "%d", i);

        status = hashtable_set(&table, key, integer(i));

        assert(status == HASHTABLE_SET_INIT || status == HASHTABLE_SET_PUSHED);
        assert(!hashtable_should_resize(&table));
    }

    status = hashtable_set(&table, "string", integer(42));
    assert(status == HASHTABLE_SET_INIT || status == HASHTABLE_SET_PUSHED);

    status = hashtable_set(&table, "100", integer(24));
    assert(status == HASHTABLE_SET_INIT || status == HASHTABLE_SET_PUSHED);

    hashtable_empty(&table);

    printlog(LOG_INFO, "Set tests passed");
}

void test_hashtable_get() {
    HashTable table = hashtable_new(APP_HASHTABLE_CAPACITY, NULL, integer_print, HASHTABLE_KEY_STRING, false, true);

    printlog(LOG_DEBUG, "Entering get operation tests");

    HashTable_ValRef value = hashtable_get(&table, "non-existent");
    assert(value == NULL);

    hashtable_set(&table, "one", integer(1));
    value = hashtable_get(&table, "one");
    assert(*(int*)value == 1);

    hashtable_empty(&table);

    printlog(LOG_INFO, "Get tests passed");
}

void test_hashtable_delete() {
    HashTable table = hashtable_new(0, NULL, integer_print, HASHTABLE_KEY_STRING, false, true);

    printlog(LOG_DEBUG, "Entering delete operation tests");

    // Test delete with non-existent key
    HashTable_DelStatus status = hashtable_delete(&table, "non-existent");
    assert(status == HASHTABLE_DEL_NOT_FOUND);

    // Test delete with existing key
    hashtable_set(&table, "one", integer(1));

    printdev("Deleting one");

    status = hashtable_delete(&table, "one");
    assert(status == HASHTABLE_DEL_SUCCESS);

    printdev("One deleted");

    for (int i = 0; i < 100; i++) {
        char* key = calloc(10, sizeof(char));

        sprintf(key, "%d", i);
        hashtable_set(&table, key, integer(i));
        assert(!hashtable_should_resize(&table));
    }

    printdev("Set 100 keys from 0 to 99");

    for (int i = 0; i < 100; i++) {
        char* key = calloc(10, sizeof(char));

        sprintf(key, "%d", i);
        status = hashtable_delete(&table, key);
        assert(status == HASHTABLE_DEL_SUCCESS);
        assert(!hashtable_should_resize(&table));
    }

    // Cleanup
    hashtable_empty(&table);

    printlog(LOG_INFO, "Delete tests passed");
}

void test_hashtable_resize() {
    HashTable table = hashtable_new(400, NULL, integer_print, HASHTABLE_KEY_STRING, false, true);

    printlog(LOG_DEBUG, "Entering resize tests");

    assert(hashtable_should_resize(&table));

    table.size = 1;
    table.capacity = 2;
    assert(!hashtable_should_resize(&table));

    table.size = 2;
    table.capacity = 10;
    assert(hashtable_should_resize(&table));

    table.size = 0;
    table.capacity = 10;
    assert(hashtable_should_resize(&table));

    table.size = 13;
    table.capacity = table.size * HASHTABLE_DEFAULT_RESIZE_FACTOR;

    printdev("table.size * HASHTABLE_DEFAULT_RESIZE_FACTOR = %zu", table.capacity);

    assert(!hashtable_should_resize(&table));

    printlog(LOG_INFO, "Passed resize tests");
}

int main() {
    test_hashtable_resize();
    test_hashtable_set();
    test_hashtable_get();
    test_hashtable_delete();

    printf("\n");

    printlog(LOG_INFO, "All tests passed!");

    return 0;
}
/**
 * @file heap.h
 * @author Gael Herrera (https://github.com/buebos)
 * @brief Array based heap implementation.
 *
 * @version 0.1
 * @date 2024-05-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Found on: https://gist.github.com/jemmanuel/b8277e7922e9b9947e2f171cc85f1d01
 * as the 'golden ratio' reallocation factor.
 *
 */
#define HEAP_REALLOCATION_FACTOR (1.5f)

#define HEAP_ELEMENT_POINTER_SIZE (sizeof(void *))

typedef enum HeapType {
    HEAP_TYPE_MIN,
    HEAP_TYPE_MAX,
} HeapType;

typedef enum HeapStoreMode {
    /**
     * Will store an array of actual copies from the pointers
     * passed at insertion into the heap.
     */
    HEAP_STORE_COPY,
    /**
     * Will store an array of references to the actual elements,
     * those references will be the pointers passed at insertion
     * into the heap.
     */
    HEAP_STORE_REFERENCE,
} HeapStoreMode;

typedef enum HeapDeleteMode {
    HEAP_DELETE_FREE_REFERENCE,
    HEAP_DELETE_RETURN_REFERENCE,
} HeapDeleteMode;

typedef enum HeapCapacityMode {
    HEAP_CAPACITY_DYNAMIC,
    HEAP_CAPACITY_STATIC,
} HeapCapacityMode;

typedef struct Heap Heap;

typedef short (*HeapDataWeightComparison)(void *a, void *b);
typedef void (*HeapDataPrint)(void *data);
typedef void (*HeapDataIndexUpdate)(Heap *heap, size_t index_new, void *data);

typedef struct HeapData {
    /**
     * Size of a single element that the data pointers actually
     * point to.
     */
    size_t _unit_size;

    HeapDataWeightComparison compare;
    HeapDataPrint print;
    HeapDataIndexUpdate index_update;
} HeapData;

typedef struct Heap {
    HeapType _type;

    /**
     * Whether to shrink or expand the heap when necessary.
     */
    HeapCapacityMode capacity_mode;

    HeapDeleteMode delete_mode;

    HeapStoreMode _store_mode;

    /** The current elements count on the heap. */
    size_t _size;

    /**
     * The capacity of the heap could be altered during
     * it's lifetime if the capacity dynamic flag is set.
     */
    size_t _capacity_current;

    /**
     * Info about the struct of the units of data stored.
     */
    HeapData _data;

    size_t _element_size;

    /**
     * Note that this translates to an array of pointers to
     * the actual element in the heap. That's the double pointer
     * for.
     */
    void **_elements;
} Heap;

/**
 * @brief Create a new heap.
 *
 * @param type The type of heap (min or max).
 * @param capacity_mode Whether the heap capacity should be dynamic or static.
 * @param capacity The initial capacity of the heap.
 * @param data_size The size of a single element in the heap.
 * @return Heap The newly created heap.
 */
Heap heap_new(HeapType type, HeapCapacityMode capacity_mode, HeapDeleteMode delete_mode, HeapStoreMode store_mode, size_t capacity, HeapData data);

/**
 * @brief Insert an element into the heap.
 *
 * @param heap The heap to insert the element into.
 * @param data The element to insert.
 */
void heap_insert(Heap *heap, void *data);

void *heap_delete_index(Heap *heap, size_t index);

typedef bool (*HeapDeletePredicate)(void *data, size_t index, void *ctx);
void *heap_delete_match(Heap *heap, HeapDeletePredicate predicate, void *ctx);

void heap_delete_all(Heap *heap);

/**
 * @brief Toggle the type of the heap (min to max or max to min).
 *
 * @param heap The heap to toggle.
 */
void heap_toggle_type(Heap *heap);

void heap_heapify(Heap *heap, size_t i);
void heap_heapify_recursive(Heap *heap, size_t i);

void heap_arrange(Heap *heap);

void heap_print(Heap *heap);
void heap_print_tree(Heap *heap);

#endif /* __HEAP_H__ */

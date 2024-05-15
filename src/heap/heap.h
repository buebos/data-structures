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

/**
 * Found on: https://gist.github.com/jemmanuel/b8277e7922e9b9947e2f171cc85f1d01
 * as the 'golden ratio' reallocation factor.
 *
 */
#define HEAP_REALLOCATION_FACTOR (1.5f)

#define HEAP_ELEMENT_SIZE (sizeof(void *))

typedef enum HeapType {
    HEAP_TYPE_MIN,
    HEAP_TYPE_MAX,
} HeapType;

typedef enum HeapCapacityStrat {
    HEAP_CAPACITY_DYNAMIC,
    HEAP_CAPACITY_STATIC,
} HeapCapacityStrat;

typedef enum HeapDataWeightRes {
    HEAP_DATA_LT = -1,
    HEAP_DATA_EQ = 0,
    HEAP_DATA_GT = 1,
} HeapDataWeightRes;

typedef HeapDataWeightRes (*HeapDataWeightComparison)(void *a, void *b);
typedef void (*HeapDataPrint)(void *a);

typedef struct Heap {
    HeapType _type;

    /**
     * Whether to shrink or expand the heap when necessary.
     */
    HeapCapacityStrat capacity_strategy;

    /** The current elements count on the heap */
    size_t _size;

    /**
     * The capacity of the heap could be altered during
     * it's lifetime if the capacity dynamic flag is set.
     */
    size_t _capacity_current;

    /**
     * Size of a single element that the data pointers actually
     * point to.
     */
    size_t _data_size;

    HeapDataWeightComparison compare;

    HeapDataPrint print_data;

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
 * @param capacity_strategy Whether the heap capacity should be dynamic or static.
 * @param capacity The initial capacity of the heap.
 * @param data_size The size of a single element in the heap.
 * @return Heap The newly created heap.
 */
Heap heap_new(HeapType type, HeapCapacityStrat capacity_strategy, size_t capacity, size_t data_size, HeapDataPrint print_data, HeapDataWeightComparison compare);

/**
 * @brief Insert an element into the heap.
 *
 * @param heap The heap to insert the element into.
 * @param data The element to insert.
 */
void heap_insert(Heap *heap, void *data);

void *heap_delete(Heap *heap);

/**
 * @brief Toggle the type of the heap (min to max or max to min).
 *
 * @param heap The heap to toggle.
 */
void heap_toggle_type(Heap *heap);

void heap_heapify(Heap *heap, size_t i);
void heap_arrange(Heap *heap);

void heap_print(Heap *heap);

#endif /* __HEAP_H__ */
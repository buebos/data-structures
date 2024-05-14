/**
 * @file heap.c
 * @author Gael Herrera (https://github.com/buebos)
 * @brief Array based heap implemenation.
 *
 * Important notes:
 *
 * 1. Struct members starting with '_' mean they shouldn't be
 * modified directly. Instead use a function to change the property
 * accordingly.
 *
 * @version 0.1
 * @date 2024-05-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdbool.h>
#include <stdlib.h>

/**
 * Found on: https://gist.github.com/jemmanuel/b8277e7922e9b9947e2f171cc85f1d01
 * as the 'golden ratio' reallocation factor.
 *
 */
#define HEAP_REALLOCATION_FACTOR (1.5f)

#define HEAP_ELEMENT_SIZE (sizeof(void *))

typedef enum HeapType {
    HEAP_MIN,
    HEAP_MAX,
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

    HeapDataWeightComparison get_data_weight_comparison;

    /**
     * Note that this translates to an array of pointers to
     * the actual element in the heap. That's the double pointer
     * for.
     */
    void **_elements;
} Heap;

Heap heap_new(
    HeapType type,
    HeapCapacityStrat capacity_strategy,
    size_t capacity,
    size_t data_size

) {
    return (Heap){
        ._type = type,
        .capacity_strategy = capacity_strategy,
        ._size = 0,
        ._capacity_current = capacity,
        ._data_size = data_size,
        ._elements = calloc(capacity, HEAP_ELEMENT_SIZE),
    };
}

size_t heap_get_parent_index(size_t i) {
    return (i - 1) / 2;
}

bool heap_is_relation_sorted(Heap *heap, void *parent, void *child) {
    switch (heap->_type) {
        case HEAP_MAX:
            return heap->get_data_weight_comparison(parent, child) >= HEAP_DATA_EQ;
        case HEAP_MIN:
            return heap->get_data_weight_comparison(parent, child) <= HEAP_DATA_EQ;
        default:
            return false;
    }
}

void heap_bubble_up(Heap *heap, size_t index) {
    if (index == 0) {
        return;
    }

    size_t parent_index = heap_get_parent_index(index);
    bool is_sorted = heap_is_relation_sorted(heap, heap->_elements[parent_index], heap->_elements[index]);

    while (index != 0 && !is_sorted) {
        void *temp = heap->_elements[parent_index];

        heap->_elements[parent_index] = heap->_elements[index];
        heap->_elements[index] = temp;

        index = parent_index;
        parent_index = heap_get_parent_index(index);

        is_sorted = heap_is_relation_sorted(heap, heap->_elements[parent_index], heap->_elements[index]);
    }
}

void heap_insert(Heap *heap, void *data) {
    if (heap->_size >= heap->_capacity_current &&
        heap->capacity_strategy == HEAP_CAPACITY_STATIC

    ) {
        return;
    }

    if (heap->_size >= heap->_capacity_current &&
        heap->capacity_strategy == HEAP_CAPACITY_DYNAMIC

    ) {
        heap->_elements = realloc(
            heap->_elements,
            heap->_capacity_current * HEAP_REALLOCATION_FACTOR * HEAP_ELEMENT_SIZE

        );
    }

    heap->_elements[heap->_size] = data;

    heap_bubble_up(heap, heap->_size);

    heap->_size += 1;
}

void heap_toggle_type(Heap *heap) {
    switch (heap->_type) {
        case HEAP_MIN:
            heap->_type = HEAP_MAX;
            break;
        case HEAP_MAX:
            heap->_type = HEAP_MIN;
            break;
        default:
            heap->_type = HEAP_MIN;
            break;
    }

    // heap_sort(heap);
}

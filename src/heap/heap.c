#include "heap.h"

Heap heap_new(
    HeapType type,
    HeapCapacityStrat capacity_strategy,
    size_t capacity,
    size_t data_size,
    HeapDataPrint print_data,
    HeapDataWeightComparison compare

) {
    return (Heap){
        ._type = type,
        .capacity_strategy = capacity_strategy,
        ._size = 0,
        ._capacity_current = capacity,
        ._data_size = data_size,

        .print_data = print_data,
        .compare = compare,

        ._elements = calloc(capacity, HEAP_ELEMENT_SIZE),
    };
}

size_t heap_get_parent_index(size_t i) {
    return (i - 1) / 2;
}

bool heap_is_relation_sorted(Heap *heap, void *parent, void *child) {
    switch (heap->_type) {
        case HEAP_TYPE_MAX:
            return heap->compare(parent, child) >= HEAP_DATA_EQ;
        case HEAP_TYPE_MIN:
            return heap->compare(parent, child) <= HEAP_DATA_EQ;
        default:
            return false;
    }
}

void heap_heapify(Heap *heap, size_t i) {
    size_t left, right, largest;

    while (i < heap->_size) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        largest = i;

        if (left < heap->_size && !heap_is_relation_sorted(heap, heap->_elements[largest], heap->_elements[left])) {
            largest = left;
        }

        if (right < heap->_size && !heap_is_relation_sorted(heap, heap->_elements[largest], heap->_elements[right])) {
            largest = right;
        }

        if (largest == i) {
            break;
        }

        void *temp = heap->_elements[i];
        heap->_elements[i] = heap->_elements[largest];
        heap->_elements[largest] = temp;

        i = largest;
    }
}
void heap_arrange(Heap *heap) {
    for (int i = heap->_size / 2 - 1; i >= 0; i--) {
        heap_heapify(heap, i);
    }
}

void heap_bubble_up(Heap *heap) {
    size_t index = heap->_size - 1;

    if (index == 0) {
        return;
    }

    size_t parent_index = heap_get_parent_index(index);
    bool is_sorted = heap_is_relation_sorted(heap, heap->_elements[parent_index], heap->_elements[index]);

    while (index > 0 && !is_sorted) {
        void *temp = heap->_elements[parent_index];

        heap->_elements[parent_index] = heap->_elements[index];
        heap->_elements[index] = temp;

        index = parent_index;
        parent_index = heap_get_parent_index(index);

        if (index > 0) {
            is_sorted = heap_is_relation_sorted(heap, heap->_elements[parent_index], heap->_elements[index]);
        }
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
    heap->_size += 1;

    heap_bubble_up(heap);
}

void *heap_delete(Heap *heap) {
    if (heap->_size == 0) {
        return NULL;
    }

    void *root = heap->_elements[0];

    heap->_elements[0] = heap->_elements[heap->_size - 1];

    heap->_size--;

    heap_heapify(heap, 0);

    return root;
}

void heap_toggle_type(Heap *heap) {
    switch (heap->_type) {
        case HEAP_TYPE_MIN:
            heap->_type = HEAP_TYPE_MAX;
            break;
        case HEAP_TYPE_MAX:
            heap->_type = HEAP_TYPE_MIN;
            break;
        default:
            heap->_type = HEAP_TYPE_MIN;
            break;
    }

    heap_arrange(heap);
}

void heap_print(Heap *heap) {
    printf("[");

    for (size_t i = 0; i < heap->_size; i++) {
        heap->print_data(heap->_elements[i]);

        if (i < heap->_size - 1) {
            printf(", ");
        }
    }

    printf("]\n");
}
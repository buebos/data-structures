#include "heap.h"

Heap heap_new(
    HeapType type,
    HeapCapacityMode capacity_mode,
    HeapDeleteMode delete_mode,
    HeapStoreMode store_mode,
    size_t capacity,
    HeapData data

) {
    Heap heap = {
        ._type = type,

        .capacity_mode = capacity_mode,
        .delete_mode = delete_mode,
        ._store_mode = store_mode,

        ._size = 0,
        ._capacity_current = capacity,

        ._data = data,
    };

    switch (store_mode) {
        case HEAP_STORE_REFERENCE:
            heap._element_size = HEAP_ELEMENT_POINTER_SIZE;
            break;
        case HEAP_STORE_COPY:
        default:
            heap._element_size = data._unit_size;
            break;
    }

    heap._elements = calloc(heap._capacity_current, heap._element_size);

    return heap;
}

size_t heap_parent_index(size_t i) {
    return (i - 1) / 2;
}
size_t heap_left_index(size_t i) {
    return 2 * i + 1;
}
size_t heap_right_index(size_t i) {
    return 2 * i + 2;
}

void heap_set(Heap *heap, size_t index, void *data) {
    switch (heap->_store_mode) {
        case HEAP_STORE_REFERENCE:
            heap->_elements[index] = data;
            break;
        case HEAP_STORE_COPY:
        default:
            if (data == NULL) {
                memset(&heap->_elements[index], 0, heap->_element_size);
                break;
            }

            memcpy(&heap->_elements[index], data, heap->_element_size);
            break;
    }

    if (heap->_data.index_update) {
        heap->_data.index_update(heap, index, data);
    }
}

void *heap_get(Heap *heap, size_t index) {
    switch (heap->_store_mode) {
        case HEAP_STORE_REFERENCE:
            return heap->_elements[index];
        case HEAP_STORE_COPY:
        default:
            return &heap->_elements[index];
    }
}

void heap_swap_indexes(Heap *heap, size_t aindex, size_t bindex) {
    void *temp = NULL;
    void *a = heap_get(heap, aindex);
    void *b = heap_get(heap, bindex);

    switch (heap->_store_mode) {
        case HEAP_STORE_REFERENCE:
            temp = a;

            heap->_elements[aindex] = b;
            heap->_elements[bindex] = temp;

            break;
        case HEAP_STORE_COPY:
        default:
            temp = memcpy(
                calloc(1, heap->_element_size),
                a,
                heap->_element_size

            );

            memcpy(a, b, heap->_element_size);
            memcpy(b, temp, heap->_element_size);

            free(temp);
            break;
    }

    if (heap->_data.index_update) {
        heap->_data.index_update(heap, aindex, b);
        heap->_data.index_update(heap, bindex, a);
    }
}

bool heap_is_relation_sorted(Heap *heap, size_t parent_index, size_t child_index) {
    void *parent = heap_get(heap, parent_index);
    void *child = heap_get(heap, child_index);

    switch (heap->_type) {
        case HEAP_TYPE_MAX:
            return heap->_data.compare(parent, child) >= 0;
        case HEAP_TYPE_MIN:
            return heap->_data.compare(parent, child) <= 0;
        default:
            return false;
    }
}

size_t heap_bubble_up(Heap *heap) {
    size_t index = heap->_size - 1;

    if (index == 0) {
        return index;
    }

    size_t parent_index = heap_parent_index(index);
    bool is_sorted = heap_is_relation_sorted(heap, parent_index, index);

    while (!is_sorted) {
        heap_swap_indexes(heap, parent_index, index);

        index = parent_index;
        parent_index = heap_parent_index(index);

        if (index == 0) {
            break;
        }

        is_sorted = heap_is_relation_sorted(heap, parent_index, index);
    }

    return index;
}

void heap_heapify(Heap *heap, size_t root) {
    size_t left, right, parent;

    while (root < heap->_size) {
        left = heap_left_index(root);
        right = heap_right_index(root);
        parent = root;

        if (left < heap->_size && !heap_is_relation_sorted(heap, parent, left)) {
            parent = left;
        }

        if (right < heap->_size && !heap_is_relation_sorted(heap, parent, right)) {
            parent = right;
        }

        if (parent == root) {
            break;
        }

        heap_swap_indexes(heap, root, parent);

        root = parent;
    }
}
void heap_heapify_recursive(Heap *heap, size_t i) {
    size_t left, right, largest;
    left = heap_left_index(i);
    right = heap_right_index(i);
    largest = i;

    if (left < heap->_size && !heap_is_relation_sorted(heap, largest, left)) {
        largest = left;
    }

    if (right < heap->_size && !heap_is_relation_sorted(heap, largest, right)) {
        largest = right;
    }

    if (largest != i) {
        heap_swap_indexes(heap, i, largest);
        heap_heapify(heap, largest);
    }
}

void heap_arrange(Heap *heap) {
    for (int i = heap->_size / 2 - 1; i >= 0; i--) {
        heap_heapify(heap, i);
    }
}

void heap_insert(Heap *heap, void *data) {
    if (heap->_size >= heap->_capacity_current &&
        heap->capacity_mode == HEAP_CAPACITY_STATIC

    ) {
        return;
    }

    if (heap->_size >= heap->_capacity_current &&
        heap->capacity_mode == HEAP_CAPACITY_DYNAMIC

    ) {
        heap->_elements = realloc(
            heap->_elements,
            (heap->_capacity_current * HEAP_REALLOCATION_FACTOR) * heap->_element_size

        );
    }

    heap_set(heap, heap->_size, data);
    heap->_size += 1;

    size_t index = heap_bubble_up(heap);
}

void *heap_delete_index(Heap *heap, size_t index) {
    if (heap->_size == 0) {
        return NULL;
    }

    void *element = NULL;

    switch (heap->delete_mode) {
        case HEAP_DELETE_FREE_REFERENCE:
            /**
             * If the heap is storing copies, that means
             * the free will be handled at the end of the
             * heap lifetime.
             */
            if (heap->_store_mode == HEAP_STORE_COPY) {
                break;
            }

            free(heap_get(heap, index));

            break;
        case HEAP_DELETE_RETURN_REFERENCE:
        default:
            if (heap->_store_mode != HEAP_STORE_COPY) {
                element = heap_get(heap, index);
                break;
            }

            element = memcpy(
                /** Copy to a new dynamically allocated memory section */
                calloc(1, heap->_element_size),
                /** The first element of the heap */
                heap_get(heap, index),
                /** Indicate the size */
                heap->_element_size

            );

            break;
    }

    heap->_size -= 1;
    heap_set(heap, index, heap_get(heap, heap->_size));
    heap_set(heap, heap->_size, NULL);

    heap_heapify(heap, index);

    return element;
}

void *heap_delete_match(Heap *heap, HeapDeletePredicate predicate, void *ctx) {
    for (size_t i = 0; i < heap->_size; i++) {
        void *element = heap_get(heap, i);

        if (predicate(element, i, ctx)) {
            return heap_delete_index(heap, i);
        }
    }

    return NULL;
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

void heap_delete_all(Heap *heap) {
    while (heap->_size) {
        heap_delete_index(heap, 0);
    }
}

void heap_print(Heap *heap) {
    printf("[");

    for (size_t i = 0; i < heap->_size; i++) {
        heap->_data.print(heap_get(heap, i));

        if (i < heap->_size - 1) {
            printf(", ");
        }
    }

    printf("]\n");
}

void heap_print_tree_index(Heap *heap, size_t index, size_t level) {
    size_t left = heap_left_index(index);
    size_t right = heap_right_index(index);

    if (level == 0) {
        printf("ROOT = ");
    }

    heap->_data.print(heap_get(heap, index));

    level += 1;

    if (right < heap->_size) {
        printf("\n");
        for (size_t i = 0; i < level; i++) {
            printf("\t");
        }
        printf("R = ");

        heap_print_tree_index(heap, right, level);
    }

    if (left < heap->_size) {
        printf("\n");
        for (size_t i = 0; i < level; i++) {
            printf("\t");
        }
        printf("L = ");

        heap_print_tree_index(heap, left, level);
    }
}
void heap_print_tree(Heap *heap) {
    if (!heap->_size) {
        printf("ROOT = (NULL)\n");
        return;
    }

    heap_print_tree_index(heap, 0, 0);

    printf("\n");
}

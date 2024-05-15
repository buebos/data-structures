#include <stdio.h>

#include "../../util/int.h"
#include "../heap.h"

HeapDataWeightRes heap_int_compare(void *a, void *b) {
    return (*(int *)a) - (*(int *)b);
}

int main(int argc, char **argv) {
    Heap heap = heap_new(
        HEAP_TYPE_MIN,
        HEAP_CAPACITY_DYNAMIC,
        10,
        sizeof(int),
        print_integer,
        heap_int_compare

    );

    heap_insert(&heap, integer(8));
    heap_insert(&heap, integer(4));
    heap_insert(&heap, integer(5));
    heap_insert(&heap, integer(1));
    heap_insert(&heap, integer(2));
    heap_insert(&heap, integer(10));

    heap_print(&heap);

    heap_toggle_type(&heap);

    heap_print(&heap);

    return 0;
}
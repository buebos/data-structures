#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_ALLOC_ARRAY NULL
#define CONFIG_CIRCULAR_QUEUE true
#define CONFIG_LINEAR_QUEUE false

#define SUCCESS_QUEUE_OPERATION 0

#define ERROR_QUEUE_FULL 1
#define ERROR_QUEUE_EMPTY 2

#define ERROR_NULL_ELEMENT 3
#define ERROR_NULL_ARRAY 4
#define ERROR_NULL_QUEUE 5

#define ERROR_ARRAY_CAPACITY_MISMATCH 6

#define ERROR_CIRCULAR_INDEX_INVALID 7

typedef struct qua {
    size_t length;

    size_t capacity;
    size_t element_size;

    size_t front_index;
    size_t rear_index;

    void *array;

    bool should_free;
    bool should_free_array;

    bool is_circular;
} QUA;

QUA *qua_new(size_t capacity, size_t element_size, void *array, bool is_circular);
QUA qua_new_static(size_t capacity, size_t element_size, void *array, bool is_circular);

int qua_free(QUA *queue);
int qua_enqueue(QUA *queue, void *element);
int qua_dequeue(QUA *queue);

int qua_fifo_toggle_circular(QUA *queue, size_t next_circular_front_index);

int qua_remove_index(QUA *queue, size_t index);

const char *qua_get_result_desc(int status);

#endif /* QUEUE_H */

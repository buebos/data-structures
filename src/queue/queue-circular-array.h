#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_INITIAL_ARRAY NULL

#define SUCCESS_QUEUE_OPERATION 0

#define ERROR_QUEUE_FULL 1
#define ERROR_QUEUE_EMPTY 2

#define ERROR_NULL_ELEMENT 3
#define ERROR_NULL_ARRAY 4
#define ERROR_NULL_QUEUE 5

#define ERROR_ARRAY_CAPACITY_MISMATCH 6

typedef struct qca {
    size_t length;
    size_t capacity;
    size_t element_size;
    size_t front_index;
    size_t rear_index;
    void *array;
    bool should_free;
    bool should_free_array;
} QCA;

QCA *qca_new(size_t capacity, size_t element_size, void *array);
QCA qca_new_static(size_t capacity, size_t element_size, void *array);

int qca_free(QCA *queue);
int qca_enqueue(QCA *queue, void *element);
int qca_dequeue(QCA *queue);

#endif /* QUEUE_H */

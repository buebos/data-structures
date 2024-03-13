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

int qca_init(QCA *queue, size_t capacity, size_t element_size, void *array) {
    queue->capacity = capacity;
    queue->element_size = element_size;

    // if (sizeof(array) != capacity * element_size) {
    //     printf("[ERROR]: Queue array capacity mismatch");
    //     return ERROR_ARRAY_CAPACITY_MISMATCH;
    // }

    if (array == NO_INITIAL_ARRAY) {
        queue->array = calloc(capacity, element_size);
        queue->should_free_array = true;
    } else {
        queue->array = array;
    }

    return SUCCESS_QUEUE_OPERATION;
}

/**
 * @brief
 *
 * @param capacity The max length the queue's array can have.
 * @param element_size The size of an element of the array.
 * @param array A pointer to the array can be passed in to avoid
 * allocation. The size must be equal to the capacity times the
 * element_size passed in.
 *
 * @return QCA*
 */
QCA *qca_new(size_t capacity, size_t element_size, void *array) {
    QCA *queue = calloc(1, sizeof(QCA));
    queue->should_free = true;
    qca_init(queue, capacity, element_size, array);
    return queue;
};

QCA qca_new_static(size_t capacity, size_t element_size, void *array) {
    QCA queue = {0};
    qca_init(&queue, capacity, element_size, array);
    return queue;
}

int qca_free(QCA *queue) {
    if (queue == NULL) {
        return ERROR_NULL_QUEUE;
    }

    if (queue->should_free) {
        free(queue);
    }

    if (queue->should_free_array) {
        free(queue->array);
    }

    return SUCCESS_QUEUE_OPERATION;
}

int qca_enqueue(QCA *queue, void *element) {
    if (queue == NULL) {
        return ERROR_NULL_QUEUE;
    }
    if (element == NULL) {
        return ERROR_NULL_ELEMENT;
    }
    if (queue->array == NULL) {
        return ERROR_NULL_ARRAY;
    }
    if (queue->length >= queue->capacity) {
        return ERROR_QUEUE_FULL;
    }

    memcpy((char *)queue->array + queue->rear_index * queue->element_size, element, queue->element_size / sizeof(char));

    queue->rear_index = (queue->rear_index + 1) % queue->capacity;
    queue->length += 1;

    return SUCCESS_QUEUE_OPERATION;
}

int qca_dequeue(QCA *queue) {
    if (queue == NULL) {
        return ERROR_NULL_QUEUE;
    }
    if (queue->length == 0) {
        return ERROR_QUEUE_EMPTY;
    }

    queue->front_index = (queue->front_index + 1) % queue->capacity;
    queue->length -= 1;

    return SUCCESS_QUEUE_OPERATION;
}

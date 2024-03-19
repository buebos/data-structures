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

int qua_init(QUA *queue, size_t capacity, size_t element_size, void *array, bool is_circular) {
    queue->capacity = capacity;
    queue->element_size = element_size;
    queue->is_circular = is_circular;

    // if (sizeof(array) != capacity * element_size) {
    //     printf("[ERROR]: Queue array capacity mismatch");
    //     return ERROR_ARRAY_CAPACITY_MISMATCH;
    // }

    if (array == CONFIG_ALLOC_ARRAY) {
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
 * @return QUA*
 */
QUA *qua_new(size_t capacity, size_t element_size, void *array, bool is_circular) {
    QUA *queue = calloc(1, sizeof(QUA));
    queue->should_free = true;
    qua_init(queue, capacity, element_size, array, is_circular);
    return queue;
};

QUA qua_new_static(size_t capacity, size_t element_size, void *array, bool is_circular) {
    QUA queue = {0};
    qua_init(&queue, capacity, element_size, array, is_circular);
    return queue;
}

int qua_free(QUA *queue) {
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

int qua_enqueue(QUA *queue, void *element) {
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

    if (queue->is_circular || (!queue->is_circular && queue->rear_index < queue->capacity)) {
        memcpy((char *)queue->array + queue->rear_index * queue->element_size, element, queue->element_size / sizeof(char));

        queue->rear_index = (queue->rear_index + 1) % queue->capacity;
    } else {
        return ERROR_QUEUE_FULL;
    }

    queue->length += 1;

    return SUCCESS_QUEUE_OPERATION;
}

int qua_dequeue(QUA *queue) {
    if (queue == NULL) {
        return ERROR_NULL_QUEUE;
    }
    if (queue->length == 0) {
        return ERROR_QUEUE_EMPTY;
    }

    if (queue->is_circular || (!queue->is_circular && queue->front_index < queue->capacity)) {
        queue->front_index = (queue->front_index + 1) % queue->capacity;
    } else {
        return ERROR_QUEUE_EMPTY;
    }

    queue->length -= 1;

    return SUCCESS_QUEUE_OPERATION;
}

int qua_fifo_toggle_circular(QUA *queue, size_t next_circular_front_index) {
    if (next_circular_front_index >= queue->capacity) {
        return ERROR_CIRCULAR_INDEX_INVALID;
    }

    QUA next_queue = qua_new_static(queue->capacity, queue->element_size, CONFIG_ALLOC_ARRAY, !queue->is_circular);
    size_t last_index = (queue->rear_index) % queue->capacity;

    if (next_queue.is_circular && next_circular_front_index > 0) {
        next_queue.front_index = next_circular_front_index;
    }

    for (size_t i = queue->front_index; i < last_index; i++) {
        size_t index = (queue->front_index + i) % queue->capacity;

        int enqueue_result = qua_enqueue(&next_queue, &(((char *)queue->array)[index]));

        if (enqueue_result != SUCCESS_QUEUE_OPERATION) {
            return enqueue_result;
        }

        int dequeue_result = qua_dequeue(queue);

        if (dequeue_result != SUCCESS_QUEUE_OPERATION) {
            return dequeue_result;
        }
    }

    *queue = next_queue;
}

int qua_remove_index(QUA *queue, size_t index) {
    return SUCCESS_QUEUE_OPERATION;
}

const char *qua_get_result_desc(int status) {
    switch (status) {
        case SUCCESS_QUEUE_OPERATION:
            return "[SUCCESS] Success";
        case ERROR_ARRAY_CAPACITY_MISMATCH:
            return "[ERROR] Array capacity mismatch";
        case ERROR_CIRCULAR_INDEX_INVALID:
            return "[ERROR] Circular index invalid";
        case ERROR_NULL_ARRAY:
            return "[ERROR] Null array";
        case ERROR_NULL_ELEMENT:
            return "[ERROR] Null element";
        case ERROR_NULL_QUEUE:
            return "[ERROR] Null queue";
        case ERROR_QUEUE_EMPTY:
            return "[ERROR] Queue empty";
        case ERROR_QUEUE_FULL:
            return "[ERROR] Queue full";
        default:
            return "[UNKNOWN] Unknown status";
    }
}

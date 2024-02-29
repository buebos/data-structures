#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode {
    void* data;
    struct QueueNode* prev;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;

    size_t length;
    size_t capacity;

    /** If the data is static this should be set to false */
    bool should_free_node_data;

    /** This should indicate the size of the data you plan to store: sizeof(<YOUR_STRUCT>) */
    size_t data_size;
} Queue;

QueueNode* queue_node_new(void* data) {
    QueueNode* node = calloc(1, sizeof(QueueNode));
    node->data = data;
    return node;
}

Queue* queue_new(size_t data_size, size_t capacity, bool should_free_node_data) {
    Queue* queue = calloc(1, sizeof(Queue));

    queue->capacity = capacity;

    return queue;
}

QueueNode* dequeue(Queue* queue);
void queue_free(Queue** queue_address) {
    Queue* queue = *queue_address;

    while (queue->front) {
        dequeue(queue);
    }

    free(queue);

    *queue_address = NULL;
}

QueueNode* enqueue(Queue* queue, void* data) {
    if (queue->length == queue->capacity) {
        return NULL;
    }

    QueueNode* queue_next_node = queue_node_new(data);

    if (!queue->rear) {
        queue->front = queue_next_node;
        queue->rear = queue->front;
    } else {
        queue_next_node->prev = queue->rear;
        queue->rear->next = queue_next_node;
        queue->rear = queue_next_node;
    }

    queue->length += 1;

    return queue->front;
}

QueueNode* dequeue(Queue* queue) {
    if (queue->length < 1) {
        return NULL;
    }

    QueueNode* prev_front = queue->front;

    queue->front = queue->front->next;

    if (queue->should_free_node_data) {
        free(prev_front->data);
    }
    free(prev_front);

    if (queue->front) {
        queue->front->prev = NULL;
    } else {
        queue->rear = NULL;
    }

    queue->length -= 1;

    return queue->front;
}

QueueNode* queue_insert(Queue* queue, size_t target_index, void* data) {
    if (queue->length == queue->capacity) {
        return NULL;
    }
    if (target_index == queue->length) {
        enqueue(queue, data);
        return queue->front;
    }

    Queue* holder = queue_new(queue->data_size, queue->capacity, false);
    /** Hold what the user of this api configured */
    bool should_free_node_data_config = queue->should_free_node_data;

    queue->should_free_node_data = false;

    while (queue->front) {
        enqueue(holder, queue->front->data);
        dequeue(queue);
    }

    size_t count = 0;

    while (holder->front) {
        if (count == target_index) {
            enqueue(queue, data);
        }

        enqueue(queue, holder->front->data);
        dequeue(holder);

        count++;
    }

    queue_free(&holder);

    queue->should_free_node_data = should_free_node_data_config;

    return queue->front;
}

void queue_print(Queue* queue, void print_data(void*)) {
    if (queue == NULL) {
        printf("(Queue)(NULL) NULL\n");
        return;
    }
    if (queue->length == 0) {
        printf("(Queue)(0/%zu) [ ]\n", queue->capacity);
        return;
    }

    Queue* holder = queue_new(queue->data_size, queue->capacity, false);
    /** Hold what the user of this api configured */
    bool should_free_node_data_config = queue->should_free_node_data;

    queue->should_free_node_data = false;

    printf("(Queue)(%zu/%zu) [ ", queue->length, queue->capacity);

    while (queue->front) {
        print_data(queue->front->data);

        enqueue(holder, queue->front->data);
        dequeue(queue);

        if (queue->front) {
            printf(" <~> ");
        }
    }

    printf(" ]\n");

    while (holder->front) {
        enqueue(queue, holder->front->data);
        dequeue(holder);
    }

    queue_free(&holder);

    queue->should_free_node_data = should_free_node_data_config;

    return;
}

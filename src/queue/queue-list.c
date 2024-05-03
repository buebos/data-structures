#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../stack/stack.h"

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

QueueNode* queue_dequeue(Queue* queue);
void queue_free(Queue** queue_address) {
    Queue* queue = *queue_address;

    while (queue->front) {
        queue_dequeue(queue);
    }

    free(queue);

    *queue_address = NULL;
}

void queue_empty(Queue* queue) {
    while (queue->front) {
        queue_dequeue(queue);
    }
}

QueueNode* queue_enqueue(Queue* queue, void* data) {
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

QueueNode* queue_dequeue(Queue* queue) {
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
        queue_enqueue(queue, data);
        return queue->front;
    }

    Queue* holder = queue_new(queue->data_size, queue->capacity, false);
    /** Hold what the user of this api configured */
    bool should_free_node_data_config = queue->should_free_node_data;

    queue->should_free_node_data = false;

    while (queue->front) {
        queue_enqueue(holder, queue->front->data);
        queue_dequeue(queue);
    }

    size_t count = 0;

    while (holder->front) {
        if (count == target_index) {
            queue_enqueue(queue, data);
        }

        queue_enqueue(queue, holder->front->data);
        queue_dequeue(holder);

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
        if (queue->capacity != -1) {
            printf("(Queue)(0/%zu) [ ]\n", queue->capacity);

        } else {
            printf("(Queue)(0/<INFINITE>) [ ]");
        }

        return;
    }

    Queue* holder = queue_new(queue->data_size, queue->capacity, false);
    /** Hold what the user of this api configured */
    bool should_free_node_data_config = queue->should_free_node_data;

    queue->should_free_node_data = false;

    if (queue->capacity != -1) {
        printf("(Queue)(%zu/%zu) [ ", queue->length, queue->capacity);
    } else {
        printf("(Queue)(%zu/<INFINITE>) [ ", queue->length);
    }

    while (queue->front) {
        print_data(queue->front->data);

        queue_enqueue(holder, queue->front->data);
        queue_dequeue(queue);

        if (queue->front) {
            printf(" <~> ");
        }
    }

    printf(" ]\n");

    while (holder->front) {
        queue_enqueue(queue, holder->front->data);
        queue_dequeue(holder);
    }

    queue_free(&holder);

    queue->should_free_node_data = should_free_node_data_config;

    return;
}

void queue_reverse(Queue* queue) {
    if (queue == NULL || queue->length <= 1) {
        return;
    }

    Stack* stack = stack_new(queue->length);

    bool should_free_node_data_config = queue->should_free_node_data;

    queue->should_free_node_data = false;
    stack->should_free_node_data_address = false;

    while (queue->front) {
        stack_push(stack, queue->front->data);
        queue_dequeue(queue);
    }

    while (stack->top) {
        queue_enqueue(queue, stack->top->data);
        stack_pop(stack);
    }

    stack_free(stack);
    queue->should_free_node_data = should_free_node_data_config;
}

void queue_foreach(Queue* queue, void (*callback)(void* data, size_t index, void* ctx), void* ctx) {
    if (queue == NULL || queue->length == 0) {
        return;
    }

    Queue* temp_queue = queue_new(queue->data_size, queue->capacity, false);
    size_t count = 0;

    bool should_free_node_data_config = queue->should_free_node_data;
    queue->should_free_node_data = false;

    while (queue->front) {
        callback(queue->front->data, count++, ctx);
        queue_enqueue(temp_queue, queue->front->data);
        queue_dequeue(queue);
    }

    while (temp_queue->front) {
        queue_enqueue(queue, temp_queue->front->data);
        queue_dequeue(temp_queue);
    }

    queue_free(&temp_queue);
    queue->should_free_node_data = should_free_node_data_config;
}

void* queue_find(Queue* queue, bool (*match_callback)(void* data, void* ctx), void* ctx) {
    if (queue == NULL || queue->length == 0) {
        return NULL;
    }

    void* found_data = NULL;
    Queue* temp_queue = queue_new(queue->data_size, queue->capacity, false);
    bool should_free_node_data_config = queue->should_free_node_data;

    queue->should_free_node_data = false;

    while (queue->front) {
        if (match_callback(queue->front->data, ctx)) {
            found_data = queue->front->data;
        }
        queue_enqueue(temp_queue, queue->front->data);
        queue_dequeue(queue);
    }

    while (temp_queue->front) {
        queue_enqueue(queue, temp_queue->front->data);
        queue_dequeue(temp_queue);
    }

    queue_free(&temp_queue);
    queue->should_free_node_data = should_free_node_data_config;
    return found_data;
}

void queue_filter(Queue* queue, bool (*match_callback)(void* data, void* ctx), void* ctx) {
    if (queue == NULL || queue->length == 0) {
        return;
    }

    Queue* temp_queue = queue_new(queue->data_size, queue->capacity, false);
    bool should_free_node_data_config = queue->should_free_node_data;

    while (queue->front) {
        if (!match_callback(queue->front->data, ctx)) {
            queue_enqueue(temp_queue, queue->front->data);
            queue->should_free_node_data = false;
        } else {
            queue->should_free_node_data = true;
        }

        queue_dequeue(queue);
    }

    while (temp_queue->front) {
        queue_enqueue(queue, temp_queue->front->data);
        queue_dequeue(temp_queue);
    }

    queue_free(&temp_queue);
    queue->should_free_node_data = should_free_node_data_config;
}

void queue_insert_priority(Queue* queue, void* data, short (*get_priority_compare)(void* data, void* b)) {
    if (!queue->front || get_priority_compare(data, queue->rear->data) < 0) {
        queue_enqueue(queue, data);
        return;
    }

    if (queue->length == queue->capacity) {
        return;
    }

    Queue* holder = queue_new(queue->data_size, queue->capacity, false);
    /** Hold what the user of this api configured */
    bool should_free_node_data_config = queue->should_free_node_data;
    bool is_inserted = false;

    queue->should_free_node_data = false;

    while (queue->front) {
        queue_enqueue(holder, queue->front->data);
        queue_dequeue(queue);
    }

    while (holder->front) {
        if (!is_inserted && get_priority_compare(data, holder->front->data) > 0) {
            queue_enqueue(queue, data);
            is_inserted = true;
        }

        queue_enqueue(queue, holder->front->data);
        queue_dequeue(holder);
    }

    queue_free(&holder);

    queue->should_free_node_data = should_free_node_data_config;
}
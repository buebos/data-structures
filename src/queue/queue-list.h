#ifndef __QUEUE__
#define __QUEUE__

#include <stdbool.h>
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

QueueNode* queue_node_new(void* data);

Queue* queue_new(size_t data_size, size_t capacity, bool should_free_node_data);

QueueNode* queue_enqueue(Queue* queue, void* data);
QueueNode* queue_dequeue(Queue* queue);
QueueNode* queue_insert(Queue* queue, size_t target_index, void* data);

void queue_free(Queue** queue_address);
void queue_empty(Queue* queue);

void queue_print(Queue* queue, void print_data(void*));

void queue_reverse(Queue* queue);
void queue_foreach(Queue* queue, void (*callback)(void* data, size_t index, void* ctx), void* ctx);
void* queue_find(Queue* queue, bool (*match_callback)(void* data, void* ctx), void* ctx);
void queue_filter(Queue* queue, bool (*match_callback)(void* data, void* ctx), void* ctx);

void queue_insert_priority(Queue* queue, void* data, short (*get_priority_compare)(void* data, void* b));

#endif /** __QUEUE__ */
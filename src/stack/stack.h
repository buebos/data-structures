#ifndef __STACK__

#define __STACK__

typedef struct StackNode {
    void* data;
    struct StackNode* next;
} StackNode;

typedef struct {
    struct StackNode* top;
    size_t length;
    size_t capacity;

    bool should_free_node_data_address;
} Stack;

Stack* stack_new(size_t capacity);

StackNode* stack_node_new(void* data);

void* stack_push(Stack* stack, void* data);

void* stack_pop(Stack* stack);

void stack_node_free(StackNode* node);

void stack_free(Stack* stack);

void stack_horizontal_print(Stack* stack, void print_data(void*));

void stack_vertical_print(Stack* stack, void print_data(void*));

#endif /** __STACK__ */
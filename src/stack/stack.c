#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

Stack* stack_new(size_t capacity) {
    Stack* stack = (Stack*)calloc(1, sizeof(Stack));

    stack->should_free_node_data_address = true;
    stack->capacity = capacity;

    return stack;
}

StackNode* stack_node_new(void* data) {
    StackNode* node = (StackNode*)calloc(1, sizeof(StackNode));

    node->data = data;

    return node;
}

void stack_node_free(StackNode* node) {
    free(node->data);
    free(node);
}

void* stack_pop(Stack* stack);
void* stack_semi_pop(Stack* stack);
void stack_free(Stack* stack) {
    while (stack->top) {
        if (stack->should_free_node_data_address) {
            stack_pop(stack);
        } else {
            stack_semi_pop(stack);
        }
    }
    free(stack);
}

void* stack_push(Stack* stack, void* data) {
    size_t next_length = stack->length + 1;

    if (next_length > stack->capacity) {
        return NULL;
    }

    StackNode* node = stack_node_new(data);

    node->next = stack->top;
    stack->top = node;

    stack->length += 1;

    return stack->top;
}

void* stack_pop(Stack* stack) {
    if (stack == NULL || stack->length < 1 || stack->top == NULL) {
        return NULL;
    }

    StackNode* prev_top = stack->top;
    stack->top = prev_top->next;

    if (stack->should_free_node_data_address) {
        stack_node_free(prev_top);
    } else {
        free(prev_top);
    }

    stack->length -= 1;

    if (stack->top == NULL) {
        return NULL;
    }

    return stack->top->data;
}

/** Only frees the node */
void* stack_semi_pop(Stack* stack) {
    StackNode* prev_top = stack->top;
    stack->top = stack->top->next;

    free(prev_top);

    stack->length -= 1;

    if (stack->top == NULL) {
        return NULL;
    }

    return stack->top->data;
}

void stack_horizontal_print(Stack* stack, void print_data(void*)) {
    if (stack == NULL) {
        printf("NULL\n");
        return;
    }

    Stack* holder = stack_new(stack->capacity);
    holder->should_free_node_data_address = stack->should_free_node_data_address;

    printf("[ ");

    while (stack->top) {
        print_data(stack->top->data);

        stack_push(holder, stack->top->data);
        stack_semi_pop(stack);

        if (stack->top) {
            printf(" | ");
        }
    }

    printf(" ]\n");

    while (holder->top) {
        stack_push(stack, holder->top->data);
        stack_semi_pop(holder);
    }
    stack_free(holder);
}

void stack_vertical_print(Stack* stack, void print_data(void*)) {
    if (stack == NULL) {
        printf("|\tNULL\t|\n");
        return;
    }

    if (stack->top == NULL && stack->length == 0) {
        printf("[\t\t]\n");
    }

    Stack* holder = stack_new(stack->capacity);
    holder->should_free_node_data_address = stack->should_free_node_data_address;

    while (stack->top) {
        printf("[\t");

        print_data(stack->top->data);

        printf("\t]\n");

        stack_push(holder, stack->top->data);

        stack_semi_pop(stack);
    }

    while (holder->top) {
        stack_push(stack, holder->top->data);
        stack_semi_pop(holder);
    }

    stack_free(holder);
}

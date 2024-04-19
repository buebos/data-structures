#include "trex-stack.h"

#include <stdio.h>

void trex_stack_push(TrexSymbolStack *stack, TrexNode *trex_node) {
    TrexSymbolStackNode *top_next = calloc(1, sizeof(TrexSymbolStackNode));

    /** Init next top to stack it */
    top_next->_trex_node = trex_node;
    top_next->_next = stack->_top;

    stack->_top = top_next;

    stack->_length += 1;
}

TrexNode *trex_stack_pop(TrexSymbolStack *stack) {
    if (stack->_top == NULL) {
        return NULL;
    }

    TrexSymbolStackNode *top_prev = stack->_top;
    TrexNode *trex_node = top_prev->_trex_node;

    stack->_top = top_prev->_next;
    free(top_prev);

    stack->_length -= 1;

    return trex_node;
}

void trex_stack_free(TrexSymbolStack *stack) {
    if (stack == NULL) {
        return;
    }

    TrexSymbolStackNode *current = stack->_top;
    TrexSymbolStackNode *next;

    while (current != NULL) {
        next = current->_next;

        free(current);
        current = next;
    }

    stack->_top = NULL;
    stack->_length = 0;
}
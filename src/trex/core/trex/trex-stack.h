#ifndef __TREX_STACK_H__

#define __TREX_STACK_H__

#include <stdlib.h>

#include "../symbol/symbol.h"
#include "trex-node.h"

typedef struct TrexSymbolStackNode {
    TrexNode *_trex_node;
    struct TrexSymbolStackNode *_next;
} TrexSymbolStackNode;

typedef struct TrexSymbolStack {
    TrexSymbolStackNode *_top;
    size_t _length;
} TrexSymbolStack;

void trex_stack_push(TrexSymbolStack *stack, TrexNode *trex_node);
TrexNode *trex_stack_pop(TrexSymbolStack *stack);

void trex_stack_free(TrexSymbolStack *stack);

#endif  // !__TREX_STACK_H__
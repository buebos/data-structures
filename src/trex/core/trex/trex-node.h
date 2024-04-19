#ifndef __TREX_NODE_H__

#define __TREX_NODE_H__

#include "../symbol/symbol.h"

typedef struct TrexNode {
    TrexSymbol _symbol;
    struct TrexNode *right;
    struct TrexNode *left;
} TrexNode;

#endif  // __TREX_NODE_H__
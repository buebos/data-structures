#ifndef __TREX_H__

#define __TREX_H__

#include "../util/varmap.h"
#include "trex-node.h"
#include "trex-stack.h"

typedef struct {
    char *expression;

    TrexNode *_root;

    TrexVarMap _var_map;

    TrexSymbolStack _operator_stack;
    TrexSymbolStack _operand_stack;

} Trex;

Trex *trex_new(char *expression);

TrexNode *trex_pop_subtree_node(TrexSymbolStack *operators, TrexSymbolStack *operands);

void trex_insert(Trex *trex, TrexSymbol symbol);

float trex_get_result(Trex *trex);
float trex_node_get_result(Trex *trex, TrexNode *node);

void trex_print_expression(TrexNode *node);

void trex_free(Trex *trex);

#endif  // __TREX_H__
#include "trex.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Trex *trex_new(char *expression) {
    Trex *trex = calloc(1, sizeof(Trex));
    trex->expression = expression;
    return trex;
}

TrexNode *trex_node_new(TrexSymbol symbol) {
    TrexNode *node = calloc(1, sizeof(TrexNode));
    node->_symbol = symbol;
    node->_symbol._str = strdup(symbol._str);

    return node;
}

TrexNode *trex_pop_subtree_node(TrexSymbolStack *operators, TrexSymbolStack *operands) {
    TrexNode *node = trex_stack_pop(operators);
    node->right = trex_stack_pop(operands);
    node->left = trex_stack_pop(operands);
    return node;
}

void trex_insert(Trex *trex, TrexSymbol symbol) {
    /*
     * Shortcuts for stack properties within the tree
     */
    TrexSymbolStack *operands = &trex->_operand_stack;
    TrexSymbolStack *operators = &trex->_operator_stack;

    /*
     * Make the new node to push into the stack
     */
    TrexNode *symbol_node = NULL;
    if (symbol._type != CLOSING_BRACE) {
        symbol_node = trex_node_new(symbol);
    }

    switch (symbol._type) {
        case NUMERIC_LITERAL:
            trex_stack_push(operands, symbol_node);
            break;
        case VARIABLE:
            trex_stack_push(operands, symbol_node);
            trex_varmap_set(&trex->_var_map, symbol._str, 0.0f);
            break;
        case OPENING_BRACE:
            trex_stack_push(operators, symbol_node);
            break;
        case CLOSING_BRACE: {
            while (operators->_top != NULL && operators->_top->_trex_node->_symbol._type != OPENING_BRACE) {
                trex_stack_push(operands, trex_pop_subtree_node(operators, operands));
            }

            if (operators->_top != NULL && operators->_top->_trex_node->_symbol._type == OPENING_BRACE) {
                trex_stack_pop(operators);
            }

            break;
        }
        case OPERATOR: {
            while (operators->_top != NULL && operators->_top->_trex_node->_symbol._precedence >= symbol._precedence) {
                trex_stack_push(operands, trex_pop_subtree_node(operators, operands));
            }

            trex_stack_push(operators, symbol_node);

            break;
        }
        case UNDEFINED:
        default:
            break;
    }
}

float trex_node_get_result(Trex *trex, TrexNode *node) {
    if (node == NULL) {
        return 0.0f;
    }

    switch (node->_symbol._type) {
        case NUMERIC_LITERAL:
            return atof(node->_symbol._str);
        case VARIABLE:
            return trex_varmap_get(&trex->_var_map, node->_symbol._str);
        case OPERATOR:
            float left_res = trex_node_get_result(trex, node->left);
            float right_res = trex_node_get_result(trex, node->right);
            return node->_symbol._get_operator_result(left_res, right_res);
        default:
            return 0.0f;
    }
}

float trex_get_result(Trex *trex) {
    return trex_node_get_result(trex, trex->_root);
}

void trex_free_node(TrexNode *node) {
    if (node == NULL) {
        return;
    }

    trex_free_node(node->left);
    trex_free_node(node->right);
    free(node->_symbol._str);
    free(node);
}

void trex_free(Trex *trex) {
    if (trex == NULL) {
        return;
    }

    free(trex->expression);

    trex_free_node(trex->_root);

    trex_varmap_free(&trex->_var_map);

    trex_stack_free(&trex->_operator_stack);

    trex_stack_free(&trex->_operand_stack);

    free(trex);
}

void trex_print_expression(TrexNode *node) {
    if (node == NULL) {
        return;
    }

    if (node->left) {
        printf("(");
    }

    trex_print_expression(node->left);

    if (node->_symbol._type == OPERATOR) {
        printf(" %s ", node->_symbol._str);
    } else {
        printf("%s", node->_symbol._str);
    }

    trex_print_expression(node->right);

    if (node->right) {
        printf(")");
    }
}

bool trex_is_valid(TrexNode *root) {
    if (root == NULL) {
        return true;
    }

    /** Is a brace delimiter */
    if (root->_symbol._type == OPENING_BRACE || root->_symbol._type == CLOSING_BRACE) {
        return false;
    }

    /** Is an operator and leaf of the tree */
    if (root->_symbol._type == OPERATOR && (!root->left || !root->right)) {
        return false;
    }

    /** Is an operand in the leafs of the tree */
    if (
        (root->_symbol._type == VARIABLE || root->_symbol._type == NUMERIC_LITERAL) &&
        (root->left || root->right)

    ) {
        return false;
    }

    /**
     * This called should be optimized automaticaLLy by the compiler, no need to check left result before calling right
     * */
    return trex_is_valid(root->left) && trex_is_valid(root->right);
}

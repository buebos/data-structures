/**
 * @file trex-parser.c
 * @author Gael Herrera (https://github.com/buebos)
 * @brief I think the other files are kinda fine, but this one is pretty
 * bad. I should have use a state machine or something similar for the parser
 * but I don't have time. Thanks to my 3 followers for supporting me :D.
 * @version 0.1
 * @date 2024-04-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "trex-parser.h"

char **operators_map_strings(OperatorArray *operators) {
    char **operator_strings = calloc(operators->_length, sizeof(char *));
    for (size_t i = 0; i < operators->_length; i++) {
        operator_strings[i] = operators->_data[i]._str;
    }
    return operator_strings;
}

Trex *trex_parse_from(char *expression, TokStrArray *skip_strings, OperatorArray *operators) {
    Trex *trex = trex_new(expression);
    Tokenizer tokenizer = tok_new(
        expression,
        /** Delimiter substrings*/
        (TokStrArray){._data = operators_map_strings(operators), ._length = operators->_length},
        /** Skippable substrings */
        *skip_strings,
        16

    );

    char *token = tok_next(&tokenizer);

    TrexSymbol prev = trex_symbol_new(NULL, operators);
    TrexSymbol current = trex_symbol_new(strdup(token), operators);
    TrexSymbol next = trex_symbol_new(strdup(tok_next(&tokenizer)), operators);

    while (token) {
        if (
            current._type == UNDEFINED ||

            (prev._type == OPERATOR &&
             current._type == OPERATOR &&
             (strcmp(current._str, "-") != 0)) ||

            ((current._type == NUMERIC_LITERAL || current._type == VARIABLE) && next._type == OPENING_BRACE)

        ) {
            trex_free(trex);
            free(prev._str);
            free(current._str);

            if (next._str) {
                free(next._str);
            }

            return NULL;
        }

        /** Nasty trick to acommodate for negative signed values on the expression */
        if (
            (!prev._type || (prev._type != NUMERIC_LITERAL && prev._type != VARIABLE)) &&
            (current._str[0] == '-') &&
            (next._type == NUMERIC_LITERAL || next._type == VARIABLE)

        ) {
            trex_insert(trex, trex_symbol_new("(", operators));
            trex_insert(trex, trex_symbol_new("0", operators));
            trex_insert(trex, current);
            trex_insert(trex, next);
            trex_insert(trex, trex_symbol_new(")", operators));

            prev = current;
            current = next;
            next = trex_symbol_new(strdup(tok_next(&tokenizer)), operators);
        } else {
            trex_insert(trex, current);
        }

        if (prev._str) {
            free(prev._str);
        }

        token = next._str;

        prev = current;
        current = next;
        next = trex_symbol_new(strdup(tok_next(&tokenizer)), operators);
    }

    while (trex->_operator_stack._top != NULL) {
        trex_stack_push(&trex->_operand_stack, trex_pop_subtree_node(&trex->_operator_stack, &trex->_operand_stack));
    }

    trex->_root = trex->_operand_stack._top->_trex_node;
    // trex->_root = trex_stack_pop(&trex->_operand_stack);

    return trex;
}
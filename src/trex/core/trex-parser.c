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

bool trex_parser_is_symbol_operand(TrexSymbol *symbol) {
    return symbol->_type == VARIABLE || symbol->_type == NUMERIC_LITERAL;
}

bool trex_parser_is_valid_token_sequence(TrexSymbol *prev, TrexSymbol *current, TrexSymbol *next) {
    if (current->_type == UNDEFINED) {
        return false;
    }

    /**
     * Examples:
     * 1.   *)
     * 2.   + )
     */
    if (current->_type == OPERATOR && next->_type == CLOSING_BRACE) {
        return false;
    }

    /**
     * Examples:
     * 1.   a1
     * 2.   1a
     * 3.   2 (
     * 4.   1(
     * 5.   a(
     */
    if (
        trex_parser_is_symbol_operand(current) &&
        (trex_parser_is_symbol_operand(next) || next->_type == OPENING_BRACE)

    ) {
        return false;
    }

    return true;
}

bool trex_parser_is_sign_sequence(TrexSymbol *prev, TrexSymbol *current, TrexSymbol *next) {
    return (!prev->_type || (prev->_type != NUMERIC_LITERAL && prev->_type != VARIABLE)) &&
           (current->_str[0] == '-') &&
           (next->_type == NUMERIC_LITERAL || next->_type == VARIABLE);
}

Trex *trex_parser_parse_from(char *expression, TokStrArray *skip_strings, OperatorArray *operators) {
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
        if (!trex_parser_is_valid_token_sequence(&prev, &current, &next)) {
            trex_free(trex);
            free(prev._str);
            free(current._str);

            if (next._str) {
                free(next._str);
            }
            

            return NULL;
        }

        /** Nasty trick to acommodate for negative signed values on the expression */
        if (trex_parser_is_sign_sequence(&prev, &current, &next)) {
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

    if (!trex_is_valid(trex->_root)) {
        trex_free(trex);
        return NULL;
    }

    return trex;
}
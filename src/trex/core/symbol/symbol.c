#include "symbol.h"

TrexSymbol trex_symbol_new(char *value, OperatorArray *operators) {
    TrexSymbol symbol = {0};
    char c = 0;
    int count = 0;

    symbol._str = value;
    symbol._precedence = -1;

    if (!value) {
        return symbol;
    }

    if (strcmp(value, "(") == 0) {
        symbol._type = OPENING_BRACE;
        return symbol;
    }
    if (strcmp(value, ")") == 0) {
        symbol._type = CLOSING_BRACE;
        return symbol;
    }

    for (int i = 0; i < operators->_length; i++) {
        Operator *operator= & operators->_data[i];

        if (strcmp(operator->_str, value) == 0) {
            symbol._type = OPERATOR;
            symbol._precedence = operators->_length - i - 1;
            symbol._get_operator_result = operator->_get_result;
            return symbol;
        }
    }

    while ((c = value[count++])) {
        enum TrexSymbolType current_char_type = UNDEFINED;

        if (isdigit(c) || c == '.') {
            current_char_type = NUMERIC_LITERAL;
        } else if (isalpha(c)) {
            current_char_type = VARIABLE;
        } else {
            current_char_type = UNDEFINED;
        }

        if (!symbol._type) {
            symbol._type = current_char_type;
        }

        if (symbol._type && current_char_type != symbol._type) {
            symbol._type = UNDEFINED;
            return symbol;
        }
    }

    return symbol;
}

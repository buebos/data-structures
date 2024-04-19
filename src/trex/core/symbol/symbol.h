#ifndef TREXP_SYMBOL_H
#define TREXP_SYMBOL_H

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "operator.h"

enum TrexSymbolType {
    UNDEFINED,
    OPERATOR,
    NUMERIC_LITERAL,
    VARIABLE,
    OPENING_BRACE,
    CLOSING_BRACE,
};

typedef struct TrexSymbol {
    char *_str;
    enum TrexSymbolType _type;

    short int _precedence;

    GetOperatorResult _get_operator_result;
} TrexSymbol;

TrexSymbol trex_symbol_new(char *value, OperatorArray *operators);

#endif /* TREXP_SYMBOL_H */
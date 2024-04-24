#include "app/components.h"
#include "core/trex-parser.h"
#include "util/operations.h"
#include "util/tokenizer.h"

int main(void) {
    App app = {0};

    char *skips[] = {" ", "\t", "\n"};
    Operator operators[] = {
        (Operator){._str = "(", ._get_result = NULL},
        (Operator){._str = ")", ._get_result = NULL},
        (Operator){._str = "^", ._get_result = trex_pow},
        (Operator){._str = "*", ._get_result = trex_multiply},
        (Operator){._str = "/", ._get_result = trex_divide},
        (Operator){._str = "-", ._get_result = trex_substract},
        (Operator){._str = "+", ._get_result = trex_add},
    };

    TokStrArray skip_str_array = {
        ._data = skips,
        ._length = sizeof(skips) / sizeof(char *),
    };
    OperatorArray operator_array = {
        ._data = operators,
        ._length = sizeof(operators) / sizeof(Operator),
    };

    app.should_run = 1;
    app.operators = &operator_array;
    app.skips = &skip_str_array;
    app.trex = NULL;

    while (app.should_run) {
        MainMenu(&app);
    }

    if (app.trex != NULL) {
        trex_free(app.trex);
    }

    Logout();

    return 0;
}
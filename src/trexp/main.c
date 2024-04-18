#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./tokenizer.h"

char *OPERATORS[] = {"(", ")", "^", "*", "/", "+", "-"};
size_t OPERATORS_LEN = sizeof(OPERATORS) / sizeof(char *);

char *SKIPS[] = {" "};
size_t SKIPS_LEN = sizeof(SKIPS) / sizeof(char *);

int main(void) {
    Tokenizer tok = tok_new(
        "PI * RADIUS ^ 2 ",
        (TokStrArray){._data = OPERATORS, ._length = OPERATORS_LEN}, /** Delimiter strings*/
        (TokStrArray){._data = SKIPS, ._length = SKIPS_LEN},         /** Skippable strings */
        16

    );

    char *token = NULL;

    while ((token = tok_next(&tok))) {
        // TODO: Get the current index of the token like: tok._input._cursor - strlen(token)
        // it is kind of buggy and strange when previous ignore chars are before the current
        // token or delimiter

        // TODO: Add option to skip over the delimiters as tokens. Maybe it would be better to
        // skip them by default and add the opposite option to include them as tok_next results
        printf("[INFO]: %s [%zu]\n", token);
    }

    printf("\n[INFO]: Current token data on tokenizer: %s[%d:%d]\n", tok._token_current._data, tok._token_current._cursor, tok._token_current._size);
}
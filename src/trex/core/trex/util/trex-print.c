#include "./trex-print.h"

void trex_print_recursive(TrexNode *current, size_t current_depth) {
    if (current == NULL || !current->_symbol._str) {
        return;
    }

    if (current_depth == 0) {
        printf("| ROOT = ");
    }

    printf("%s", current->_symbol._str);

    if (current->right) {
        printf("\n");

        for (size_t i = 0; i < current_depth + 1; i++) {
            if (i != current_depth) {
                printf("\t");
                continue;
            }

            printf("\t| R = ");
        }

        trex_print_recursive(current->right, current_depth + 1);
    }

    if (current->left) {
        printf("\n");

        for (size_t i = 0; i < current_depth + 1; i++) {
            if (i != current_depth) {
                printf("\t");
                continue;
            }

            printf("\t| L = ");
        }

        trex_print_recursive(current->left, current_depth + 1);
    }
}
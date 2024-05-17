#include <stdio.h>

#include "../binary-tree.h"

int *integer(int value) {
    int *address = calloc(1, sizeof(int));
    *address = value;
    return address;
}

unsigned int get_integer_weight(void *address) { return *((int *)address); }

void integer_print(void *address) { printf("%d\n", *((int *)address)); }

int main() {
    BT *bts[3] = {bt_new(integer_print, get_integer_weight), bt_new(integer_print, get_integer_weight), bt_new(integer_print, get_integer_weight)};

    for (int i = 0; i < 3; i++) {
        if (i == 2) {
            bt_insert_iterative(bts[i], integer(1));
            bt_insert_iterative(bts[i], integer(1));
        } else {
            bt_insert_iterative(bts[i], integer(5));
        }

        bt_insert_iterative(bts[i], integer(3));
        bt_insert_iterative(bts[i], integer(2));
        bt_insert_iterative(bts[i], integer(4));
        bt_insert_iterative(bts[i], integer(7));
        bt_insert_iterative(bts[i], integer(8));
        bt_insert_iterative(bts[i], integer(6));

        printf("[INFO]: (Tree) | id: %d - size: %zu |\n", i + 1, bts[i]->_length);
        bt_print(bts[i], 0);
        printf("\n");
    }

    printf("[INFO]: Tree 1 and 2 are equal: %s\n", bt_equal(bts[0], bts[1]) ? "TRUE" : "FALSE");
    printf("[INFO]: Tree 1 and 3 are equal: %s", bt_equal(bts[0], bts[2]) ? "TRUE" : "FALSE");

    for (int i = 0; i < 3; i++) {
        bt_free_recursive(bts[i]);
    }

    printf("[INFO]: Freed all trees\n\n");
    printf("[BYE]: See you in space cowboy!\n");

    return 0;
}

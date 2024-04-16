#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../util/clear.h"
#include "../binary-tree.h"

typedef struct {
    BT *main_bt;

    short current_menu;
    bool should_run;
} App;

int *integer(int value) {
    int *address = calloc(1, sizeof(int));
    *address = value;
    return address;
}

unsigned int get_integer_weight(void *address) { return *((int *)address); }

void print_integer(void *address) { printf("%d\n", *((int *)address)); }

void InputToContinue(App *app) {
    printf("\n\n[INPUT]: Type any number to continue: ");
    scanf("%hd", &app->current_menu);
    app->current_menu = 0;
}

int main(void) {
    App app = {0};
    app.main_bt = bt_new(print_integer, get_integer_weight);
    app.should_run = true;
    app.current_menu = 0;

    while (app.should_run) {
        clear();

        printf("--- Bitify ---\n\n");
        printf("[INFO] Select an option to operate on a binary tree:\n\n");
        printf("[1]: Insert a number\n");
        printf("[2]: Get a number's depth\n");
        printf("[3]: Get the size of the tree\n");
        printf("[4]: Get the depth of the tree\n");
        printf("[5]: Print the tree\n");
        printf("[6]: Exit program\n\n");

        printf("[INPUT]: Type the option number: ");
        scanf("%hd", &app.current_menu);

        printf("\n");

        switch (app.current_menu) {
            case 1: {
                int value = 0;

                printf("[INPUT]: Type the number to insert: ");
                scanf("%d", &value);

                bt_insert_iterative(app.main_bt, integer(value));
                break;
            }
            case 2: {
                int value = 0;

                printf("[INPUT]: Type the number to search: ");
                scanf("%d", &value);

                printf("[INFO]: The depth of that data is: %d",
                       bt_get_data_depth(app.main_bt, integer(value)));

                InputToContinue(&app);
                break;
            }
            case 3: {
                printf("[INFO]: The size of the tree is: %zu",
                       bt_get_node_count_recursive(app.main_bt->root));

                InputToContinue(&app);
                break;
            }
            case 4: {
                printf("[INFO]: The depth of the tree is: %zu",
                       bt_get_depth_recursive(app.main_bt->root, 0));

                InputToContinue(&app);
                break;
            }
            case 5: {
                bt_print_recursive(app.main_bt, app.main_bt->root, 0);

                InputToContinue(&app);
                break;
            }
            default:
                app.should_run = false;
                break;
        }
    }

    clear();
    printf("--- Bitify ---\n\n");
    printf("[BYE]: See you in space cowboy!\n\n");

    return 0;
}

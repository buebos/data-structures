#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../util/clear.h"
#include "../dll.h"

int* integer(int value) {
    int* addr = calloc(1, sizeof(int));
    *addr = value;
    return addr;
}

void print_int_element(void* value_addr) {
    printf("%d", *((int*)value_addr));
}

void print_header_lists(DoublyLinkedList** lists) {
    printf("[INFO]: List A is: ");
    dll_print(lists[0], print_int_element);

    printf("\n");
    printf("[INFO]: List B is: ");
    dll_print(lists[1], print_int_element);

    printf("\n");
    printf("[INFO]: List C is: ");
    dll_print(lists[2], print_int_element);

    printf("\n\n");
}

int main() {
    const int LISTS_LEN = 3;
    DoublyLinkedList* lists[3] = {dll(), dll(), dll()};
    bool should_continue = true;

    while (should_continue) {
        DoublyLinkedList* list_target = NULL;
        char list_target_label = 0;
        DLLNode* node_target = NULL;

        char res_char = 0;
        int index = 0;
        int value = 0;

        clear();

        print_header_lists(lists);

        printf("[0]: Insert into list A\n");
        printf("[1]: Delete from list A\n");
        printf("[2]: Insert into list B\n");
        printf("[3]: Delete from list B\n");
        printf("[4]: Insert list A or B into list C\n");
        printf("[5]: Close program\n\n");

        printf("[INPUT]: Type the option to run:\t");
        scanf("%d", &index);

        clear();

        list_target = lists[index < 2 ? 0 : (index < 4 ? 1 : 2)];
        list_target_label = index < 2 ? 'A' : (index < 4 ? 'B' : 'C');

        print_header_lists(lists);

        if (index < 5) {
            printf("[INFO]: Operating on list %c\n", list_target_label);
        }

        switch (index) {
            case 0:
            case 2:
                printf("[INPUT]: Type index to insert in:\t");
                scanf("%d", &index);
                printf("[INPUT]: Type value to insert:\t");
                scanf("%d", &value);
                node_target = dll_insert(list_target, index, integer(value));
                break;
            case 1:
            case 3:
                printf("[INPUT]: Type index to delete:\t");
                scanf("%d", &index);
                node_target = dll_delrange(list_target, index, index);
                break;
            case 4:
                list_target = lists[2];

                printf("[INPUT]: Type the list to cut ('A' | 'B': defaults to A other key):\t");
                scanf(" %c", &res_char);
                printf("[INPUT]: Type index to paste in:\t");
                scanf("%d", &index);

                node_target = dll_cutfrom(lists[res_char == 'B'], list_target, index);
                break;
            default:
                clear();

                for (int i = 0; i < LISTS_LEN; i++) {
                    dll_free(&lists[i]);
                }

                print_header_lists(lists);

                printf("[INFO]: See you space cowboy!\n");
                return 0;
        }

        clear();

        if (node_target == NULL) {
            print_header_lists(lists);

            printf("[ERROR]: Operation failed, please provide an index within 0 and the list length which is: %d\n\n", list_target->length);

            printf("[INPUT]: Type any letter to continue: ");
            scanf(" %c", &res_char);
        }
    }

    return 0;
}
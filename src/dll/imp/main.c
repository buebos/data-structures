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

int main() {
    DoublyLinkedList* list = dll();
    bool should_continue = true;

    while (should_continue) {
        char res = 0;
        int index = 0;
        int value = 0;

        clear();

        printf("[INFO]: List is: ");
        dll_print(list, print_int_element);
        printf("[INFO]: List length is: %d", list->length);

        printf("\n\n");

        printf("[INPUT]: Type index to insert in:\t");
        scanf("%d", &index);

        printf("[INPUT]: Type value to insert:\t");
        scanf("%d", &value);

        clear();

        DLLNode* node = dll_insert(list, index, integer(value));
        if (node == NULL) {
            printf("[ERROR]: Failed to insert, please provide an index within 0 and the list length which is: %d\n\n", list->length);
        }

        printf("[INFO]: Modified list is: ");
        dll_print(list, print_int_element);
        printf("[INFO]: List length is: %d", list->length);

        printf("\n\n");

        printf("[INPUT]: Type 'y' to continue:\t");
        scanf(" %c", &res);

        should_continue = res == *"y";
    }

    printf("\n[INFO]: See you space cowboy!");

    return 0;
}
#include <stdlib.h>
#include <time.h>

#include "../linked_list.h"

int get_random_integer(int lower, int upper) {
    return (rand() %
            (upper - lower + 1)) +
           lower;
}

int* integer(int value) {
    int* value_address = calloc(1, sizeof(int));
    *value_address = value;
    return value_address;
}

void print_int(void* addr, int index, LinkedList* list) {
    printf("%d", *((int*)addr));

    if (index < list->length - 1) {
        printf(" -> ");
    }
}
void printll_int(LinkedList* list) {
    printf("[");
    for_each_node(list, print_int);
    printf("]\n");
}
Node* insert_int_ordered(LinkedList* list, int insert_value) {
    if (list->head == NULL) {
        return append(list, integer(insert_value));
    }
    if (insert_value < *((int*)list->head->value_addr)) {
        unshift(list, integer(insert_value));
        return list->head;
    }

    Node* current = list->head;
    Node* next = current->next;

    for (int i = 0; i < list->length; i++) {
        if (next == NULL) {
            return append(list, integer(insert_value));
        }
        int next_value = *((int*)next->value_addr);

        if (insert_value >= next_value) {
            current = next;
            next = current->next;
            continue;
        }

        Node* insert_node = node(integer(insert_value));

        current->next = insert_node;
        insert_node->next = next;

        list->length += 1;

        return insert_node;
    }

    return NULL;
}

int get_asc_int_priority(void* a, void* b) {
    int va = *((int*)a);
    int vb = *((int*)b);

    return va - vb;
}

int main() {
    srand(time(0));

    LinkedList* list = linked_list(sizeof(int));

    printf("[INFO]: Append 5 on list:\n");
    for (int i = 1; i <= 5; i++) {
        append(list, integer(i));
    }
    printll_int(list);

    printf("\n[INFO]: Remove last 3 on list:\n");
    for (int i = 1; i <= 3; i++) {
        pop(list);
    }
    printll_int(list);

    printf("\n[INFO]: Append and insertion on list:\n");
    append(list, integer(4));
    printll_int(list);
    insert(list, 2, integer(3));
    printll_int(list);

    printf("\n[INFO]: Unshift on list:\n");
    unshift(list, integer(0));
    printll_int(list);

    printf("\n[INFO]: Delete from index 1 to 3 on list:\n");
    delete_index(list, 1, 3);
    printll_int(list);

    printf("\n[INFO]: Empty action on list:\n");
    empty_list(list);
    printll_int(list);

    printf("\n[INFO]: Insertions on list:\n");
    for (int i = 0; i <= 3; i++) {
        insert(list, i, integer(i * 2));
    }
    printll_int(list);
    for (int i = 0; i <= 3; i++) {
        insert(list, i * 2 + 1, integer(i * 2 + 1));
    }
    printll_int(list);

    printf("\n[INFO]: Empty action on list:\n");
    empty_list(list);

    printf("\n[INFO]: Unordered 10 appends on list:\n");
    for (int i = 1; i <= 10; i++) {
        append(list, integer(get_random_integer(0, 100)));
    }
    printll_int(list);

    printf("\n[INFO]: Sorted list:\n");
    sortll(list, get_asc_int_priority);
    printll_int(list);

    printf("\n[INFO]: Ordered insertions on list:\n");
    for (int i = 1; i <= 10; i++) {
        insert_int_ordered(list, get_random_integer(0, 100));
        printll_int(list);
    }

    free_list(&list);
    printf("\n[INFO]: Freed integer list\n");

    return 0;
}
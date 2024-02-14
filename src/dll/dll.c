#include <stdbool.h>
#include <stdlib.h>

typedef struct DLLNode {
    void *value_addr;
    struct DLLNode *next;
    struct DLLNode *prev;
} DLLNode;
typedef struct {
    int length;
    struct DLLNode *head;
    struct DLLNode *tail;
} DoublyLinkedList;

DoublyLinkedList *dll() {
    return (DoublyLinkedList *)calloc(1, sizeof(DoublyLinkedList));
}
DLLNode *dllnode(void *value_addr) {
    return (DLLNode *)calloc(1, sizeof(DLLNode));
}

DLLNode *append(DoublyLinkedList *list, void *value_addr) {
    if (list == NULL) {
        return NULL;
    }
    DLLNode *node = dllnode(value_addr);

    if (list->length == 0) {
        list->head = node;
        list->tail = node;
        list->length = 1;
        return list->head;
    }

    DLLNode *current = list->head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = node;
    node->prev = current;
    list->tail = node;

    list->length += 1;
}

DLLNode *dll_delete_where(DoublyLinkedList *list, bool predicate(void *, int)) {
    if (list == NULL || list->length == 0) {
        return;
    }

    DLLNode *current = list->head;

    for (int i = 0; i < list->length; i++) {
        if (predicate(current->value_addr, i)) {
            if (current->prev == NULL) {
                list->head = current->next;

                if (list->head != NULL) {
                    list->head->prev = NULL;
                }
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }

            free(current);
            list->length -= 1;
        }

        if (current->next == NULL) {
            list->tail = current;
        }

        current = current->next;
    }
}
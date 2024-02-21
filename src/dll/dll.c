#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct DLLNode {
    void *value_addr;
    struct DLLNode *next;
    struct DLLNode *prev;
} DLLNode;
typedef struct {
    size_t length;
    struct DLLNode *head;
    struct DLLNode *tail;
} DoublyLinkedList;

DoublyLinkedList *dll() {
    return (DoublyLinkedList *)calloc(1, sizeof(DoublyLinkedList));
}
DLLNode *dllnode(void *value_addr) {
    DLLNode *node = (DLLNode *)calloc(1, sizeof(DLLNode));
    node->value_addr = value_addr;
    return node;
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
DLLNode *unshift(DoublyLinkedList *list, void *value_addr) {
    DLLNode *next_head = dllnode(value_addr);

    if (list->head->next == NULL) {
        list->tail = list->head;
    }

    next_head->next = list->head;
    list->head->prev = next_head;
    list->head = next_head;

    list->length += 1;

    return list->head;
}

DLLNode *insertdll(DoublyLinkedList *list, int index, void *value_addr) {
    if (list == NULL) {
        printf("[WARN]: Attempted insert for list on address: %p but it's freed\n", list);
        return NULL;
    }
    if (index < 0) {
        return NULL;
    }
    if (index > list->length) {
        return NULL;
    }

    if (index == list->length) {
        return append(list, value_addr);
    }
    if (list->length >= 0 && index == 0) {
        return unshift(list, value_addr);
    }

    DLLNode *current = list->head;
    int count = 1;

    while (count < index) {
        current = current->next;

        if (current == NULL) {
            return NULL;
        }

        count += 1;
    }

    DLLNode *next = current->next;

    current->next = dllnode(value_addr);
    current->next->next = next;
    next->prev = current->next;

    list->length += 1;

    return current->next;
}

DLLNode *dll_delete_where(DoublyLinkedList *list, bool predicate(void *, int)) {
    if (list == NULL || list->length == 0) {
        return NULL;
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

void printdll(DoublyLinkedList *list, void print_element(void *)) {
    printf("[ ");

    DLLNode *node = list->head;

    while (node != NULL) {
        print_element(node->value_addr);

        node = node->next;

        if (node) {
            printf(" <-> ");
        }
    }

    printf(" ]\n");
}

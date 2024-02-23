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

DLLNode *delrangedll(DoublyLinkedList *list, int from, int to) {
    if (list == NULL) {
        printf("[WARN]: Attempted delete for list on address: %p but it's freed\n", list);
        return NULL;
    }
    int max_index = list->length - 1;

    if (from < 0 || to < 0 || from > to || from > max_index || to > max_index) {
        printf("[ERROR]: Wrong index access for list on address: %p (from: %d, to: %d)\n", list, from, to);
        return NULL;
    }

    DLLNode *current = list->head;
    DLLNode *prev = NULL;

    for (int i = 0; i <= to; i++) {
        if (i < from) {
            prev = current;
            current = current->next;
            continue;
        }

        DLLNode *next = current->next;

        if (prev != NULL) {
            prev->next = next;
            if (next) {
                next->prev = prev;
            }
        } else {
            list->head = next;
            if (next) {
                next->prev = NULL;
            }
        }

        free(current->value_addr);
        free(current);

        current = next;
    }

    list->length -= (to - from) + 1;

    if (current == NULL) {
        return (DLLNode *)0x1;
    }

    return current;
}

DLLNode *delwheredll(DoublyLinkedList *list, bool predicate(void *, int)) {
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

DLLNode *dllcutfrom(DoublyLinkedList *list_source, DoublyLinkedList *list_target, int target_index) {
    if (list_source == NULL || list_target == NULL) {
        return NULL;
    }
    /** Out of range */
    if (target_index < 0 || target_index > list_target->length) {
        return NULL;
    }
    /** There is nothing to cut from the list */
    if (list_source->head == NULL || list_source->tail == NULL) {
        return list_target->head;
    }

    /** Reach the desired node */
    DLLNode *target_node = list_target->head;

    /** Means it should do an unshift */
    if (target_index == 0 || list_target->tail == NULL) {
        /** Handles unshift modifying heads and tails of lists */
        DLLNode *target_head = list_target->head;

        list_target->head = list_source->head;

        list_source->tail->next = target_head;

        if (target_head != NULL) {
            target_head->prev = list_source->tail;
        }
        if (list_target->tail == NULL) {
            list_target->tail = list_source->tail;
        }

        target_node = list_target->head;
    } else if (target_index == list_target->length) {
        /** Handles append modifying heads of lists */
        target_node = list_target->tail;

        list_target->tail->next = list_source->head;

        /** Do not need to check if source head is null again */
        list_source->head->prev = list_target->tail;
        list_source->tail = list_target->head;
    } else {
        int count = 1;
        while (count < target_index) {
            target_node = target_node->next;

            if (target_node == NULL) {
                return NULL;
            }

            count += 1;
        }

        /**
         * Hold the one that was the next index on the list
         * to append it on the last node of the source list
         */
        DLLNode *target_node_next = target_node->next;

        /**
         * Links the node at the target index and the start
         * of the source list
         */
        target_node->next = list_source->head;
        list_source->head->prev = target_node;

        /**
         * Links the node that was next on the target list
         * to the end of the source list
         */
        target_node_next->prev = list_source->tail;
        list_source->tail->next = target_node_next;
    }

    list_target->length += list_source->length;

    list_source->head = NULL;
    list_source->tail = NULL;
    list_source->length = 0;

    return target_node;
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

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void* value_addr;
    struct Node* next;
} Node;
typedef struct {
    int length;
    int node_value_size;
    struct Node* head;
} LinkedList;

Node* node(void* value_addr) {
    Node* node = calloc(sizeof(Node), 1);
    node->value_addr = value_addr;
    return node;
}

LinkedList* linked_list(int node_value_size) {
    LinkedList* list = calloc(sizeof(LinkedList), 1);
    list->node_value_size = node_value_size;
    return list;
}

/**
 * Adds the new value_addr to the end of the list
 * and returns a pointer to that node.
 */
Node* append(LinkedList* list, void* value_addr) {
    if (list == NULL) {
        printf("[WARN] Attempted to append node for list on address: %p but the list it's freed\n", list);
        return NULL;
    }

    Node* last = list->head;

    if (last == NULL) {
        list->head = node(value_addr);
        list->length += 1;
        return list->head;
    }

    while (last->next != NULL) {
        last = last->next;
    }

    last->next = node(value_addr);
    /** This means the node could not be allocated */
    if (last->next == NULL) {
        return NULL;
    }

    list->length += 1;

    return last->next;
}

/** Removes the last node and returns the new length */
int remove_last(LinkedList* list) {
    if (list == NULL) {
        printf("[WARN] Attempted to remove last node for list on address: %p but it's freed\n", list);
        return -1;
    }

    if (list->head == NULL) {
        return 0;
    }
    if (list->head->next == NULL) {
        free(list->head->value_addr);
        free(list->head);
        list->head = NULL;
        return 0;
    }

    Node* prev = list->head;
    Node* current = list->head->next;

    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }
    free(current->value_addr);
    free(current);
    prev->next = NULL;

    list->length -= 1;

    return list->length;
}

void unshift(LinkedList* list, void* value_addr) {
    Node* head = node(value_addr);

    head->next = list->head;
    list->head = head;

    list->length += 1;
}

Node* insert(LinkedList* list, int index, void* value_addr) {
    if (list == NULL) {
        printf("[WARN] Attempted insert for list on address: %p but it's freed\n", list);
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
        unshift(list, value_addr);
        return list->head;
    }

    Node* current = list->head;
    int count = 1;

    while (count < index) {
        current = current->next;

        if (current == NULL) {
            return NULL;
        }

        count += 1;
    }

    Node* next = current->next;

    current->next = node(value_addr);
    current->next->next = next;
    list->length += 1;

    return current->next;
}

void empty_list(LinkedList* list) {
    if (list == NULL) {
        printf("[WARN] Attempted to empty list on address: %p but it's freed\n", list);
        return;
    }

    if (list->head == NULL) {
        return;
    }

    Node* current = list->head;
    Node* next = current->next;

    while (current != NULL) {
        free(current->value_addr);
        free(current);
        current = next;

        if (next) {
            next = next->next;
        }
    }

    list->length = 0;
    list->head = NULL;
}

void free_ll(LinkedList** list) {
    if (list == NULL) {
        printf("[WARN] Attempted to delete list on address: %p but it's freed\n", list);
        return;
    }

    empty_list(*list);
    free(*list);

    (*list) = NULL;
}

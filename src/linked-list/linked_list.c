#include <stdbool.h>
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
        printf("[WARN]: Attempted to append node for list on address: %p but the list it's freed\n", list);
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
Node* pop(LinkedList* list) {
    if (list == NULL) {
        printf("[WARN]: Attempted to remove last node for list on address: %p but it's freed\n", list);
        return NULL;
    }
    if (list->head == NULL) {
        return NULL;
    }
    if (list->head->next == NULL) {
        free(list->head->value_addr);
        free(list->head);
        list->head = NULL;
        return NULL;
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

    return prev;
}

void unshift(LinkedList* list, void* value_addr) {
    Node* head = node(value_addr);

    head->next = list->head;
    list->head = head;

    list->length += 1;
}

Node* insert(LinkedList* list, int index, void* value_addr) {
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
Node* delete_index(LinkedList* list, int from, int to) {
    if (list == NULL) {
        printf("[WARN]: Attempted delete for list on address: %p but it's freed\n", list);
        return NULL;
    }
    int max_index = list->length - 1;

    if (from < 0 || to < 0 || from > to || from > max_index || to > max_index) {
        printf("[ERROR]: Wrong index access for list on address: %p (from: %d, to: %d)\n", list, from, to);
        return NULL;
    }

    Node* current = list->head;
    Node* prev = NULL;

    for (int i = 0; i <= to; i++) {
        if (i < from) {
            prev = current;
            current = current->next;
            continue;
        }

        Node* next = current->next;

        if (prev != NULL) {
            prev->next = next;
        } else {
            list->head = next;
        }

        free(current->value_addr);
        free(current);

        current = next;
    }

    list->length -= (to - from) + 1;

    return current;
}

int delete_where(LinkedList* list, bool predicate(void*, int)) {
    Node* prev = NULL;
    Node* current = list->head;
    int delete_count = 0;

    for (int i = 0; i < list->length; i++) {
        bool should_delete = predicate(current->value_addr, i);

        if (!should_delete) {
            prev = current;
            current = current->next;
            continue;
        }

        Node* next = current->next;

        if (prev != NULL) {
            prev->next = next;
        } else {
            list->head = next;
        }

        free(current->value_addr);
        free(current);
        current = next;

        delete_count += 1;
    }

    list->length -= delete_count;

    return delete_count;
}
void for_each_node(LinkedList* list, void callback(void*, int, LinkedList*)) {
    if (list == NULL) {
        printf("[WARN]: Attempted to print list on address: %p but it's freed\n", list);
        return;
    }

    Node* current = list->head;

    if (current == NULL) {
        return;
    }

    for (int i = 0; i < list->length; i++) {
        callback(current->value_addr, i, list);
        current = current->next;
    }
}

void sortll(LinkedList* list, int get_order_weight(void*, void*)) {
    if (list->length <= 1) {
        return;
    }

    for (int i = 0; i < list->length; i++) {
        bool was_swapped = false;
        Node* prev = NULL;
        Node* current = list->head;
        Node* next = current->next;

        for (int j = 0; j < list->length - i - 1; j++) {
            int current_weight = get_order_weight(current->value_addr, next->value_addr);
            int next_weight = get_order_weight(next->value_addr, current->value_addr);

            if (current_weight > next_weight) {
                if (prev == NULL) {
                    list->head = next;
                } else {
                    prev->next = next;
                }
                current->next = next->next;
                next->next = current;
                current = next;

                was_swapped = true;
            }

            prev = current;
            current = prev->next;
            next = current->next;
        }

        if (!was_swapped) {
            break;
        }
    }
}

LinkedList* copyll(LinkedList* list, void* get_copy_value_addr(void*)) {
    LinkedList* new_list = linked_list(list->node_value_size);

    Node* current = list->head;

    while (current != NULL) {
        void* copy_addr = get_copy_value_addr(current->value_addr);

        append(new_list, copy_addr);

        current = current->next;
    }

    return new_list;
}

void empty_list(LinkedList* list) {
    if (list == NULL) {
        printf("[WARN]: Attempted to empty list on address: %p but it's freed\n", list);
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

void free_list(LinkedList** list) {
    if (list == NULL) {
        printf("[WARN]: Attempted to delete list on address: %p but it's freed\n", list);
        return;
    }

    empty_list(*list);
    free(*list);

    (*list) = NULL;
}

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* next;
} Node;
typedef struct {
    int length;
    struct Node* head;
} LinkedList;

Node* NewNode(int value) {
    Node* node = calloc(sizeof(Node), 1);
    node->value = value;
    return node;
}
LinkedList* NewLinkedList() {
    LinkedList* list = calloc(sizeof(LinkedList), 1);
    return list;
}

Node* getLastFrom(Node* node) {
    if (node == NULL) {
        return NULL;
    }

    while (node->next != NULL) {
        node = node->next;
    }
    return node;
}

/**
 * Adds the new value to the end of the list
 * and returns a pointer to that node.
 */
Node* append(LinkedList* list, int value) {
    if (list == NULL) {
        printf("[WARN] Attempted to append node for list on address: %p but the list it's freed\n", list);
        return NULL;
    }

    Node* last = getLastFrom(list->head);

    if (last == NULL) {
        list->head = NewNode(value);
        list->length += 1;
        return list->head;
    }

    last->next = NewNode(value);
    if (last->next == NULL) {
        return NULL;
    }

    list->length += 1;

    return last->next;
}

/** Removes the last node and returns the new length */
int removeLast(LinkedList* list) {
    if (list == NULL) {
        printf("[WARN] Attempted to remove last node for list on address: %p but it's freed\n", list);
        return -1;
    }

    if (list->head == NULL) {
        return 0;
    }
    if (list->head->next == NULL) {
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
    free(current);
    prev->next = NULL;

    list->length -= 1;

    return list->length;
}

void unshift(LinkedList* list, int value) {
    Node* head = NewNode(value);

    head->next = list->head;
    list->head = head;

    list->length += 1;
}
Node* insert(LinkedList* list, int index, int value) {
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
        return append(list, value);
    }
    if (list->length >= 0 && index == 0) {
        unshift(list, value);
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

    current->next = NewNode(value);
    current->next->next = next;
    list->length += 1;

    return current->next;
}

void emptyList(LinkedList* list) {
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
        free(current);
        current = next;

        if (next) {
            next = next->next;
        }
    }

    list->length = 0;
    list->head = NULL;
}

void freeList(LinkedList** list) {
    if (list == NULL) {
        printf("[WARN] Attempted to delete list on address: %p but it's freed\n", list);
        return;
    }

    emptyList(*list);
    free(*list);

    (*list) = NULL;
}

void printList(LinkedList* list) {
    if (list == NULL) {
        printf("[WARN] Attempted to print list on address: %p but it's freed\n", list);
        return;
    }

    Node* current = list->head;

    if (current == NULL) {
        printf("[]\n");
        return;
    }

    printf("[");

    while (current != NULL) {
        printf("%d", current->value);

        current = current->next;

        if (current != NULL) {
            printf(" -> ");
        }
    }

    printf("]");

    printf("\n");
}

int main(int argc, char** argv) {
    LinkedList* list = NewLinkedList();

    printf("[INFO] Append 5 on List:\n");
    for (int i = 1; i <= 5; i++) {
        append(list, i);
    }
    printList(list);

    printf("\n[INFO] Remove last 3 on List:\n");
    for (int i = 1; i <= 3; i++) {
        removeLast(list);
    }
    printList(list);

    printf("\n[INFO] Append and insertion on List:\n");
    append(list, 4);
    printList(list);
    insert(list, 2, 3);
    printList(list);

    printf("\n[INFO] Unshift on List:\n");
    unshift(list, 0);
    printList(list);

    printf("\n[INFO] Empty action on List:\n");
    emptyList(list);
    printList(list);

    printf("\n[INFO] Insertions on List:\n");
    for (int i = 0; i <= 3; i++) {
        insert(list, i, i * 2);
    }
    printList(list);
    for (int i = 0; i <= 3; i++) {
        insert(list, i * 2 + 1, i * 2 + 1);
    }
    printList(list);

    freeList(&list);
    printf("\n[INFO] Freed List\n");

    return 0;
}
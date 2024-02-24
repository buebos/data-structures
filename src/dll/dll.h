#ifndef __DLL_H__
#define __DLL_H__

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

DoublyLinkedList *dll();
DLLNode *dllnode(void *value_addr);

DLLNode *dll_append(DoublyLinkedList *list, void *value_addr);
DLLNode *dll_unshift(DoublyLinkedList *list, void *value_addr);
DLLNode *dll_insert(DoublyLinkedList *list, int index, void *value_addr);
DLLNode *dll_delwhere(DoublyLinkedList *list, bool predicate(void *, int));
DLLNode *dll_delrange(DoublyLinkedList *list, int from, int to);
DLLNode *dll_cutfrom(DoublyLinkedList *list_source, DoublyLinkedList *list_target, int target_index);

void dll_empty(DoublyLinkedList *list);
void dll_free(DoublyLinkedList **list);
void dll_print(DoublyLinkedList *list, void print_element(void *));

#endif /* __DLL_H__ */

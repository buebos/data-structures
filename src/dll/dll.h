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
DLLNode *append(DoublyLinkedList *list, void *value_addr);
DLLNode *unshift(DoublyLinkedList *list, void *value_addr);
DLLNode *insertdll(DoublyLinkedList *list, int index, void *value_addr);
DLLNode *delwheredll(DoublyLinkedList *list, bool predicate(void *, int));
DLLNode *delrangedll(DoublyLinkedList *list, int from, int to);
DLLNode *dllcutfrom(DoublyLinkedList *list_source, DoublyLinkedList *list_target, int target_index);
void printdll(DoublyLinkedList *list, void print_element(void *));

#endif /* __DLL_H__ */

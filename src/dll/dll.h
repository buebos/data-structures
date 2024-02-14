#ifndef __DLL__
#define __DLL__

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

DoublyLinkedList *dll();
DLLNode *dllnode(void *value_addr);

DLLNode *append(DoublyLinkedList *list, void *value_addr);

DLLNode *dll_delete_where(DoublyLinkedList *list, bool predicate(DLLNode *, int));

#endif /* __DLL */

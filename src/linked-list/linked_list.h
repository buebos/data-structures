#ifndef __LINKED_LIST__
#include <stdio.h>
#include <stdlib.h>

#define __LINKED_LIST__
typedef struct Node {
    void* value_addr;
    struct Node* next;
} Node;
typedef struct {
    int length;
    int node_value_size;
    struct Node* head;
} LinkedList;

Node* node(void* value_addr);
LinkedList* linked_list(int node_value_size);

/**
 * Adds a new node that contains the value address
 *
 * @returns A pointer to the new node
 */
Node* append(LinkedList* list, void* value_addr);

/**
 * Deallocates the last Node and the value_addr of it
 *
 * @returns The new length of the list
 */
int pop(LinkedList* list);

/**
 * Adds a new node in the desired index
 *
 * @returns A pointer to the node added
 */
Node* insert(LinkedList* list, int index, void* value_addr);

/**
 * Deallocates nodes from a starting index to an specified index, both
 * inclusive.
 */
Node* delete_index(LinkedList* list, int from, int to);

/**
 * Sets a new node with the value address to the
 * head of the list and the previous head to the next
 * pointer of the new node
 */
void unshift(LinkedList* list, void* value_addr);

/**
 * Deallocates the nodes of the linked list and frees
 * the associated value addresses of them without
 * destroying the linked list
 */
void empty_list(LinkedList* list);

/**
 * Empties the linked list and frees the linked list
 */
void free_ll(LinkedList** list);

#endif /** __LINKED_LIST__ */
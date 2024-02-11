#ifndef __LINKED_LIST__
#include <stdbool.h>
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
 * Deletes the nodes that match the predicate
 *
 * @returns The amount of nodes deleted
 */
int delete_where(LinkedList* list, bool predicate(void*, int));

void for_each_node(LinkedList* list, void callback(void*, int, LinkedList*));

/**
 * Sets a new node with the value address to the
 * head of the list and the previous head to the next
 * pointer of the new node
 */
void unshift(LinkedList* list, void* value_addr);

/**
 * @brief Bubble sort based on the get_order_weight callback, the
 * higher the value it returns for a node, the upper it will be
 * on the list.
 *
 * @param list The list to sort
 * @param get_order_weight Gets passed in two pointers to values on the list
 * and should return an int that indicates if the first element should go
 * earlier on the list by being a lower value.
 */
void sortll(LinkedList* list, int get_order_weight(void*, void*));

/**
 * @brief Copies a list, for each value the return of the get_copy_value_addr
 * should be a pointer to a new allocated value of the original value pointed by
 * the address passed in to the function
 *
 * @param list
 * @param get_copy_value_addr Gets a pointer to the original value to return
 * a pointer to a new allocated value that should be a copy of the original.
 * @return LinkedList*
 */
LinkedList* copyll(LinkedList* list, void* get_copy_value_addr(void*));

/**
 * Deallocates the nodes of the linked list and frees
 * the associated value addresses of them without
 * destroying the linked list
 */
void empty_list(LinkedList* list);

/**
 * Empties the linked list and frees the linked list
 */
void free_list(LinkedList** list);

#endif /** __LINKED_LIST__ */
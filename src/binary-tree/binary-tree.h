#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>
#include <stdlib.h>

#define SUCCESS_BT_OPERATION 0
#define ERROR_BT_OPERATION 1
#define ERROR_BT_NULL 2

typedef struct BTNode {
    void *data;
    struct BTNode *left;
    struct BTNode *right;
} BTNode;

typedef unsigned int (*GetDataWeightOperation)(void *data);
typedef void (*PrintDataOperation)(void *data);

typedef struct BT {
    BTNode *root;
    size_t _length;
    GetDataWeightOperation get_node_data_weight;
    PrintDataOperation print_data;
} BT;

BTNode *bt_new_node(void *data);
BT *bt_new(PrintDataOperation print_data,
           GetDataWeightOperation get_node_data_weight);
short int bt_insert_iterative(BT *bt, void *data);
int bt_get_data_depth(BT *bt, void *data);
size_t bt_get_node_count_recursive(BTNode *current);
size_t bt_get_depth_recursive(BTNode *current, size_t prev_depth);
void bt_print_recursive(BT *bt, BTNode *current, size_t current_depth);

#endif /* BINARY_TREE_H */

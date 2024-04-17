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

typedef int (*GetDataWeightComparation)(void *data_a, void *data_b);

typedef void (*PrintDataOperation)(void *data);

typedef struct BT {
    BTNode *root;
    size_t _length;
    /**
     * Since the struct kinda uses some polymorfism
     * it needs a way to know how to sort the nodes
     * in the tree. This will be used at insertion
     * operations to get an int that quantifies the
     * data address to be able to sort it in the tree.
     */
    GetDataWeightOperation get_node_data_weight;
    PrintDataOperation print_data;

    GetDataWeightComparation get_data_weight_comparison;
} BT;

BTNode *bt_new_node(void *data);
BT *bt_new(PrintDataOperation print_data,
           GetDataWeightOperation get_node_data_weight);
short int bt_insert_iterative(BT *bt, void *data);
int bt_get_data_depth(BT *bt, void *data);
size_t bt_get_node_count_recursive(BTNode *current);
size_t bt_get_depth_recursive(BTNode *current, size_t prev_depth);
void bt_print_recursive(BT *bt, BTNode *current, size_t current_depth);
short unsigned bt_equal(BT *bta, BT *btb);
void bt_print(BT *bt, short strategy);
void bt_free_recursive(BT *bt);
void *bt_get_data_by_weight(BT *bt, unsigned int data_weight);

void bt_foreach_recursive(BT *bt, void (*callback)(size_t i, void *data));

void *bt_get_index_recursive(BT *bt, size_t target);

void *bt_get_data_by_weight_comparison(BT *bt, void* data);

#endif /* BINARY_TREE_H */

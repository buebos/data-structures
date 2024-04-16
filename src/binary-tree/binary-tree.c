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

/**
 * Used on the struct of the Binary Tree to sort. Receives
 * a generic data address and translates it to an int.
 *
 * @param data The generic data address.
 * @return The int value of the data.
 */
typedef unsigned int (*GetDataWeightOperation)(void *data);

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
} BT;

BTNode *bt_new_node(void *data) {
    BTNode *btnode = calloc(1, sizeof(BTNode));
    btnode->data = data;
    return btnode;
}

BT *bt_new(PrintDataOperation print_data,
           GetDataWeightOperation get_node_data_weight) {
    BT *bt = calloc(1, sizeof(BT));
    bt->get_node_data_weight = get_node_data_weight;
    bt->print_data = print_data;
    return bt;
}

short int bt_insert_iterative(BT *bt, void *data) {
    BTNode *current_node = bt->root;
    unsigned int data_weight = bt->get_node_data_weight(data);

    while (current_node != NULL) {
        unsigned int current_weight = bt->get_node_data_weight(current_node->data);

        if (data_weight <= current_weight && !current_node->left) {
            current_node->left = bt_new_node(data);
            bt->_length += 1;
            return SUCCESS_BT_OPERATION;
        }
        if (data_weight > current_weight && !current_node->right) {
            current_node->right = bt_new_node(data);
            bt->_length += 1;
            return SUCCESS_BT_OPERATION;
        }

        if (data_weight <= current_weight) {
            current_node = current_node->left;
        } else {
            current_node = current_node->right;
        }
    }

    bt->root = bt_new_node(data);
    bt->_length += 1;

    return SUCCESS_BT_OPERATION;
}

int bt_get_data_depth(BT *bt, void *data) {
    BTNode *current_node = bt->root;
    unsigned int data_weight = bt->get_node_data_weight(data);
    int depth = 0;

    while (current_node != NULL) {
        unsigned int current_weight = bt->get_node_data_weight(current_node->data);

        if (current_weight == data_weight) {
            return depth;
        }

        if (current_weight < data_weight) {
            current_node = current_node->right;
        } else {
            current_node = current_node->left;
        }

        depth += 1;
    }

    return -1;
}

size_t bt_get_node_count_recursive(BTNode *current) {
    if (current == NULL) {
        return 0;
    }

    return 1 + bt_get_node_count_recursive(current->left) +
           bt_get_node_count_recursive(current->right);
}

size_t bt_get_depth_recursive(BTNode *current, size_t prev_depth) {
    if (current == NULL) {
        return prev_depth;
    }

    size_t left_depth = bt_get_depth_recursive(current->left, prev_depth + 1);
    size_t right_depth = bt_get_depth_recursive(current->right, prev_depth + 1);

    if (left_depth > right_depth) {
        return left_depth;
    }

    return right_depth;
}

void bt_print_recursive(BT *bt, BTNode *current, size_t current_depth) {
    if (current == NULL) {
        return;
    }

    if (current_depth == 0) {
        printf("| ROOT = ");
    }

    bt->print_data(current->data);

    if (current->left) {
        printf("\n");

        for (size_t i = 0; i < current_depth + 1; i++) {
            if (i != current_depth) {
                printf("\t");
                continue;
            }

            printf("\t| L = ");
        }

        bt_print_recursive(bt, current->left, current_depth + 1);
    }

    if (current->right) {
        printf("\n");

        for (size_t i = 0; i < current_depth + 1; i++) {
            if (i != current_depth) {
                printf("\t");
                continue;
            }

            printf("\t| R = ");
        }

        bt_print_recursive(bt, current->right, current_depth + 1);
    }
}

short bt_print(BT *bt, short strategy) {
    if (bt == NULL) {
        printf("[ERROR]: Binary tree is null");
        return ERROR_BT_NULL;
    }

    bt_print_recursive(bt, bt->root, 0);
}
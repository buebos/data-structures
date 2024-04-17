#include <stdbool.h>
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
        bool should_insert_left = data_weight <= current_weight;

        if (bt->get_data_weight_comparison) {
            should_insert_left = bt->get_data_weight_comparison(data, current_node->data) <= 0;
        }

        if (should_insert_left && !current_node->left) {
            current_node->left = bt_new_node(data);
            bt->_length += 1;
            return SUCCESS_BT_OPERATION;
        }
        if (!should_insert_left && !current_node->right) {
            current_node->right = bt_new_node(data);
            bt->_length += 1;
            return SUCCESS_BT_OPERATION;
        }

        if (should_insert_left) {
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
}

short bt_print(BT *bt, short strategy) {
    if (bt == NULL) {
        printf("[ERROR]: Binary tree is null");
        return ERROR_BT_NULL;
    }

    bt_print_recursive(bt, bt->root, 0);
}

short unsigned bt_equal_recursive(BT *bt, BTNode *node_a, BTNode *node_b) {
    if (!node_a && !node_b) {
        return 1;
    }

    if ((!node_a && node_b) || (node_a && !node_b)) {
        return 0;
    }

    if (bt->get_node_data_weight(node_a->data) !=
        bt->get_node_data_weight(node_b->data)) {
        return 0;
    }

    return bt_equal_recursive(bt, node_a->left, node_b->left) &&
           bt_equal_recursive(bt, node_a->right, node_b->right);
}

short unsigned bt_equal(BT *bta, BT *btb) {
    return bt_equal_recursive(bta, bta->root, btb->root);
}

void bt_free_recursive_inner(BTNode *node) {
    if (node == NULL) {
        return;
    }

    bt_free_recursive_inner(node->left);
    bt_free_recursive_inner(node->right);

    free(node->data);
    free(node);
}

void bt_free_recursive(BT *bt) {
    bt_free_recursive_inner(bt->root);

    free(bt);
}

void *bt_get_data_by_weight(BT *bt, unsigned int data_weight) {
    BTNode *current_node = bt->root;

    while (current_node != NULL) {
        unsigned int current_weight = bt->get_node_data_weight(current_node->data);

        if (current_weight == data_weight) {
            return current_node->data;
        }

        if (data_weight < current_weight) {
            current_node = current_node->left;
        } else {
            current_node = current_node->right;
        }
    }

    return NULL;
}

void *bt_get_data_by_weight_comparison(BT *bt, void *data) {
    BTNode *current_node = bt->root;

    while (current_node != NULL) {
        int comparison = bt->get_data_weight_comparison(data, current_node->data);

        if (comparison == 0) {
            return current_node->data;
        }

        if (comparison < 0) {
            current_node = current_node->left;
        } else {
            current_node = current_node->right;
        }
    }

    return NULL;
}

void bt_foreach_recursive_inner(BT *bt, BTNode *current, size_t *count,
                                void (*callback)(size_t i, void *data)) {
    if (current == NULL) {
        return;
    }

    bt_foreach_recursive_inner(bt, current->left, count, callback);

    callback(*count, current->data);
    *count += 1;

    bt_foreach_recursive_inner(bt, current->right, count, callback);
}
void bt_foreach_recursive(BT *bt, void (*callback)(size_t i, void *data)) {
    size_t count = 0;

    return bt_foreach_recursive_inner(bt, bt->root, &count, callback);
}

void *bt_get_index_recursive_inner(BTNode *current, size_t *count,
                                   size_t target) {
    if (current == NULL) {
        return NULL;
    }
    void *result = NULL;

    result = bt_get_index_recursive_inner(current->left, count, target);

    if (result) {
        return result;
    }
    if (*count == target) {
        return current->data;
    }

    *count += 1;

    return bt_get_index_recursive_inner(current->right, count, target);
}

void *bt_get_index_recursive(BT *bt, size_t target) {
    size_t count = 0;

    return bt_get_index_recursive_inner(bt->root, &count, target);
}

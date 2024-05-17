#include <stdio.h>

#include "../../util/clear.h"
#include "../../util/int.h"
#include "../../util/log/colors.h"
#include "../../util/log/log.h"
#include "../heap.h"

#define FILENAME_LEN 32

typedef struct App {
    bool should_run;
    unsigned char current_menu;
} App;

/**
 * File and FileNode logic for the heap concrete
 * implementations.
 */
typedef struct File {
    char name[FILENAME_LEN];
    unsigned int page_size;
} File;

typedef struct FileNode {
    File file;
    struct FileNode *right;
    struct FileNode *left;
} FileNode;

FileNode *filenode_new() {
    FileNode *node = calloc(1, sizeof(FileNode));
    return node;
}

bool filenode_is_name_equal(void *data, size_t _index, void *ctx) {
    FileNode *node = (FileNode *)data;
    char *name = (char *)ctx;
    return strcmp(node->file.name, name) == 0;
}
short filenode_compare(void *a, void *b) {
    return ((FileNode *)a)->file.page_size - ((FileNode *)b)->file.page_size;
}
void filenode_print(void *data) {
    File *file = &((FileNode *)data)->file;
    printf("(File){ name = %s, page_size = %d }", file->name, file->page_size);
}
void filenode_update(Heap *heap, size_t index_new, void *data) {
    FileNode *node = (FileNode *)data;

    if (index_new > 0) {
        size_t parent_index = (index_new - 1) / 2;
        FileNode *parent = (FileNode *)heap->_elements[parent_index];

        if (index_new % 2 == 0) {
            parent->right = node;
        } else {
            parent->left = node;
        }
    }

    if (node == NULL) {
        return;
    }

    size_t left_index = 2 * index_new + 1;
    if (left_index >= heap->_size) {
        node->left = NULL;
        node->right = NULL;
        return;
    }
    node->left = heap->_elements[left_index];

    size_t right_index = left_index + 1;
    if (right_index >= heap->_size) {
        node->right = NULL;
        return;
    }
    node->right = heap->_elements[right_index];
}

void filenode_print_tree_node(FileNode *node, size_t level) {
    FileNode *left = node->left;
    FileNode *right = node->right;

    if (level == 0) {
        printf("ROOT = ");
    }

    filenode_print(node);

    level += 1;

    if (right) {
        printf("\n");
        for (size_t i = 0; i < level; i++) {
            printf("\t");
        }
        printf("R = ");

        filenode_print_tree_node(right, level);
    }

    if (left) {
        printf("\n");
        for (size_t i = 0; i < level; i++) {
            printf("\t");
        }
        printf("L = ");

        filenode_print_tree_node(left, level);
    }
}
void filenode_print_tree(void *data) {
    FileNode *node = (FileNode *)data;

    filenode_print_tree_node(node, 0);
}

/**
 * Components for usual IO operations on the main
 * function.
 */
void TemplateReset() {
    clear();
    printf(AC_YELLOW "--- HEAPTER (HEAP-PRINTER) ---" AC_RESET "\n\n");
}
void KeyToContinue() {
    char holder = 0;

    printf("[INPUT]: Type any key to continue: ");
    scanf(" %c", &holder);
}

int main(int argc, char **argv) {
    App app = {
        .current_menu = 0,
        .should_run = true,
    };
    Heap heap = heap_new(
        HEAP_TYPE_MAX,
        HEAP_CAPACITY_DYNAMIC,
        HEAP_DELETE_FREE_REFERENCE,
        HEAP_STORE_REFERENCE,
        10,
        (HeapData){
            ._unit_size = sizeof(FileNode),
            .print = filenode_print,
            .compare = filenode_compare,
            .index_update = filenode_update,
        }

    );

    while (app.should_run) {
        TemplateReset();

        printlog(LOG_INFO, "Heap mode: %s", heap._type == HEAP_TYPE_MAX ? "DESC" : "ASC");
        printlog(LOG_INFO, "Heap files count: %d\n", heap._size);

        printf("[1]: Invert heap priority\n");
        printf("[2]: Display heap\n");
        printf("[3]: Add file\n");
        printf("[4]: Delete file\n");
        printf("[5]: Delete all files\n");
        printf("[6]: Process/Print file\n");
        printf("[7]: Close program\n\n");

        printf("[INPUT]: Select an option: ");
        scanf("%d", &app.current_menu);

        switch (app.current_menu) {
            case 1:
                heap_toggle_type(&heap);
                break;
            case 2: {
                TemplateReset();

                /** Display as tree and array */
                printlog(LOG_INFO, "Array format:");
                printf("\n");
                heap_print(&heap);

                printf("\n\n");

                printlog(LOG_INFO, "Tree format:");
                printf("\n");
                heap_print_tree(&heap);

                printf("\n\n");

                printlog(LOG_INFO, "Tree format using file node struct:");
                printf("\n");
                if (heap._size) {
                    filenode_print_tree(heap._elements[0]);
                } else {
                    printf("ROOT = (NULL)\n");
                }
                printf("\n");

                printf("\n");
                KeyToContinue();

                break;
            }
            case 3:
                FileNode *node = filenode_new();
                File *file = &node->file;

                TemplateReset();

                printf("[INPUT]: Type the file's name (%d maximum chars): ", FILENAME_LEN);
                scanf("%s", &file->name);
                printf("[INPUT]: Type the file's page size: ");
                scanf("%zu", &file->page_size);

                while (file->page_size < 1) {
                    printlog(LOG_ERROR, "Please insert a valid page size\n");
                    printf("[INPUT]: Type the file's page size: ");
                    scanf("%zu", &file->page_size);
                }

                heap_insert(&heap, node);

                break;
            case 4:
                TemplateReset();

                char file_name[FILENAME_LEN] = {1};
                file_name[FILENAME_LEN - 1] = 0;

                heap_print_tree(&heap);

                printf("\n");

                printf("[INPUT]: Type the file's name to delete (%d maximum chars): ", FILENAME_LEN);
                scanf("%s", file_name);

                heap_delete_match(&heap, filenode_is_name_equal, file_name);

                break;
            case 5:
                heap_delete_all(&heap);

                break;
            case 6: {
                TemplateReset();

                if (heap._size == 0) {
                    printlog(LOG_ERROR, "There are no files to print");
                    printf("\n");
                    KeyToContinue();
                    break;
                }

                File *file = &((FileNode *)heap._elements[0])->file;

                printlog(LOG_INFO, "Printing file: %s (%d pages)...", file->name, file->page_size);

                heap_delete_index(&heap, 0);

                printlog(LOG_INFO, "Done printing. %d files remaining on heap.", heap._size);

                printf("\n");
                KeyToContinue();

                break;
            }
            case 7:
                app.should_run = false;
                break;
            default:
                printlog(LOG_ERROR, "Invalid selection");
                KeyToContinue();
                break;
        }
    }

    TemplateReset();

    heap_delete_all(&heap);

    printlog(LOG_INFO, "See you in space cowboy!");

    return 0;
}
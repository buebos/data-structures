#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../util/clear.h"
#include "../../util/int.h"
#include "../queue-list.h"

void header() {
    printf("~~~~~ QUESHOP ~~~~~\n\n");
}

void chmenu(char* title, Queue* queue) {
    clear();
    header();

    if (title == NULL || title[0] == '\0') {
        return;
    }

    printf("%s\n\n", title);

    printf("[QUEUE]: ");
    queue_print(queue, integer_print);
    printf("\n");
}

int main() {
    size_t queue_capacity = 0;
    bool should_continue = true;

    chmenu("[MENU]: Pre-config menu", NULL);

    printf("[INFO]: Type the desired capacity for the queue: ");
    scanf("%zu", &queue_capacity);

    Queue* queue = queue_new(sizeof(int), queue_capacity, true);

    while (should_continue) {
        char res_char = '\0';
        unsigned int option = 0;

        chmenu("[MENU]: Main menu", queue);

        printf("[INFO]: Select an operation for the queue\n\n");

        printf("[0]: Enqueue an element\n");
        printf("[1]: Dequeue an element\n");
        printf("[2]: Insert an element on a particular index\n");
        printf("[3]: Exit\n\n");

        printf("[INPUT]: Your choice: ");
        scanf("%u", &option);

        switch (option) {
            case 0:
                chmenu("[MENU]: Enqueue", queue);

                int value = 0;

                if (queue->length == queue_capacity) {
                    printf("[ERROR]: Cannot queue_enqueue. Queue is at full capacity: %zu\n", queue->capacity);
                    break;
                }

                printf("[INFO]: Enter the element to queue_enqueue: ");
                scanf("%d", &value);
                queue_enqueue(queue, integer(value));

                chmenu("[MENU]: Enqueue", queue);

                break;
            case 1:
                if (queue->length > 0) {
                    queue_dequeue(queue);
                    chmenu("[MENU]: Dequeue", queue);
                } else {
                    chmenu("[MENU]: Dequeue", queue);
                    printf("[ERROR]: Queue is empty, cannot queue_dequeue.\n");
                }

                break;
            case 2: {
                size_t target_index = 0;
                int value = 0;

                chmenu("[MENU]: Insertion", queue);

                if (queue->length == queue_capacity) {
                    printf("[ERROR]: Cannot insert. Queue is at full capacity: %zu\n", queue->capacity);
                    break;
                }

                printf("[INFO]: Enter the index to insert the element: ");
                scanf("%zu", &target_index);

                if (target_index > queue->length) {
                    chmenu("[MENU]: Insertion", queue);
                    printf("[ERROR]: Index out of bounds.\n");
                    break;
                }

                printf("[INFO]: Enter the element to insert: ");
                scanf("%d", &value);

                queue_insert(queue, target_index, integer(value));

                chmenu("[MENU]: Insertion", queue);

                break;
            }
            case 3:
                should_continue = false;
                break;
            default:
                chmenu("[MENU]: Main menu", queue);
                printf("[INFO]: Invalid option, please try again.\n");
        }

        if (should_continue) {
            printf("[INPUT]: Type 'y' to continue: ");
            scanf(" %c", &res_char);

            should_continue = (res_char == 'y' || res_char == 'Y');
        }
    }

    chmenu("[MENU]: Exit", queue);

    printf("[BYE]: See you space cowboy!\n");

    queue_free(&queue);

    return 0;
}

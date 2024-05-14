#include "../../util/clear.h"
#include "../../util/log/log.h"
#include "../queue-list.c"

#define FILE_NAME_LEN 128

typedef enum AppQueueOrder {
    ASC,
    DESC
} AppQueueOrder;

typedef struct App {
    AppQueueOrder queue_order;
    int current_menu;
    bool should_run;
} App;
typedef struct File {
    char name[FILE_NAME_LEN];
    size_t page_size;
} File;

File *file_new() {
    File *file = calloc(1, sizeof(File));
    return file;
}

bool file_queue_match_page_size(void *queue_data, void *page_size) {
    Queue *queue = (Queue *)queue_data;

    if (!queue->front || ((File *)queue->front->data)->page_size != (*((size_t *)page_size))) {
        return false;
    }

    return true;
}

void file_print(void *file_data) {
    File *file = (File *)file_data;
    printf("(File){ name: %s, page_size: %zu } ", file->name, file->page_size);
}
void file_queues_print(void *queue_data, size_t _index, void *_ctx) {
    Queue *queue = (Queue *)queue_data;

    printlog(LOG_INFO, "Queue page size: %zu", ((File *)queue->front->data)->page_size);
    queue_print(queue, file_print);

    printf("\n");
}

bool file_match_name(void *file_data, void *file_target_ctx) {
    return strcmp(((File *)file_data)->name, (char *)file_target_ctx) == 0;
}
void file_queue_delete_matching_name(void *file_queue, size_t _index, void *file_target_ctx) {
    queue_filter((Queue *)file_queue, file_match_name, file_target_ctx);
}

void file_queue_empty(void *queue_data, size_t _index, void *_ctx) {
    Queue *queue = (Queue *)queue_data;
    queue->should_free_node_data = true;
    queue_empty((Queue *)queue_data);
}

bool file_queue_is_empty(void *queue, void *_ctx) {
    return !((Queue *)queue)->front;
}

short file_queue_priority_compare_asc(void *queue_a, void *queue_b) {
    File *file_a = ((File *)((Queue *)queue_a)->front->data);
    File *file_b = ((File *)((Queue *)queue_b)->front->data);

    if (file_a->page_size > file_b->page_size) {
        return -1;
    }

    return 1;
}
short file_queue_priority_compare_desc(void *queue_a, void *queue_b) {
    File *file_a = ((File *)((Queue *)queue_a)->front->data);
    File *file_b = ((File *)((Queue *)queue_b)->front->data);

    if (file_a->page_size > file_b->page_size) {
        return 1;
    }

    return -1;
}

void TemplateReset() {
    clear();
    printf("--- PRIORITY QUEUE---\n\n");
}

void KeyToContinue() {
    char holder = 0;

    printf("[INPUT]: Type any key to continue: ");
    scanf(" %c", &holder);
}

int main(int argc, char **argv) {
    App app = {.current_menu = 0, .should_run = true};
    Queue *file_queues = queue_new(sizeof(Queue), -1, true);

    while (app.should_run) {
        TemplateReset();

        printf("[1]: Invert queue priority\n");
        printf("[2]: Display queue\n");
        printf("[3]: Add file\n");
        printf("[4]: Delete file\n");
        printf("[5]: Delete all files\n");
        printf("[6]: Process/Print file\n");
        printf("[7]: Close program\n\n");

        printf("[INPUT]: Select an option: ");
        scanf("%d", &app.current_menu);

        switch (app.current_menu) {
            case 1:
                queue_reverse(file_queues);
                app.queue_order = !app.queue_order;
                break;
            case 2: {
                TemplateReset();

                if (!file_queues->front) {
                    printlog(LOG_INFO, "(Queue) { <EMPTY> } ");
                } else {
                    queue_foreach(file_queues, file_queues_print, NULL);
                }

                printf("\n");
                KeyToContinue();

                break;
            }
            case 3:
                File *file = file_new();

                TemplateReset();

                printf("[INPUT]: Type the file's name (%d maximum chars): ", FILE_NAME_LEN);
                scanf("%s", &file->name);
                printf("[INPUT]: Type the file's page size: ");
                scanf("%zu", &file->page_size);

                while (file->page_size < 1) {
                    printf("[ERROR]: Please insert a valid page size\n");
                    printf("[INPUT]: Type the file's page size: ");
                    scanf("%zu", &file->page_size);
                }

                Queue *queue_page_match = queue_find(file_queues, file_queue_match_page_size, &file->page_size);

                if (queue_page_match) {
                    queue_enqueue(queue_page_match, file);
                    break;
                }

                Queue *queue_page_new = queue_new(sizeof(File), -1, true);
                queue_enqueue(queue_page_new, file);

                if (app.queue_order == ASC) {
                    queue_insert_priority(file_queues, queue_page_new, file_queue_priority_compare_asc);
                } else {
                    queue_insert_priority(file_queues, queue_page_new, file_queue_priority_compare_desc);
                }

                break;
            case 4:
                TemplateReset();

                char file_target[FILE_NAME_LEN] = {1};
                file_target[FILE_NAME_LEN - 1] = 0;

                if (!file_queues->front) {
                    printlog(LOG_INFO, "(Queue) { <EMPTY> } ");
                } else {
                    queue_foreach(file_queues, file_queues_print, NULL);
                }

                printf("\n");

                printf("[INPUT]: Type the file's name to delete (%d maximum chars): ", FILE_NAME_LEN);
                scanf("%s", file_target);

                queue_foreach(file_queues, file_queue_delete_matching_name, file_target);
                queue_filter(file_queues, file_queue_is_empty, NULL);

                break;
            case 5:
                queue_foreach(file_queues, file_queue_empty, NULL);
                queue_empty(file_queues);

                break;
            case 6: {
                TemplateReset();

                if (!file_queues->front) {
                    printlog(LOG_ERROR, "No file queues in line");
                    printf("\n");
                    KeyToContinue();
                    break;
                }

                Queue *file_queue = file_queues->front->data;

                if (!file_queue->front) {
                    printlog(LOG_ERROR, "No files in line");
                    printf("\n");
                    KeyToContinue();
                    break;
                }

                File *file = (File *)file_queue->front->data;

                printlog(LOG_INFO, "Proccesing: (File){ name: %s, page_size: %zu }", file->name, file->page_size);
                queue_dequeue(file_queue);

                if (!file_queue->front) {
                    queue_dequeue(file_queues);
                }

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

    queue_foreach(file_queues, file_queue_empty, NULL);
    queue_free(&file_queues);

    printf("\n");
    printlog(LOG_INFO, "See you in space cowboy!");

    return 0;
}
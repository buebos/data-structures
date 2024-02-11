#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../linked_list.h"

/** To call the correct system clear console */
#ifdef __unix__
#elif defined(_WIN32) || defined(WIN32)
#define OS_Windows
#endif

/** Clears the terminal */
void clear() {
#ifdef OS_Windows
    system("cls");
#else
    system("clear");
#endif
}

typedef struct {
    int x;
    int y;
    int z;
} Position;
typedef struct {
    int id;
    int health;
    int type;
    Position position;
} Enemy;

Enemy* enemy(int id, int health, int type, Position position) {
    Enemy* enemy = calloc(1, sizeof(Enemy));
    enemy->id = id;
    enemy->health = health;
    enemy->position = position;
    enemy->type = type;

    return enemy;
}

char* get_enemy_tag(int type) {
    switch (type) {
        case 1:
            return "Zombie";
        case 2:
            return "Skeleton";
        case 3:
            return "Creeper";
        default:
            return "Enemy";
    }
}

bool is_zombie(void* enemy, int index) {
    return ((Enemy*)enemy)->type == 1;
}

void print_enemy(void* addr, int index, LinkedList* list) {
    Enemy* enemy = (Enemy*)addr;
    printf("\t{ id: %d,\t tag: %s,\t health: %d, coords: (%d, %d, %d) }", enemy->id, get_enemy_tag(enemy->type), enemy->health, enemy->position.x, enemy->position.y, enemy->position.z);

    if (index < list->length - 1) {
        printf(" -> \n");
    }
}
void printll_enemy(LinkedList* list) {
    printf("[");
    if (list->length > 0) {
        printf("\n");
    }
    for_each_node(list, print_enemy);
    if (list->length > 0) {
        printf("\n");
    }
    printf("]\n");
}

void* copy_enemy(void* addr) {
    Enemy* original = (Enemy*)addr;
    Enemy* copy = enemy(original->id, original->health, original->type, original->position);
    return (void*)copy;
}
int aschealth(void* a, void* b) {
    Enemy* ea = (Enemy*)a;
    Enemy* eb = (Enemy*)b;
    return ea->health - eb->health;
}
int deschealth(void* a, void* b) {
    Enemy* ea = (Enemy*)a;
    Enemy* eb = (Enemy*)b;
    return eb->health - ea->health;
}
int asctag(void* a, void* b) {
    Enemy* ea = (Enemy*)a;
    Enemy* eb = (Enemy*)b;
    return get_enemy_tag(ea->type)[0] - get_enemy_tag(eb->type)[0];
}
int desctag(void* a, void* b) {
    Enemy* ea = (Enemy*)a;
    Enemy* eb = (Enemy*)b;
    return get_enemy_tag(eb->type)[0] - get_enemy_tag(ea->type)[0];
}

int main(int argc, char** argv) {
    LinkedList* list = linked_list(sizeof(Enemy));
    int should_continue = true;

    for (int i = 1; i <= 20; i++) {
        /**
         * Calls based on i to get a kind of scattered sample. Could use
         * rand function but I think it would be an overkill for this
         * purpose lol
         */
        append(list, enemy(i, (100 - i) % i + 5, i % 3 + 1, (Position){(100 - i) % i + 5, i, i}));
    }

    while (should_continue) {
        LinkedList* list_to_order = copyll(list, copy_enemy);
        int sort_strategy = 0;
        char continue_ans;

        clear();
        printf("[INFO]: Original list:\n");
        printll_enemy(list);
        printf("\n");
        printf("[INFO]: Select a sort strategy:\n[1]: Asc by health\n[2]: Desc by health\n[3]: Asc by tag (Creeper, Skeleton, Zombie)\n[4]: Desc by tag (Zombie, Skeleton, Creeper)\n\n[INPUT]: ");

        scanf("%d", &sort_strategy);
        clear();

        printf("[INFO]: Sorted");

        switch (sort_strategy) {
            case 1:
                printf(" asc by health:\n");
                sortll(list_to_order, aschealth);
                break;
            case 2:
                printf(" desc by health:\n");
                sortll(list_to_order, deschealth);
                break;
            case 3:
                printf(" asc by tag:\n");
                sortll(list_to_order, asctag);
                break;
            default:
                printf(" desc by tag:\n");
                sortll(list_to_order, desctag);
                break;
        }

        printll_enemy(list_to_order);
        printf("\n");

        free_list(&list_to_order);
        printf("[INFO]: Freed sorted enemy list\n");

        printf("[INPUT]: Do you want to continue? (y / n): ");
        scanf(" %c", &continue_ans);
        should_continue = continue_ans == "y"[0];
    }

    clear();
    free_list(&list);
    printf("[INFO]: Freed original enemy list\n");
    printf("[INFO]: Bye!\n");

    return 0;
}

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../linked_list.h"

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
    printf("{ id: %d, tag: %s, health: %d, coords: (%d, %d, %d) }", enemy->id, get_enemy_tag(enemy->type), enemy->health, enemy->position.x, enemy->position.y, enemy->position.z);

    if (index < list->length - 1) {
        printf(" -> ");
    }
}
void printll_enemy(LinkedList* list) {
    printf("[");
    for_each_node(list, print_enemy);
    printf("]\n");
}

int main(int argc, char** argv) {
    LinkedList* list = linked_list(sizeof(Enemy));

    printf("\n[INFO] Append 4 enemies\n");
    append(list, enemy(list->length, 10, 1, (Position){0, 0, 0}));
    append(list, enemy(list->length, 20, 2, (Position){1, 3, 0}));
    append(list, enemy(list->length, 10, 1, (Position){3, 3, 0}));
    append(list, enemy(list->length, 30, 3, (Position){3, 3, 0}));
    printll_enemy(list);

    printf("\n[INFO] Deleting zombies\n");

    int zombies_deleted_count = delete_where(list, is_zombie);

    printf("\n[INFO] Deleted %d zombies\n", zombies_deleted_count);
    printll_enemy(list);

    printf("\n[INFO] Freed enemy list\n");
    free_list(&list);

    return 0;
}

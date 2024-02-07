#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

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

int* integer(int value) {
    int* value_address = calloc(1, sizeof(int));
    *value_address = value;
    return value_address;
}
void printll_int(LinkedList* list) {
    if (list == NULL) {
        printf("[WARN] Attempted to print list on address: %p but it's freed\n", list);
        return;
    }

    Node* current = list->head;

    if (current == NULL) {
        printf("[]\n");
        return;
    }

    printf("[");

    while (current != NULL) {
        printf("%d", (*(int*)(current->value_addr)));

        current = current->next;

        if (current != NULL) {
            printf(" -> ");
        }
    }

    printf("]");

    printf("\n");
}

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
void printll_enemy(LinkedList* list) {
    if (list == NULL) {
        printf("[WARN] Attempted to print list on address: %p but it's freed\n", list);
        return;
    }

    Node* current = list->head;

    if (current == NULL) {
        printf("[]\n");
        return;
    }

    printf("[");

    while (current != NULL) {
        Enemy* enemy = (Enemy*)(current->value_addr);
        printf("{ id: %d, tag: %s, health: %d, coords: (%d, %d, %d) }", enemy->id, get_enemy_tag(enemy->type), enemy->health, enemy->position.x, enemy->position.y, enemy->position.z);

        current = current->next;

        if (current != NULL) {
            printf(" -> ");
        }
    }

    printf("]");

    printf("\n");
}

bool is_zombie(void* enemy, int index) {
    return get_enemy_tag(((Enemy*)enemy)->type) == "Zombie";
}

int main(int argc, char** argv) {
    LinkedList* list = linked_list(sizeof(int));

    printf("[INFO] Append 5 on list:\n");
    for (int i = 1; i <= 5; i++) {
        append(list, integer(i));
    }
    printll_int(list);

    printf("\n[INFO] Remove last 3 on list:\n");
    for (int i = 1; i <= 3; i++) {
        pop(list);
    }
    printll_int(list);

    printf("\n[INFO] Append and insertion on list:\n");
    append(list, integer(4));
    printll_int(list);
    insert(list, 2, integer(3));
    printll_int(list);

    printf("\n[INFO] Unshift on list:\n");
    unshift(list, integer(0));
    printll_int(list);

    printf("\n[INFO] Delete from index 1 to 3 on list:\n");
    delete_index(list, 1, 3);
    printll_int(list);

    printf("\n[INFO] Empty action on list:\n");
    empty_list(list);
    printll_int(list);

    printf("\n[INFO] Insertions on list:\n");
    for (int i = 0; i <= 3; i++) {
        insert(list, i, integer(i * 2));
    }
    printll_int(list);
    for (int i = 0; i <= 3; i++) {
        insert(list, i * 2 + 1, integer(i * 2 + 1));
    }
    printll_int(list);

    free_list(&list);
    printf("\n[INFO] Freed integer list\n");

    printf("\n[INFO] Creating enemy list\n");
    list = linked_list(sizeof(Enemy));
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

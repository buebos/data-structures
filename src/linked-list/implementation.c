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

int* integer(int value_addr) {
    int* value_addr_address = calloc(1, sizeof(int));
    *value_addr_address = value_addr;
    return value_addr_address;
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
        printf("{ id: %d, tag: %s, health: %d }", enemy->id, get_enemy_tag(enemy->type), enemy->health);

        current = current->next;

        if (current != NULL) {
            printf(" -> ");
        }
    }

    printf("]");

    printf("\n");
}

int main(int argc, char** argv) {
    LinkedList* list = linked_list(sizeof(int));

    printf("[INFO] Append 5 on List:\n");
    for (int i = 1; i <= 5; i++) {
        append(list, integer(i));
    }
    printll_int(list);

    printf("\n[INFO] Remove last 3 on List:\n");
    for (int i = 1; i <= 3; i++) {
        remove_last(list);
    }
    printll_int(list);

    printf("\n[INFO] Append and insertion on List:\n");
    append(list, integer(4));
    printll_int(list);
    insert(list, 2, integer(3));
    printll_int(list);

    printf("\n[INFO] Unshift on List:\n");
    unshift(list, integer(0));
    printll_int(list);

    printf("\n[INFO] Empty action on List:\n");
    empty_list(list);
    printll_int(list);

    printf("\n[INFO] Insertions on List:\n");
    for (int i = 0; i <= 3; i++) {
        insert(list, i, integer(i * 2));
    }
    printll_int(list);
    for (int i = 0; i <= 3; i++) {
        insert(list, i * 2 + 1, integer(i * 2 + 1));
    }
    printll_int(list);

    free_ll(&list);
    printf("\n[INFO] Freed integer list\n");

    printf("\n[INFO] Creating enemy list\n");
    list = linked_list(sizeof(Enemy));
    printf("\n[INFO] Append 1 enemy\n");
    append(list, enemy(list->length, 10, 1, (Position){0, 0, 0}));
    printll_enemy(list);
    printf("\n[INFO] Freed enemy list\n");
    free_ll(&list);

    return 0;
}

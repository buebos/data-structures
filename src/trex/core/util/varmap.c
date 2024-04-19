#include "varmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TrexVarMapPair* trex_varmap_pair_new(char* key, float value) {
    TrexVarMapPair* pair = calloc(1, sizeof(TrexVarMapPair));
    pair->key = strdup(key);
    pair->value = value;
    return pair;
}

void trex_varmap_set(TrexVarMap* map, char* key, float value) {
    TrexVarMapPair* current = map->_head;
    TrexVarMapPair* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return;
        }

        prev = current;
        current = current->next;
    }

    TrexVarMapPair* new_current = trex_varmap_pair_new(key, value);

    if (prev == NULL) {
        map->_head = new_current;
    } else {
        prev->next = new_current;
    }

    map->_length += 1;
}

float trex_varmap_get(TrexVarMap* map, char* key) {
    TrexVarMapPair* current = map->_head;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }

    return 0.0f;
}

void trex_varmap_del(TrexVarMap* map, char* key) {
    TrexVarMapPair* current = map->_head;
    TrexVarMapPair* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                map->_head = current->next;
            } else {
                prev->next = current->next;
            }

            free(current->key);
            free(current);

            map->_length -= 1;

            return;
        }

        prev = current;
        current = current->next;
    }
}

void trex_varmap_free(TrexVarMap* map) {
    TrexVarMapPair* current = map->_head;
    TrexVarMapPair* next;

    while (current != NULL) {
        next = current->next;

        free(current->key);
        free(current);

        current = next;
    }

    map->_head = NULL;
    map->_length = 0;
}

int trex_varmap_has_key(TrexVarMap* map, char* key) {
    TrexVarMapPair* current = map->_head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return 1;  // Key found
        }
        current = current->next;
    }
    return 0;  // Key not found
}
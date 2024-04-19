#ifndef __TREX_VAR_MAP_H__

#define __TREX_VAR_MAP_H__

#include <stdlib.h>

typedef struct TrexVarMapPair {
    char *key;
    float value;
    struct TrexVarMapPair *next;
} TrexVarMapPair;

typedef struct TrexVarMap {
    TrexVarMapPair *_head;
    size_t _length;
} TrexVarMap;

void trex_varmap_set(TrexVarMap *map, char *key, float value);
float trex_varmap_get(TrexVarMap *map, char *key);

void trex_varmap_del(TrexVarMap *map, char *key);

void trex_varmap_free(TrexVarMap *map);

int trex_varmap_has_key(TrexVarMap *map, char *key);

#endif  // !__TREX_VAR_MAP_H__
#ifndef TABLE_H
#define TABLE_H

#include "bb.h"
#include "move.h"

typedef struct {
    bb key;
    int depth;
    int value;
    Move move;
} Entry;

typedef struct {
    int size;
    int mask;
    Entry *data;
} Table;

void table_alloc(Table *table, int bits);
void table_free(Table *table);
Entry *table_get(Table *table, bb key);
void table_set(Table *table, bb key, int depth, int value, Move *move);

#endif

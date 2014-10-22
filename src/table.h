#ifndef TABLE_H
#define TABLE_H

#include "bb.h"
#include "move.h"

#define TABLE_ENTRY(table, key) ((table)->data + ((key) & (table)->mask))

typedef struct {
    bb key;
    unsigned char depth;
    Move move;
} Entry;

typedef struct {
    int size;
    int mask;
    Entry *data;
} Table;

void table_alloc(Table *table, int bits);
void table_free(Table *table);
Entry *table_entry(Table *table, bb key);
Move *table_get_move(Table *table, bb key);
void table_set_move(Table *table, bb key, int depth, Move *move);

#endif

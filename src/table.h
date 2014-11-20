#ifndef TABLE_H
#define TABLE_H

#include "bb.h"
#include "move.h"

#define TABLE_ENTRY(table, key) ((table)->data + ((key) & (table)->mask))

#define TABLE_NONE 0
#define TABLE_EXACT 1
#define TABLE_ALPHA 2
#define TABLE_BETA 3

typedef struct {
    bb key;
    int value;
    unsigned char flag;
    unsigned char depth;
    Move move;
} Entry;

typedef struct {
    int size;
    int mask;
    Entry *data;
} Table;

typedef struct {
    bb key;
    int value;
} PawnEntry;

typedef struct {
    int size;
    int mask;
    PawnEntry *data;
} PawnTable;

void table_alloc(Table *table, int bits);
void table_free(Table *table);
Move *table_get_move(Table *table, bb key);
void table_set_move(Table *table, bb key, int depth, Move *move);
void table_set(Table *table, bb key, int depth, int value, int flag);
int table_get(Table *table, bb key, int depth, int alpha, int beta, int *value);

void pawn_table_alloc(PawnTable *table, int bits);
void pawn_table_free(PawnTable *table);
int pawn_table_get(PawnTable *table, bb key, int *value);
void pawn_table_set(PawnTable *table, bb key, int value);

#endif

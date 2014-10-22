#include <stdlib.h>
#include <string.h>
#include "table.h"

void table_alloc(Table *table, int bits) {
    table->size = 1 << bits;
    table->mask = table->size - 1;
    table->data = calloc(table->size, sizeof(Entry));
}

void table_free(Table *table) {
    free(table->data);
}

Move *table_get_move(Table *table, bb key) {
    Entry *entry = TABLE_ENTRY(table, key);
    if (entry->key == key) {
        return &entry->move;
    }
    return NULL;
}

void table_set_move(Table *table, bb key, int depth, Move *move) {
    Entry *entry = TABLE_ENTRY(table, key);
    if (entry->depth <= depth) {
        entry->key = key;
        entry->depth = depth;
        memcpy(&entry->move, move, sizeof(Move));
    }
}

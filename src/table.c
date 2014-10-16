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

Entry *table_get(Table *table, bb key) {
    return table->data + (key & table->mask);
}

void table_set(Table *table, bb key, int depth, int value, Move *move) {
    Entry *entry = table_get(table, key);
    entry->key = key;
    entry->depth = depth;
    entry->value = value;
    if (move) {
        memcpy(&entry->move, move, sizeof(Move));
    }
}

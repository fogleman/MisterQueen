#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "move.h"

#define INF 1000000
#define MATE 100000

typedef struct {
    double duration;
} SearchParameters;

int search(Board *board, SearchParameters *parameters, Move *move);
void stop_search();

#endif

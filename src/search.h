#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "move.h"

#define INF 1000000
#define MATE 100000

int search(Board *board, double duration, Move *move);
void stop_search();

#endif

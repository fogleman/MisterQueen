#ifndef MOVE_H
#define MOVE_H

#include "board.h"

typedef struct {
    int src;
    int dst;
} Move;

void do_move(Board *board, Move *move);

#endif

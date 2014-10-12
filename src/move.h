#ifndef MOVE_H
#define MOVE_H

#include "board.h"

typedef struct {
    int src;
    int dst;
    int piece;
    int capture;
} Move;

void do_move(Board *board, Move *move);
void undo_move(Board *board, Move *move);

void move_notation(Board *board, Move *move, char *result);

#endif

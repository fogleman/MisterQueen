#ifndef MOVE_H
#define MOVE_H

#include "bb.h"
#include "board.h"

#define MAX_MOVES 256

typedef struct {
    int src;
    int dst;
    int promotion;
} Move;

typedef struct {
    int piece;
    int capture;
    int castle;
    int ep;
} Undo;

void do_move(Board *board, Move *move, Undo *undo);
void undo_move(Board *board, Move *move, Undo *undo);

void move_notation(Board *board, Move *move, char *result);

#endif

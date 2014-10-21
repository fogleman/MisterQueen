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
    bb ep;
} Undo;

void make_move(Board *board, Move *move);
void do_null_move(Board *board, Undo *undo);
void undo_null_move(Board *board, Undo *undo);
void do_move(Board *board, Move *move, Undo *undo);
void undo_move(Board *board, Move *move, Undo *undo);
int score_move(Board *board, Move *move);

void notate_move(Board *board, Move *move, Move *moves, int count, char *result);
void print_move(Board *board, Move *move);
int parse_move(Board *board, char *notation, Move *move);
void parse_pgn(Board *board, char *pgn);

#endif

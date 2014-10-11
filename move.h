#ifndef MOVE_H
#define MOVE_H

#include "board.h"

typedef struct {
    int src;
    int dst;
} Move;

int gen_white_pawn_moves(Board *board, Move *moves);
int gen_white_knight_moves(Board *board, Move *moves);
int gen_white_bishop_moves(Board *board, Move *moves);
int gen_white_rook_moves(Board *board, Move *moves);
int gen_white_queen_moves(Board *board, Move *moves);
int gen_white_king_moves(Board *board, Move *moves);
int gen_white_moves(Board *board, Move *moves);

#endif

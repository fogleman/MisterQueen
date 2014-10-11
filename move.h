#ifndef MOVE_H
#define MOVE_H

#include "board.h"

typedef struct {
    int src;
    int dst;
} Move;

int gen_knight_moves(Move *moves, bb srcs, bb mask);
int gen_bishop_moves(Move *moves, bb srcs, bb mask, bb all);
int gen_rook_moves(Move *moves, bb srcs, bb mask, bb all);
int gen_queen_moves(Move *moves, bb srcs, bb mask, bb all);
int gen_king_moves(Move *moves, bb srcs, bb mask);

int gen_white_pawn_moves(Board *board, Move *moves);
int gen_white_knight_moves(Board *board, Move *moves);
int gen_white_bishop_moves(Board *board, Move *moves);
int gen_white_rook_moves(Board *board, Move *moves);
int gen_white_queen_moves(Board *board, Move *moves);
int gen_white_king_moves(Board *board, Move *moves);
int gen_white_moves(Board *board, Move *moves);

int gen_black_pawn_moves(Board *board, Move *moves);
int gen_black_knight_moves(Board *board, Move *moves);
int gen_black_bishop_moves(Board *board, Move *moves);
int gen_black_rook_moves(Board *board, Move *moves);
int gen_black_queen_moves(Board *board, Move *moves);
int gen_black_king_moves(Board *board, Move *moves);
int gen_black_moves(Board *board, Move *moves);

#endif

#ifndef BOARD_H
#define BOARD_H

#include "bb.h"

#define WHITE 0x00
#define BLACK 0x10

#define EMPTY 0x00
#define PAWN 0x01
#define KNIGHT 0x02
#define BISHOP 0x03
#define ROOK 0x04
#define QUEEN 0x05
#define KING 0x06

#define PIECE(x) ((x) & 0x0f)
#define COLOR(x) ((x) & 0x10)

typedef struct {
    int color;
    int squares[64];
    bb all;
    bb white;
    bb black;
    bb white_pawns;
    bb black_pawns;
    bb white_knights;
    bb black_knights;
    bb white_bishops;
    bb black_bishops;
    bb white_rooks;
    bb black_rooks;
    bb white_queens;
    bb black_queens;
    bb white_kings;
    bb black_kings;
} Board;

void board_reset(Board *board);
void board_set(Board *board, int sq, int piece);
void board_print(Board *board);

#endif

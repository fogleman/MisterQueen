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

typedef struct {
    int squares[64];
    bb white;
    bb black;
} Board;

void board_reset(Board *board);
void board_set(Board *board, int sq, int piece);
void board_print(Board *board);

#endif

#include "move.h"

void gen_white_pawn_moves(Board *board) {
    bb all = board->white | board->black;
    bb pawns = board->white_pawns;
    bb p1 = (pawns << 8) & ~all;
    bb p2 = ((p1 & 0x0000000000ff0000L) << 8) & ~all;
    bb a1 = ((pawns & 0x7f7f7f7f7f7f7f7fL) << 7) & board->black;
    bb a2 = ((pawns & 0xfefefefefefefefeL) << 9) & board->black;
    bb_print(p1);
    bb_print(p2);
    bb_print(a1);
    bb_print(a2);
    while (p1) {
        int sq = LSB(p1);
        printf("%d, %d\n", sq - 8, sq);
        p1 &= ~SQ(sq);
    }
    while (p2) {
        int sq = LSB(p2);
        printf("%d, %d\n", sq - 16, sq);
        p2 &= ~SQ(sq);
    }
    while (a1) {
        int sq = LSB(a1);
        printf("%d, %d\n", sq - 7, sq);
        a1 &= ~SQ(sq);
    }
    while (a2) {
        int sq = LSB(a2);
        printf("%d, %d\n", sq - 9, sq);
        a2 &= ~SQ(sq);
    }
}

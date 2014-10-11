#include <stdio.h>
#include "board.h"

void board_reset(Board *board) {
    for (int sq = 0; sq < 64; sq++) {
        board_set(board, sq, EMPTY);
    }
    for (int file = 0; file < 8; file++) {
        board_set(board, RF(1, file), WHITE | PAWN);
        board_set(board, RF(6, file), BLACK | PAWN);
    }
    board_set(board, RF(0, 0), WHITE | ROOK);
    board_set(board, RF(0, 1), WHITE | KNIGHT);
    board_set(board, RF(0, 2), WHITE | BISHOP);
    board_set(board, RF(0, 3), WHITE | QUEEN);
    board_set(board, RF(0, 4), WHITE | KING);
    board_set(board, RF(0, 5), WHITE | BISHOP);
    board_set(board, RF(0, 6), WHITE | KNIGHT);
    board_set(board, RF(0, 7), WHITE | ROOK);
    board_set(board, RF(7, 0), BLACK | ROOK);
    board_set(board, RF(7, 1), BLACK | KNIGHT);
    board_set(board, RF(7, 2), BLACK | BISHOP);
    board_set(board, RF(7, 3), BLACK | QUEEN);
    board_set(board, RF(7, 4), BLACK | KING);
    board_set(board, RF(7, 5), BLACK | BISHOP);
    board_set(board, RF(7, 6), BLACK | KNIGHT);
    board_set(board, RF(7, 7), BLACK | ROOK);
}

void board_set(Board *board, int sq, int piece) {
    board->squares[sq] = piece;
    if (piece) {

    }
    else {
        
    }
}

void board_print(Board *board) {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            char c;
            int piece = board->squares[RF(rank, file)];
            switch (piece & 0xf) {
                case EMPTY:  c = '.'; break;
                case PAWN:   c = 'P'; break;
                case KNIGHT: c = 'N'; break;
                case BISHOP: c = 'B'; break;
                case ROOK:   c = 'R'; break;
                case QUEEN:  c = 'Q'; break;
                case KING:   c = 'K'; break;
            };
            if ((piece & 0xf0) == BLACK) {
                c |= (1 << 5);
            }
            putchar(c);
        }
        putchar('\n');
    }
    putchar('\n');
}

#include <stdio.h>
#include <string.h>
#include "board.h"

void board_reset(Board *board) {
    memset(board, 0, sizeof(Board));
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
        bb bit = SQ(sq);
        if (COLOR(piece)) {
            board->black |= bit;
            switch (PIECE(piece)) {
                case PAWN:   board->black_pawns |= bit; break;
                case KNIGHT: board->black_knights |= bit; break;
                case BISHOP: board->black_bishops |= bit; break;
                case ROOK:   board->black_rooks |= bit; break;
                case QUEEN:  board->black_queens |= bit; break;
                case KING:   board->black_kings |= bit; break;
            }
        }
        else {
            board->white |= bit;
            switch (PIECE(piece)) {
                case PAWN:   board->white_pawns |= bit; break;
                case KNIGHT: board->white_knights |= bit; break;
                case BISHOP: board->white_bishops |= bit; break;
                case ROOK:   board->white_rooks |= bit; break;
                case QUEEN:  board->white_queens |= bit; break;
                case KING:   board->white_kings |= bit; break;
            }
        }
    }
    else {
        bb mask = ~SQ(sq);
        board->white &= mask;
        board->black &= mask;
        board->white_pawns &= mask;
        board->black_pawns &= mask;
        board->white_knights &= mask;
        board->black_knights &= mask;
        board->white_bishops &= mask;
        board->black_bishops &= mask;
        board->white_rooks &= mask;
        board->black_rooks &= mask;
        board->white_queens &= mask;
        board->black_queens &= mask;
        board->white_kings &= mask;
        board->black_kings &= mask;
    }
}

void board_print(Board *board) {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            char c;
            int piece = board->squares[RF(rank, file)];
            switch (PIECE(piece)) {
                case EMPTY:  c = '.'; break;
                case PAWN:   c = 'P'; break;
                case KNIGHT: c = 'N'; break;
                case BISHOP: c = 'B'; break;
                case ROOK:   c = 'R'; break;
                case QUEEN:  c = 'Q'; break;
                case KING:   c = 'K'; break;
            };
            if (COLOR(piece)) {
                c |= 0x20;
            }
            putchar(c);
        }
        putchar('\n');
    }
    putchar('\n');
}

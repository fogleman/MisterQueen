#include <stdio.h>
#include <string.h>
#include "board.h"

void board_clear(Board *board) {
    memset(board, 0, sizeof(Board));
    board->castle = CASTLE_ALL;
    board->ep = -1;
}

void board_reset(Board *board) {
    board_clear(board);
    for (int file = 0; file < 8; file++) {
        board_set(board, RF(1, file), WHITE_PAWN);
        board_set(board, RF(6, file), BLACK_PAWN);
    }
    board_set(board, RF(0, 0), WHITE_ROOK);
    board_set(board, RF(0, 1), WHITE_KNIGHT);
    board_set(board, RF(0, 2), WHITE_BISHOP);
    board_set(board, RF(0, 3), WHITE_QUEEN);
    board_set(board, RF(0, 4), WHITE_KING);
    board_set(board, RF(0, 5), WHITE_BISHOP);
    board_set(board, RF(0, 6), WHITE_KNIGHT);
    board_set(board, RF(0, 7), WHITE_ROOK);
    board_set(board, RF(7, 0), BLACK_ROOK);
    board_set(board, RF(7, 1), BLACK_KNIGHT);
    board_set(board, RF(7, 2), BLACK_BISHOP);
    board_set(board, RF(7, 3), BLACK_QUEEN);
    board_set(board, RF(7, 4), BLACK_KING);
    board_set(board, RF(7, 5), BLACK_BISHOP);
    board_set(board, RF(7, 6), BLACK_KNIGHT);
    board_set(board, RF(7, 7), BLACK_ROOK);
}

void board_set(Board *board, int sq, int piece) {
    int previous = board->squares[sq];
    board->squares[sq] = piece;
    if (previous) {
        bb mask = ~BIT(sq);
        board->all &= mask;
        if (COLOR(previous)) {
            board->black &= mask;
            switch (PIECE(previous)) {
                case PAWN:   board->black_pawns &= mask; break;
                case KNIGHT: board->black_knights &= mask; break;
                case BISHOP: board->black_bishops &= mask; break;
                case ROOK:   board->black_rooks &= mask; break;
                case QUEEN:  board->black_queens &= mask; break;
                case KING:   board->black_kings &= mask; break;
            }
        }
        else {
            board->white &= mask;
            switch (PIECE(previous)) {
                case PAWN:   board->white_pawns &= mask; break;
                case KNIGHT: board->white_knights &= mask; break;
                case BISHOP: board->white_bishops &= mask; break;
                case ROOK:   board->white_rooks &= mask; break;
                case QUEEN:  board->white_queens &= mask; break;
                case KING:   board->white_kings &= mask; break;
            }
        }
    }
    if (piece) {
        bb bit = BIT(sq);
        board->all |= bit;
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

void board_load_fen(Board *board, char *fen) {
    board_clear(board);
    int i = 0;
    int n = strlen(fen);
    int rank = 7;
    int file = 0;
    for (; i < n; i++) {
        int done = 0;
        switch (fen[i]) {
            case 'P': board_set(board, RF(rank, file++), WHITE_PAWN); break;
            case 'N': board_set(board, RF(rank, file++), WHITE_KNIGHT); break;
            case 'B': board_set(board, RF(rank, file++), WHITE_BISHOP); break;
            case 'R': board_set(board, RF(rank, file++), WHITE_ROOK); break;
            case 'Q': board_set(board, RF(rank, file++), WHITE_QUEEN); break;
            case 'K': board_set(board, RF(rank, file++), WHITE_KING); break;
            case 'p': board_set(board, RF(rank, file++), BLACK_PAWN); break;
            case 'n': board_set(board, RF(rank, file++), BLACK_KNIGHT); break;
            case 'b': board_set(board, RF(rank, file++), BLACK_BISHOP); break;
            case 'r': board_set(board, RF(rank, file++), BLACK_ROOK); break;
            case 'q': board_set(board, RF(rank, file++), BLACK_QUEEN); break;
            case 'k': board_set(board, RF(rank, file++), BLACK_KING); break;
            case '/': file = 0; rank--; break;
            case '1': file += 1; break;
            case '2': file += 2; break;
            case '3': file += 3; break;
            case '4': file += 4; break;
            case '5': file += 5; break;
            case '6': file += 6; break;
            case '7': file += 7; break;
            case '8': file += 8; break;
            case ' ': done = 1; break;
            default: return;
        }
        if (done) {
            if (rank != 0 || file != 8) {
                return;
            }
            break;
        }
    }
    i++;
    switch (fen[i++]) {
        case 'w': board->color = WHITE; break;
        case 'b': board->color = BLACK; break;
        default: return;
    }
    i++;
    board->castle = 0;
    for (; i < n; i++) {
        int done = 0;
        switch (fen[i]) {
            case 'K': board->castle |= CASTLE_WHITE_KING; break;
            case 'Q': board->castle |= CASTLE_WHITE_QUEEN; break;
            case 'k': board->castle |= CASTLE_BLACK_KING; break;
            case 'q': board->castle |= CASTLE_BLACK_QUEEN; break;
            case '-': done = 1; break;
            case ' ': done = 1; break;
            default: return;
        }
        if (done) {
            break;
        }
    }
    i++;
}

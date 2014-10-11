#include "move.h"

void do_move(Board *board, Move *move) {
    int piece = board->squares[move->src];
    board_set(board, move->src, EMPTY);
    board_set(board, move->dst, piece);
}

void move_notation(Board *board, Move *move, char *result) {
    int piece = board->squares[move->src];
    int capture = board->squares[move->dst] != EMPTY;
    // char rank1 = '1' + move->src / 8;
    char file1 = 'a' + move->src % 8;
    char rank2 = '1' + move->dst / 8;
    char file2 = 'a' + move->dst % 8;
    switch (PIECE(piece)) {
        case PAWN:
            if (capture) {
                *result++ = file1;
            }
            break;
        case KNIGHT: *result++ = 'N'; break;
        case BISHOP: *result++ = 'B'; break;
        case ROOK:   *result++ = 'R'; break;
        case QUEEN:  *result++ = 'Q'; break;
        case KING:   *result++ = 'K'; break;
    }
    if (capture) {
        *result++ = 'x';
    }
    *result++ = file2;
    *result++ = rank2;
    *result++ = 0;
}

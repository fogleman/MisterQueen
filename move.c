#include "move.h"

void do_move(Board *board, Move *move) {
    int piece = board->squares[move->src];
    board_set(board, move->src, EMPTY);
    board_set(board, move->dst, piece);
}

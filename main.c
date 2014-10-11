#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "bb.h"
#include "board.h"
#include "move.h"

int main(int argc, char **argv) {
    srand(time(NULL));
    bb_init();
    Board board;
    board_reset(&board);
    board_print(&board);
    Move moves[256];
    int count;
    count = gen_white_pawn_moves(&board, moves);
    printf("%d\n", count);
    count = gen_white_knight_moves(&board, moves);
    printf("%d\n", count);
    count = gen_white_bishop_moves(&board, moves);
    printf("%d\n", count);
    count = gen_white_rook_moves(&board, moves);
    printf("%d\n", count);
    count = gen_white_queen_moves(&board, moves);
    printf("%d\n", count);
    count = gen_white_king_moves(&board, moves);
    printf("%d\n", count);
    count = gen_white_moves(&board, moves);
    printf("%d\n", count);
    for (int i = 0; i < count; i++) {
        Move *move = moves + i;
        printf("%2d => %2d\n", move->src, move->dst);
    }
    return 0;
}

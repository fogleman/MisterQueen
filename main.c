#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "bb.h"
#include "board.h"
#include "gen.h"
#include "move.h"

int main(int argc, char **argv) {
    srand(time(NULL));
    bb_init();
    Board board;
    board_reset(&board);
    board_print(&board);
    Move moves[256];
    int count;
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            count = gen_white_moves(&board, moves);
        }
        else {
            count = gen_black_moves(&board, moves);
        }
        Move move = moves[rand() % count];
        do_move(&board, &move);
        board_print(&board);
    }
    return 0;
}

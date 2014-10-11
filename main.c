#include <stdlib.h>
#include <time.h>
#include "bb.h"
#include "board.h"

int main(int argc, char **argv) {
    srand(time(NULL));
    bb_init();
    bb obstacles = 0;
    for (int i = 0; i < 20; i++) {
        obstacles |= 1L << (rand() % 64);
    }
    bb_print(obstacles);
    bb moves = sq_queen(0, obstacles);
    bb_print(moves);
    Board board;
    board_reset(&board);
    board_print(&board);
    return 0;
}

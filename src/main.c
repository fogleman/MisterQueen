#include "bb.h"
#include "board.h"
#include "move.h"
#include "perft.h"
#include "search.h"

int main(int argc, char **argv) {
    bb_init();
    // perft_tests();
    Board board;
    Move move;
    board_reset(&board);
    // board_load_fen(&board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
    while (1) {
        board_print(&board);
        search(&board, 10, &move);
        make_move(&board, &move);
        // break;
    }
    return 0;
}

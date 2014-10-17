#include <stdio.h>
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
    // board_load_fen(&board, "rn2kb1r/pp1q2p1/2p1pn2/3p3p/3P1N2/2N2P2/PPP3PP/R1BQKB1R w KQkq - 4 10");
    while (1) {
        board_print(&board);
        search(&board, 1, &move);
        print_move(&board, &move);
        make_move(&board, &move);

        board_print(&board);
        while (1) {
            char notation[16];
            printf("Enter move: ");
            scanf("%15s", notation);
            if (parse_move(&board, notation, &move)) {
                make_move(&board, &move);
                break;
            }
            else {
                printf("Invalid move!\n");
            }
        }
    }
    return 0;
}

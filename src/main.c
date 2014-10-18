#include <stdio.h>
#include "bb.h"
#include "board.h"
#include "move.h"
#include "search.h"

int main(int argc, char **argv) {
    bb_init();
    Board board;
    Move move;
    board_reset(&board);
    while (1) {
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

        board_print(&board);
        search(&board, 1, &move);
        print_move(&board, &move);
        make_move(&board, &move);
    }
    return 0;
}

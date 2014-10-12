#include <stdio.h>
#include "gen.h"
#include "move.h"
#include "perft.h"

unsigned long long perft(Board *board, int depth) {
    if (depth == 0) {
        return 1L;
    }
    unsigned long long result = 0;
    Move moves[256];
    int count = gen_moves(board, moves);
    for (int i = 0; i < count; i++) {
        do_move(board, &moves[i]);
        if (!is_illegal(board)) {
            result += perft(board, depth - 1);
        }
        undo_move(board, &moves[i]);
    }
    return result;
}

void perft_test() {
    Board board;
    board_load_fen(&board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
    board_print(&board);
    for (int depth = 0; depth < 10; depth++) {
        unsigned long long result = perft(&board, depth);
        printf("depth = %d, nodes = %lld\n", depth, result);
    }
}

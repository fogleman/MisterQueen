#include <stdio.h>
#include "gen.h"
#include "move.h"
#include "perft.h"

static unsigned long long nodes;
static unsigned long long captures;
static unsigned long long checks;
static unsigned long long mates;

void perft(Board *board, int depth) {
    if (depth == 1 && is_check(board)) {
        checks++;
    }
    Move moves[256];
    int count = gen_moves(board, moves);
    if (depth == 1) {
        nodes += count;
        for (int i = 0; i < count; i++) {
            if (board->squares[moves[i].dst]) {
                captures++;
            }
        }
        return;
    }
    int legal = 0;
    for (int i = 0; i < count; i++) {
        do_move(board, &moves[i]);
        if (!is_illegal(board)) {
            legal = 1;
            perft(board, depth - 1);
        }
        undo_move(board, &moves[i]);
    }
    if (depth == 2 && legal == 0) {
        mates++;
    }
}

void perft_test() {
    int depth = 1;
    while (1) {
        nodes = captures = checks = mates = 0L;
        Board board;
        board_reset(&board);
        perft(&board, depth);
        printf("depth = %d, nodes = %lld, captures = %lld, checks = %lld, mates = %lld\n",
            depth, nodes, captures, checks, mates);
        depth++;
    }
}

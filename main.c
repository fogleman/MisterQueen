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
    char notation[16];
    int move_count, attack_count, check_count;
    for (int i = 0; i < 100; i++) {
        if (i % 2 == 0) {
            attack_count = gen_white_attacks_all(&board, moves);
            check_count = gen_white_checks(&board, moves);
            move_count = gen_white_moves(&board, moves);
        }
        else {
            attack_count = gen_black_attacks_all(&board, moves);
            check_count = gen_black_checks(&board, moves);
            move_count = gen_black_moves(&board, moves);
        }
        printf("%d, %d, %d\n", move_count, attack_count, check_count);
        Move move = moves[rand() % move_count];
        move_notation(&board, &move, notation);
        printf("%s\n", notation);
        do_move(&board, &move);
        board_print(&board);
    }
    return 0;
}

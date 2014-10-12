#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "bb.h"
#include "board.h"
#include "gen.h"
#include "move.h"
#include "perft.h"

void random_game() {
    Board board;
    board_reset(&board);
    Move moves[256];
    int move_count;
    // Move list[256];
    for (int i = 0; i < 50; i++) {
        if (i % 2 == 0) {
            move_count = gen_white_moves(&board, moves);
        }
        else {
            move_count = gen_black_moves(&board, moves);
        }
        if (move_count == 0) {
            return;
        }
        // Move move = moves[rand() % move_count];
        // do_move(&board, &move);
        // memcpy(&list[i], &move, sizeof(Move));
    }
    // board_print(&board);
    // for (int i = 49; i >= 0; i--) {
    //     Move move = list[i];
    //     undo_move(&board, &move);
    // }
    // board_print(&board);
}

void random_games() {
    for (int i = 0; i < 200000; i++) {
        random_game();
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));
    bb_init();
    perft_test();
    // random_games();
    // Board board;
    // board_reset(&board);
    // board_print(&board);
    // Move moves[256];
    // char notation[16];
    // int move_count, attack_count, check_count;
    // for (int i = 0; i < 100; i++) {
    //     if (i % 2 == 0) {
    //         attack_count = gen_white_attacks_all(&board, moves);
    //         check_count = gen_white_checks(&board, moves);
    //         move_count = gen_white_moves(&board, moves);
    //     }
    //     else {
    //         attack_count = gen_black_attacks_all(&board, moves);
    //         check_count = gen_black_checks(&board, moves);
    //         move_count = gen_black_moves(&board, moves);
    //     }
    //     printf("%d, %d, %d\n", move_count, attack_count, check_count);
    //     Move move = moves[rand() % move_count];
    //     move_notation(&board, &move, notation);
    //     printf("%s\n", notation);
    //     do_move(&board, &move);
    //     board_print(&board);
    // }
    return 0;
}

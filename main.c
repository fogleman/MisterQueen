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
    count = gen_white_moves(&board, moves);
    printf("%d\n", count);
    for (int i = 0; i < count; i++) {
        Move *move = moves + i;
        printf("%2d => %2d\n", move->src, move->dst);
    }
    count = gen_black_moves(&board, moves);
    printf("%d\n", count);
    for (int i = 0; i < count; i++) {
        Move *move = moves + i;
        printf("%2d => %2d\n", move->src, move->dst);
    }
    return 0;
}

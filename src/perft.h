#include "board.h"

unsigned long long perft(Board *board, int depth);
void perft_test(char *fen, unsigned long long *expected, int count);
void perft_tests();

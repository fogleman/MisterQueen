#ifndef EVAL_H
#define EVAL_H

#include "board.h"

#define SCORE_INF 100000
#define SCORE_KING 20000
#define SCORE_QUEEN 900
#define SCORE_ROOK 500
#define SCORE_BISHOP 325
#define SCORE_KNIGHT 300
#define SCORE_PAWN 100
#define SCORE_MOBILITY 1

int evaluate(Board *board);

#endif

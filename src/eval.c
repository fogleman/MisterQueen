#include "eval.h"
#include "gen.h"
#include "move.h"

int evaluate(Board *board) {
    int score = 0;
    score += board->white_material;
    score -= board->black_material;
    score += board->white_position;
    score -= board->black_position;
    return board->color ? -score : score;
}

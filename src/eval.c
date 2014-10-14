#include "eval.h"
#include "gen.h"
#include "move.h"

int evaluate(Board *board) {
    int score = 0;
    score += BITS(board->white_kings) * SCORE_KING;
    score += BITS(board->white_queens) * SCORE_QUEEN;
    score += BITS(board->white_rooks) * SCORE_ROOK;
    score += BITS(board->white_bishops) * SCORE_BISHOP;
    score += BITS(board->white_knights) * SCORE_KNIGHT;
    score += BITS(board->white_pawns) * SCORE_PAWN;
    score -= BITS(board->black_kings) * SCORE_KING;
    score -= BITS(board->black_queens) * SCORE_QUEEN;
    score -= BITS(board->black_rooks) * SCORE_ROOK;
    score -= BITS(board->black_bishops) * SCORE_BISHOP;
    score -= BITS(board->black_knights) * SCORE_KNIGHT;
    score -= BITS(board->black_pawns) * SCORE_PAWN;
    // Move moves[MAX_MOVES];
    // score += gen_white_moves(board, moves) * SCORE_MOBILITY;
    // score -= gen_black_moves(board, moves) * SCORE_MOBILITY;
    return board->color ? -score : score;
}

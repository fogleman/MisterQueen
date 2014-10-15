#include <stdio.h>
#include <string.h>
#include "search.h"
#include "eval.h"
#include "gen.h"
#include "move.h"

#define SIZE (1 << 23)
#define MASK ((SIZE) - 1)

typedef struct {
    bb key;
    bb value;
    int depth;
} Entry;

Entry TABLE[SIZE];

int quiesce(Board *board, int alpha, int beta) {
    int score = evaluate(board);
    if (score >= beta) {
        return beta;
    }
    if (score > alpha) {
        alpha = score;
    }
    Undo undo;
    Move moves[MAX_MOVES];
    int count = gen_attacks(board, moves);
    for (int i = 0; i < count; i++) {
        do_move(board, &moves[i], &undo);
        int score = -quiesce(board, -beta, -alpha);
        undo_move(board, &moves[i], &undo);
        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
        }
    }
    return alpha;
}

int alpha_beta(Board *board, int depth, int alpha, int beta) {
    int index = board->hash & MASK;
    Entry *entry = &TABLE[index];
    if (entry->key == board->hash && entry->depth == depth) {
        return entry->value;
    }

    if (depth == 0) {
        // return evaluate(board);
        return quiesce(board, alpha, beta);
    }
    Undo undo;
    Move moves[MAX_MOVES];
    int count = gen_moves(board, moves);
    for (int i = 0; i < count; i++) {
        do_move(board, &moves[i], &undo);
        int score = -alpha_beta(board, depth - 1, -beta, -alpha);
        undo_move(board, &moves[i], &undo);
        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
        }
    }

    entry->key = board->hash;
    entry->value = alpha;
    entry->depth = depth;
    return alpha;
}

int root_search(Board *board, int depth, Move *move) {
    int alpha = -SCORE_INF;
    int beta = SCORE_INF;
    Undo undo;
    Move moves[MAX_MOVES];
    int count = gen_moves(board, moves);
    for (int i = 0; i < count; i++) {
        do_move(board, &moves[i], &undo);
        int score = -alpha_beta(board, depth - 1, -beta, -alpha);
        undo_move(board, &moves[i], &undo);
        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            memcpy(move, &moves[i], sizeof(Move));
        }
    }
    return alpha;
}

void search(Board *board, Move *result) {
    Move move;
    for (int depth = 1; depth <= 8; depth++) {
        int score = root_search(board, depth, &move);
        printf("%d, %d, ", depth, score);
        print_move(board, &move);
    }
    memcpy(result, &move, sizeof(Move));
}

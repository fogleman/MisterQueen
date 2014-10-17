#include <stdio.h>
#include <string.h>
#include "search.h"
#include "eval.h"
#include "gen.h"
#include "move.h"
#include "table.h"
#include "util.h"

static Table TABLE;

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
        Move *move = &moves[i];
        int p1 = PIECE(board->squares[move->src]);
        int p2 = PIECE(board->squares[move->dst]);
        if (p2 < p1) {
            continue;
        }
        do_move(board, move, &undo);
        int score = -quiesce(board, -beta, -alpha);
        undo_move(board, move, &undo);
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
    if (is_illegal(board)) {
        return board->color ? -INF : INF;
    }
    Entry *entry = table_get(&TABLE, board->hash);
    if (entry->key == board->hash && entry->depth == depth) {
        return entry->value;
    }
    if (depth <= 0) {
        // return evaluate(board);
        return quiesce(board, alpha, beta);
    }
    do_null_move(board);
    int score = -alpha_beta(board, depth - 1 - 2, -beta, -beta + 1);
    undo_null_move(board);
    if (score >= beta) {
        return beta;
    }
    Undo undo;
    Move moves[MAX_MOVES];
    int count = gen_moves(board, moves);
    for (int i = 0; i < count; i++) {
        Move *move = &moves[i];
        do_move(board, move, &undo);
        int score = -alpha_beta(board, depth - 1, -beta, -alpha);
        undo_move(board, move, &undo);
        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
        }
    }
    table_set(&TABLE, board->hash, depth, alpha, NULL);
    return alpha;
}

int root_search(Board *board, int depth, int alpha, int beta, Move *result) {
    Undo undo;
    Move moves[MAX_MOVES];
    int count = gen_moves(board, moves);
    for (int i = 0; i < count; i++) {
        Move *move = &moves[i];
        do_move(board, move, &undo);
        int score = -alpha_beta(board, depth - 1, -beta, -alpha);
        undo_move(board, move, &undo);
        // printf("%d: ", score);
        // print_move(board, move);
        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            memcpy(result, move, sizeof(Move));
        }
    }
    return alpha;
}

void search(Board *board, double duration, Move *move) {
    table_alloc(&TABLE, 22);
    double start = now();
    int alpha = -INF;
    int beta = INF;
    int window = 100;
    int depth = 1;
    while (1) {
        int score = root_search(board, depth, alpha, beta, move);
        if (score <= alpha || score >= beta) {
            alpha = -INF;
            beta = INF;
            continue;
        }
        printf("%d, %d, ", depth, score);
        print_move(board, move);
        if (now() - start > duration) {
            break;
        }
        // alpha = score - window;
        // beta = score + window;
        depth++;
    }
    table_free(&TABLE);
}

#include <stdio.h>
#include <string.h>
#include "search.h"
#include "eval.h"
#include "gen.h"
#include "move.h"
#include "table.h"
#include "util.h"

static Table TABLE;

#define XOR_SWAP(a, b) a = a ^ b; b = a ^ b; a = a ^ b;

// TODO: don't use a global here
static int root_depth;

void sort_moves(Board *board, Move *moves, int count) {
    int scores[MAX_MOVES];
    int indexes[MAX_MOVES];
    Undo undo;
    for (int i = 0; i < count; i++) {
        Move *move = moves + i;
        do_move(board, move, &undo);
        scores[i] = evaluate(board);
        indexes[i] = i;
        undo_move(board, move, &undo);
    }
    for (int i = 1; i < count; i++) {
        int j = i;
        while (j > 0 && scores[j - 1] > scores[j]) {
            XOR_SWAP(scores[j - 1], scores[j]);
            XOR_SWAP(indexes[j - 1], indexes[j]);
            j--;
        }
    }
    Move temp[MAX_MOVES];
    memcpy(temp, moves, sizeof(Move) * count);
    for (int i = 0; i < count; i++) {
        memcpy(moves + i, temp + indexes[i], sizeof(Move));
    }
}

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
    sort_moves(board, moves, count);
    for (int i = 0; i < count; i++) {
        Move *move = &moves[i];
        int p1 = PIECE(board->squares[move->src]);
        int p2 = PIECE(board->squares[move->dst]);
        if (p2 < p1) {
            // continue;
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
        return INF;
    }
    Entry *entry = table_get(&TABLE, board->hash);
    if (entry->key == board->hash && entry->depth == depth) {
        return entry->value;
    }
    if (depth <= 0) {
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
    sort_moves(board, moves, count);
    int can_move = 0;
    Move *best = NULL;
    for (int i = 0; i < count; i++) {
        Move *move = &moves[i];
        do_move(board, move, &undo);
        int score = -alpha_beta(board, depth - 1, -beta, -alpha);
        undo_move(board, move, &undo);
        if (score > -INF) {
            can_move = 1;
        }
        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            best = move;
        }
    }
    if (!can_move) {
        if (is_check(board)) {
            return -MATE + root_depth - depth;
        }
        else {
            return 0;
        }
    }
    table_set(&TABLE, board->hash, depth, alpha, best);
    return alpha;
}

int root_search(Board *board, int depth, int alpha, int beta, Move *result) {
    Undo undo;
    Move moves[MAX_MOVES];
    int count = gen_moves(board, moves);
    sort_moves(board, moves, count);
    Move *best = NULL;
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
            memcpy(result, move, sizeof(Move));
            best = move;
        }
    }
    table_set(&TABLE, board->hash, depth, alpha, best);
    return alpha;
}

void print_pv(Board *board, int depth) {
    if (depth <= 0) {
        return;
    }
    Entry *entry = table_get(&TABLE, board->hash);
    if (entry->key != board->hash) {
        return;
    }
    print_move(board, &entry->move);
    printf(" ");
    Undo undo;
    do_move(board, &entry->move, &undo);
    print_pv(board, depth - 1);
    undo_move(board, &entry->move, &undo);
}

int search(Board *board, double duration, Move *move) {
    int result = 1;
    table_alloc(&TABLE, 22);
    double start = now();
    for (int depth = 1; depth < 100; depth++) {
        root_depth = depth;
        int score = root_search(board, depth, -INF, INF, move);
        if (score == -INF) {
            result = 0;
            break;
        }
        printf("%3d, %4d, ", depth, score);
        print_move(board, move);
        printf(" [ ");
        print_pv(board, depth);
        printf("]\n");
        if (now() - start > duration) {
            break;
        }
        if (score <= -MATE + depth || score >= MATE - depth) {
            break;
        }
    }
    table_free(&TABLE);
    return result;
}

#include <stdio.h>
#include <string.h>
#include "search.h"
#include "book.h"
#include "eval.h"
#include "gen.h"
#include "move.h"
#include "table.h"
#include "util.h"

#define XOR_SWAP(a, b) a = a ^ b; b = a ^ b; a = a ^ b;

static volatile int stop_flag;

static Table TABLE;
static int root_depth;
static int nodes;

void sort_moves(Board *board, Move *moves, int count) {
    Move *best = table_get_move(&TABLE, board->hash);
    int scores[MAX_MOVES];
    int indexes[MAX_MOVES];
    for (int i = 0; i < count; i++) {
        Move *move = moves + i;
        scores[i] = score_move(board, move);
        if (best && memcmp(best, move, sizeof(Move)) == 0) {
            scores[i] += INF;
        }
        indexes[i] = i;
    }
    for (int i = 1; i < count; i++) {
        int j = i;
        while (j > 0 && scores[j - 1] < scores[j]) {
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
    if (is_illegal(board)) {
        return INF;
    }
    int score = evaluate(board);
    if (score >= beta) {
        return beta;
    }
    if (score > alpha) {
        alpha = score;
    }
    Undo undo;
    Move moves[MAX_MOVES];
    // TODO: only good attacks
    int count = gen_attacks(board, moves);
    sort_moves(board, moves, count);
    for (int i = 0; i < count; i++) {
        Move *move = &moves[i];
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
    int value;
    if (table_get(&TABLE, board->hash, depth, alpha, beta, &value)) {
        return value;
    }
    if (depth <= 0) {
        int value = quiesce(board, alpha, beta);
        table_set(&TABLE, board->hash, depth, value, TABLE_EXACT);
        return value;
    }
    nodes++;
    Undo undo;
    do_null_move(board, &undo);
    int score = -alpha_beta(board, depth - 1 - 2, -beta, -beta + 1);
    undo_null_move(board, &undo);
    if (score >= beta) {
        table_set(&TABLE, board->hash, depth, beta, TABLE_BETA);
        return beta;
    }
    Move moves[MAX_MOVES];
    int count = gen_moves(board, moves);
    sort_moves(board, moves, count);
    int can_move = 0;
    int flag = TABLE_ALPHA;
    for (int i = 0; i < count; i++) {
        Move *move = &moves[i];
        do_move(board, move, &undo);
        int score = -alpha_beta(board, depth - 1, -beta, -alpha);
        undo_move(board, move, &undo);
        if (stop_flag) {
            return 0;
        }
        if (score > -INF) {
            can_move = 1;
        }
        if (score >= beta) {
            table_set(&TABLE, board->hash, depth, beta, TABLE_BETA);
            table_set_move(&TABLE, board->hash, depth, move);
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            flag = TABLE_EXACT;
            table_set_move(&TABLE, board->hash, depth, move);
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
    table_set(&TABLE, board->hash, depth, alpha, flag);
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
        if (stop_flag) {
            return 0;
        }
        if (score > alpha) {
            alpha = score;
            best = move;
        }
    }
    if (best) {
        memcpy(result, best, sizeof(Move));
        table_set_move(&TABLE, board->hash, depth, best);
    }
    return alpha;
}

void print_pv(Board *board, int depth) {
    if (depth <= 0) {
        return;
    }
    Entry *entry = TABLE_ENTRY(&TABLE, board->hash);
    if (entry->key != board->hash) {
        return;
    }
    char move_string[16];
    move_to_string(&entry->move, move_string);
    printf(" %s", move_string);
    Undo undo;
    do_move(board, &entry->move, &undo);
    print_pv(board, depth - 1);
    undo_move(board, &entry->move, &undo);
}

int search(Board *board, SearchParameters *parameters, Move *move) {
    if (parameters->use_book && book_move(board, move)) {
        char move_string[16];
        move_to_string(move, move_string);
        printf("bestmove %s\n", move_string);
        return 1;
    }
    stop_flag = 0;
    int result = 1;
    table_alloc(&TABLE, 20);
    double start = now();
    double duration = parameters->duration;
    int score = 0;
    for (int depth = 1; depth < 100; depth++) {
        root_depth = depth;
        nodes = 0;
        int lo = 5;
        int hi = 5;
        while (1) {
            int alpha = score - lo;
            int beta = score + hi;
            score = root_search(board, depth, alpha, beta, move);
            if (stop_flag) {
                break;
            }
            if (score == alpha) {
                lo *= 5;
            }
            else if (score == beta) {
                hi *= 5;
            }
            else {
                break;
            }
        }
        if (stop_flag) {
            break;
        }
        if (score == -INF) {
            result = 0;
            break;
        }
        double elapsed = now() - start;
        char move_string[16];
        move_to_string(move, move_string);
        int millis = elapsed * 1000;
        printf("info depth %d score cp %d nodes %d time %d pv",
            depth, score, nodes, millis);
        print_pv(board, depth);
        printf("\n");
        if (duration > 0 && elapsed > duration) {
            break;
        }
        if (score <= -MATE + depth || score >= MATE - depth) {
            break;
        }
    }
    char move_string[16];
    move_to_string(move, move_string);
    printf("bestmove %s\n", move_string);
    table_free(&TABLE);
    return result;
}

void stop_search() {
    stop_flag = 1;
}

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "search.h"
#include "book.h"
#include "eval.h"
#include "gen.h"
#include "move.h"
#include "table.h"
#include "tinycthread.h"
#include "util.h"

#define XOR_SWAP(a, b) a = a ^ b; b = a ^ b; a = a ^ b;

void sort_moves(Search *search, Board *board, Move *moves, int count) {
    Move *best = table_get_move(&search->table, board->hash);
    int scores[MAX_MOVES];
    int indexes[MAX_MOVES];
    for (int i = 0; i < count; i++) {
        Move *move = moves + i;
        scores[i] = score_move(board, move);
        if (best && best->src == move->src && best->dst == move->dst) {
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

int quiesce(Search *search, Board *board, int alpha, int beta) {
    if (is_illegal(board)) {
        return INF;
    }
    int pawn_score = 0;
    if (!pawn_table_get(&search->pawn_table, board->pawn_hash, &pawn_score)) {
        pawn_score = evaluate_pawns(board);
        pawn_table_set(&search->pawn_table, board->pawn_hash, pawn_score);
    }
    int score = evaluate(board) + pawn_score;
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
    sort_moves(search, board, moves, count);
    for (int i = 0; i < count; i++) {
        Move *move = &moves[i];
        search->nodes++;
        do_move(board, move, &undo);
        int score = -quiesce(search, board, -beta, -alpha);
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

int alpha_beta(Search *search, Board *board, int depth, int ply, int alpha, int beta) {
    if (is_illegal(board)) {
        return INF;
    }
    int value;
    if (table_get(&search->table, board->hash, depth, alpha, beta, &value)) {
        return value;
    }
    if (depth <= 0) {
        int value = quiesce(search, board, alpha, beta);
        table_set(&search->table, board->hash, depth, value, TABLE_EXACT);
        return value;
    }
    Undo undo;
    search->nodes++;
    do_null_move(board, &undo);
    int score = -alpha_beta(search, board, depth - 1 - 2, ply + 1, -beta, -beta + 1);
    undo_null_move(board, &undo);
    if (score >= beta) {
        table_set(&search->table, board->hash, depth, beta, TABLE_BETA);
        return beta;
    }
    Move moves[MAX_MOVES];
    int count = gen_moves(board, moves);
    sort_moves(search, board, moves, count);
    int can_move = 0;
    int flag = TABLE_ALPHA;
    for (int i = 0; i < count; i++) {
        Move *move = &moves[i];
        search->nodes++;
        do_move(board, move, &undo);
        int score = -alpha_beta(search, board, depth - 1, ply + 1, -beta, -alpha);
        undo_move(board, move, &undo);
        if (search->stop) {
            return 0;
        }
        if (score > -INF) {
            can_move = 1;
        }
        if (score >= beta) {
            table_set(&search->table, board->hash, depth, beta, TABLE_BETA);
            table_set_move(&search->table, board->hash, depth, move);
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            flag = TABLE_EXACT;
            table_set_move(&search->table, board->hash, depth, move);
        }
    }
    if (!can_move) {
        if (is_check(board)) {
            return -MATE + ply;
        }
        else {
            return 0;
        }
    }
    table_set(&search->table, board->hash, depth, alpha, flag);
    return alpha;
}

int root_search(Search *search, Board *board, int depth, int ply, int alpha, int beta, Move *result) {
    Undo undo;
    Move moves[MAX_MOVES];
    int count = gen_moves(board, moves);
    sort_moves(search, board, moves, count);
    Move *best = NULL;
    for (int i = 0; i < count; i++) {
        Move *move = &moves[i];
        search->nodes++;
        do_move(board, move, &undo);
        int score = -alpha_beta(search, board, depth - 1, ply + 1, -beta, -alpha);
        undo_move(board, move, &undo);
        if (search->stop) {
            return 0;
        }
        if (score > alpha) {
            alpha = score;
            best = move;
        }
    }
    if (best) {
        memcpy(result, best, sizeof(Move));
        table_set_move(&search->table, board->hash, depth, best);
    }
    return alpha;
}

void print_pv(Search *search, Board *board, int depth) {
    if (depth <= 0) {
        return;
    }
    Entry *entry = TABLE_ENTRY(&search->table, board->hash);
    if (entry->key != board->hash) {
        return;
    }
    char move_string[16];
    move_to_string(&entry->move, move_string);
    printf(" %s", move_string);
    Undo undo;
    do_move(board, &entry->move, &undo);
    print_pv(search, board, depth - 1);
    undo_move(board, &entry->move, &undo);
}

static int thread_func(void *arg) {
    Search *search = arg;
    sleep(search->duration);
    search->stop = 1;
    return 0;
}

static void thread_start(Search *search) {
    thrd_t thrd;
    thrd_create(&thrd, thread_func, search);
}

int do_search(Search *search, Board *board) {
    if (search->use_book && book_move(board, &search->move)) {
        sleep(1);
        char move_string[16];
        move_to_string(&search->move, move_string);
        printf("bestmove %s\n", move_string);
        return 1;
    }
    search->stop = 0;
    int result = 1;
    table_alloc(&search->table, 20);
    pawn_table_alloc(&search->pawn_table, 20);
    double start = now();
    double duration = search->duration;
    if (duration > 0) {
        thread_start(search);
    }
    int score = 0;
    search->nodes = 0;
    for (int depth = 1; depth < 100; depth++) {
        int lo = 20;
        int hi = 20;
        while (1) {
            int alpha = score - lo;
            int beta = score + hi;
            score = root_search(search, board, depth, 0, alpha, beta, &search->move);
            if (search->stop) {
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
        if (search->stop) {
            break;
        }
        if (score == -INF) {
            result = 0;
            break;
        }
        double elapsed = now() - start;
        if (search->uci) {
            char move_string[16];
            move_to_string(&search->move, move_string);
            int millis = elapsed * 1000;
            printf("info depth %d score cp %d nodes %d time %d pv",
                depth, score, search->nodes, millis);
            print_pv(search, board, depth);
            printf("\n");
        }
        if (duration > 0 && elapsed > duration) {
            break;
        }
        if (score <= -MATE + depth || score >= MATE - depth) {
            break;
        }
    }
    if (now() - start < 1) {
        sleep(1);
    }
    if (search->uci) {
        char move_string[16];
        move_to_string(&search->move, move_string);
        printf("bestmove %s\n", move_string);
    }
    table_free(&search->table);
    pawn_table_free(&search->pawn_table);
    return result;
}

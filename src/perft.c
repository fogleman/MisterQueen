#include <stdio.h>
#include "gen.h"
#include "move.h"
#include "perft.h"
#include "util.h"

#define SIZE (1 << 20)
#define MASK ((SIZE) - 1)

typedef struct {
    bb key;
    bb value;
    int depth;
} Entry;

Entry TABLE[SIZE];

static int hits;

unsigned long long perft(Board *board, int depth) {
    if (is_illegal(board)) {
        return 0L;
    }
    if (depth == 0) {
        return 1L;
    }
    int index = board->hash & MASK;
    Entry *entry = &TABLE[index];
    if (entry->key == board->hash && entry->depth == depth) {
        hits += entry->value;
        return entry->value;
    }
    unsigned long long result = 0;
    Undo undo;
    Move moves[MAX_MOVES];
    int count = gen_moves(board, moves);
    for (int i = 0; i < count; i++) {
        do_move(board, &moves[i], &undo);
        result += perft(board, depth - 1);
        undo_move(board, &moves[i], &undo);
    }
    entry->key = board->hash;
    entry->value = result;
    entry->depth = depth;
    return result;
}

void perft_test(char *fen, unsigned long long *expected, int count) {
    Board board;
    board_load_fen(&board, fen);
    board_print(&board);
    double start = now();
    for (int depth = 0; depth < count; depth++) {
        hits = 0;
        unsigned long long actual = perft(&board, depth);
        char *result = actual == expected[depth] ? "PASS" : "FAIL";
        double elapsed = now() - start;
        printf("%s: depth = %2d, time = %.3f, expected = %12llu, actual = %12llu, hits = %g%%\n",
            result, depth, elapsed, expected[depth], actual, 100.0 * hits / actual);
    }
    printf("\n");
}

void perft_tests() {
    char *fen1 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    unsigned long long nodes1[] = {
        1L,
        20L,
        400L,
        8902L,
        197281L,
        4865609L,
        119060324L,
        // 3195901860L,
        // 84998978956L,
    };

    char *fen2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
    unsigned long long nodes2[] = {
        1L,
        48L,
        2039L,
        97862L,
        4085603L,
        193690690L,
    };

    char *fen3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";
    unsigned long long nodes3[] = {
        1L,
        14L,
        191L,
        2812L,
        43238L,
        674624L,
        11030083L,
        178633661L,
    };

    char *fen4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
    unsigned long long nodes4[] = {
        1L,
        6L,
        264L,
        9467L,
        422333L,
        15833292L,
        706045033L,
    };

    char *fen5 = "rnbqkb1r/pp1p1ppp/2p5/4P3/2B5/8/PPP1NnPP/RNBQK2R w KQkq - 0 6";
    unsigned long long nodes5[] = {
        1L,
        42L,
        1352L,
        53392L,
    };

    char *fen6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
    unsigned long long nodes6[] = {
        1L,
        46L,
        2079L,
        89890L,
        3894594L,
        164075551L,
        6923051137L,
        287188994746L,
        11923589843526L,
        490154852788714L,
    };

    perft_test(fen1, nodes1, sizeof(nodes1) / sizeof(unsigned long long));
    perft_test(fen2, nodes2, sizeof(nodes2) / sizeof(unsigned long long));
    perft_test(fen3, nodes3, sizeof(nodes3) / sizeof(unsigned long long));
    perft_test(fen4, nodes4, sizeof(nodes4) / sizeof(unsigned long long));
    perft_test(fen5, nodes5, sizeof(nodes5) / sizeof(unsigned long long));
    perft_test(fen6, nodes6, sizeof(nodes6) / sizeof(unsigned long long));
}

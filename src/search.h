#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "move.h"
#include "table.h"

#define INF 1000000
#define MATE 100000

typedef struct {
    // input
    int uci;
    int use_book;
    double duration;
    // output
    Move move;
    // control
    int stop;
    // internal
    int nodes;
    Table table;
    PawnTable pawn_table;
} Search;

int do_search(Search *search, Board *board);

#endif

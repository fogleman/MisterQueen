#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "move.h"
#include "table.h"

#define INF 1000000
#define MATE 100000

typedef struct {
    double duration;
    int use_book;
    int root_depth;
    int nodes;
    int score;
    Move move;
    Table table;
    volatile int stop_flag;
} Search;

int do_search(Search *search, Board *board);
void stop_search();

#endif

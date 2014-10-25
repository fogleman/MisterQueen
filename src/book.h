#ifndef BOOK_H
#define BOOK_H

#include "bb.h"
#include "board.h"
#include "move.h"

typedef struct {
    bb hash;
    unsigned char src;
    unsigned char dst;
    int count;
} BookEntry;

int book_move(Board *board, Move *result);

#endif

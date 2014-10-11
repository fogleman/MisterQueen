#include "move.h"

#define EMIT_MOVE(m, a, b) (m)->src = (a); (m)->dst = (b); (m)++;

int gen_white_pawn_moves(Board *board, Move *moves) {
    Move *ptr = moves;
    bb all = board->white | board->black;
    bb pawns = board->white_pawns;
    bb p1 = (pawns << 8) & ~all;
    bb p2 = ((p1 & 0x0000000000ff0000L) << 8) & ~all;
    bb a1 = ((pawns & 0x7f7f7f7f7f7f7f7fL) << 7) & board->black;
    bb a2 = ((pawns & 0xfefefefefefefefeL) << 9) & board->black;
    while (p1) {
        int sq = LSB(p1);
        p1 &= ~SQ(sq);
        EMIT_MOVE(moves, sq - 8, sq);
    }
    while (p2) {
        int sq = LSB(p2);
        p2 &= ~SQ(sq);
        EMIT_MOVE(moves, sq - 16, sq);
    }
    while (a1) {
        int sq = LSB(a1);
        a1 &= ~SQ(sq);
        EMIT_MOVE(moves, sq - 7, sq);
    }
    while (a2) {
        int sq = LSB(a2);
        a2 &= ~SQ(sq);
        EMIT_MOVE(moves, sq - 9, sq);
    }
    return moves - ptr;
}

int gen_white_knight_moves(Board *board, Move *moves) {
    Move *ptr = moves;
    bb srcs = board->white_knights;
    bb mask = ~board->white;
    while (srcs) {
        int src = LSB(srcs);
        srcs &= ~SQ(src);
        bb dsts = SQ_KNIGHT[src] & mask;
        while (dsts) {
            int dst = LSB(dsts);
            dsts &= ~SQ(dst);
            EMIT_MOVE(moves, src, dst);
        }
    }
    return moves - ptr;
}

int gen_white_bishop_moves(Board *board, Move *moves) {
    Move *ptr = moves;
    bb srcs = board->white_bishops;
    bb mask = ~board->white;
    bb all = board->white | board->black;
    while (srcs) {
        int src = LSB(srcs);
        srcs &= ~SQ(src);
        bb dsts = sq_bishop(src, all) & mask;
        while (dsts) {
            int dst = LSB(dsts);
            dsts &= ~SQ(dst);
            EMIT_MOVE(moves, src, dst);
        }
    }
    return moves - ptr;
}

int gen_white_rook_moves(Board *board, Move *moves) {
    Move *ptr = moves;
    bb srcs = board->white_rooks;
    bb mask = ~board->white;
    bb all = board->white | board->black;
    while (srcs) {
        int src = LSB(srcs);
        srcs &= ~SQ(src);
        bb dsts = sq_rook(src, all) & mask;
        while (dsts) {
            int dst = LSB(dsts);
            dsts &= ~SQ(dst);
            EMIT_MOVE(moves, src, dst);
        }
    }
    return moves - ptr;
}

int gen_white_queen_moves(Board *board, Move *moves) {
    Move *ptr = moves;
    bb srcs = board->white_queens;
    bb mask = ~board->white;
    bb all = board->white | board->black;
    while (srcs) {
        int src = LSB(srcs);
        srcs &= ~SQ(src);
        bb dsts = sq_queen(src, all) & mask;
        while (dsts) {
            int dst = LSB(dsts);
            dsts &= ~SQ(dst);
            EMIT_MOVE(moves, src, dst);
        }
    }
    return moves - ptr;
}

int gen_white_king_moves(Board *board, Move *moves) {
    Move *ptr = moves;
    bb srcs = board->white_kings;
    bb mask = ~board->white;
    while (srcs) {
        int src = LSB(srcs);
        srcs &= ~SQ(src);
        bb dsts = SQ_KING[src] & mask;
        while (dsts) {
            int dst = LSB(dsts);
            dsts &= ~SQ(dst);
            EMIT_MOVE(moves, src, dst);
        }
    }
    return moves - ptr;
}

int gen_white_moves(Board *board, Move *moves) {
    Move *ptr = moves;
    moves += gen_white_pawn_moves(board, moves);
    moves += gen_white_knight_moves(board, moves);
    moves += gen_white_bishop_moves(board, moves);
    moves += gen_white_rook_moves(board, moves);
    moves += gen_white_queen_moves(board, moves);
    moves += gen_white_king_moves(board, moves);
    return moves - ptr;
}

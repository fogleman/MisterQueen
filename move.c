#include "move.h"

#define EMIT_MOVE(m, a, b) (m)->src = (a); (m)->dst = (b); (m)++;

// generic move generators
int gen_knight_moves(Move *moves, bb srcs, bb mask) {
    Move *ptr = moves;
    int src, dst;
    while (srcs) {
        POP_LSB(src, srcs);
        bb dsts = SQ_KNIGHT[src] & mask;
        while (dsts) {
            POP_LSB(dst, dsts);
            EMIT_MOVE(moves, src, dst);
        }
    }
    return moves - ptr;
}

int gen_bishop_moves(Move *moves, bb srcs, bb mask, bb all) {
    Move *ptr = moves;
    int src, dst;
    while (srcs) {
        POP_LSB(src, srcs);
        bb dsts = sq_bishop(src, all) & mask;
        while (dsts) {
            POP_LSB(dst, dsts);
            EMIT_MOVE(moves, src, dst);
        }
    }
    return moves - ptr;
}

int gen_rook_moves(Move *moves, bb srcs, bb mask, bb all) {
    Move *ptr = moves;
    int src, dst;
    while (srcs) {
        POP_LSB(src, srcs);
        bb dsts = sq_rook(src, all) & mask;
        while (dsts) {
            POP_LSB(dst, dsts);
            EMIT_MOVE(moves, src, dst);
        }
    }
    return moves - ptr;
}

int gen_queen_moves(Move *moves, bb srcs, bb mask, bb all) {
    Move *ptr = moves;
    int src, dst;
    while (srcs) {
        POP_LSB(src, srcs);
        bb dsts = sq_queen(src, all) & mask;
        while (dsts) {
            POP_LSB(dst, dsts);
            EMIT_MOVE(moves, src, dst);
        }
    }
    return moves - ptr;
}

int gen_king_moves(Move *moves, bb srcs, bb mask) {
    Move *ptr = moves;
    int src, dst;
    while (srcs) {
        POP_LSB(src, srcs);
        bb dsts = SQ_KING[src] & mask;
        while (dsts) {
            POP_LSB(dst, dsts);
            EMIT_MOVE(moves, src, dst);
        }
    }
    return moves - ptr;
}

// white move generators
int gen_white_pawn_moves(Board *board, Move *moves) {
    Move *ptr = moves;
    bb pawns = board->white_pawns;
    bb p1 = (pawns << 8) & ~board->all;
    bb p2 = ((p1 & 0x0000000000ff0000L) << 8) & ~board->all;
    bb a1 = ((pawns & 0x7f7f7f7f7f7f7f7fL) << 7) & board->black;
    bb a2 = ((pawns & 0xfefefefefefefefeL) << 9) & board->black;
    int sq;
    while (p1) {
        POP_LSB(sq, p1);
        EMIT_MOVE(moves, sq - 8, sq);
    }
    while (p2) {
        POP_LSB(sq, p2);
        EMIT_MOVE(moves, sq - 16, sq);
    }
    while (a1) {
        POP_LSB(sq, a1);
        EMIT_MOVE(moves, sq - 7, sq);
    }
    while (a2) {
        POP_LSB(sq, a2);
        EMIT_MOVE(moves, sq - 9, sq);
    }
    return moves - ptr;
}

int gen_white_knight_moves(Board *board, Move *moves) {
    return gen_knight_moves(
        moves, board->white_knights, ~board->white);
}

int gen_white_bishop_moves(Board *board, Move *moves) {
    return gen_bishop_moves(
        moves, board->white_bishops, ~board->white, board->all);
}

int gen_white_rook_moves(Board *board, Move *moves) {
    return gen_rook_moves(
        moves, board->white_rooks, ~board->white, board->all);
}

int gen_white_queen_moves(Board *board, Move *moves) {
    return gen_queen_moves(
        moves, board->white_queens, ~board->white, board->all);
}

int gen_white_king_moves(Board *board, Move *moves) {
    return gen_king_moves(
        moves, board->white_kings, ~board->white);
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

// white attack generators
int gen_white_pawn_attacks(Board *board, Move *moves, bb mask) {
    Move *ptr = moves;
    bb pawns = board->white_pawns;
    bb a1 = ((pawns & 0x7f7f7f7f7f7f7f7fL) << 7) & mask;
    bb a2 = ((pawns & 0xfefefefefefefefeL) << 9) & mask;
    int sq;
    while (a1) {
        POP_LSB(sq, a1);
        EMIT_MOVE(moves, sq - 7, sq);
    }
    while (a2) {
        POP_LSB(sq, a2);
        EMIT_MOVE(moves, sq - 9, sq);
    }
    return moves - ptr;
}

int gen_white_knight_attacks(Board *board, Move *moves, bb mask) {
    return gen_knight_moves(
        moves, board->white_knights, mask);
}

int gen_white_bishop_attacks(Board *board, Move *moves, bb mask) {
    return gen_bishop_moves(
        moves, board->white_bishops, mask, board->all);
}

int gen_white_rook_attacks(Board *board, Move *moves, bb mask) {
    return gen_rook_moves(
        moves, board->white_rooks, mask, board->all);
}

int gen_white_queen_attacks(Board *board, Move *moves, bb mask) {
    return gen_queen_moves(
        moves, board->white_queens, mask, board->all);
}

int gen_white_king_attacks(Board *board, Move *moves, bb mask) {
    return gen_king_moves(
        moves, board->white_kings, mask);
}

int gen_white_attacks(Board *board, Move *moves, bb mask) {
    Move *ptr = moves;
    moves += gen_white_pawn_attacks(board, moves, mask);
    moves += gen_white_knight_attacks(board, moves, mask);
    moves += gen_white_bishop_attacks(board, moves, mask);
    moves += gen_white_rook_attacks(board, moves, mask);
    moves += gen_white_queen_attacks(board, moves, mask);
    moves += gen_white_king_attacks(board, moves, mask);
    return moves - ptr;
}

int gen_white_checks(Board *board, Move *moves) {
    return gen_white_attacks(board, moves, board->black_kings);
}

// black move generators
int gen_black_pawn_moves(Board *board, Move *moves) {
    Move *ptr = moves;
    bb pawns = board->black_pawns;
    bb p1 = (pawns >> 8) & ~board->all;
    bb p2 = ((p1 & 0x0000ff0000000000L) >> 8) & ~board->all;
    bb a1 = ((pawns & 0x7f7f7f7f7f7f7f7fL) >> 9) & board->white;
    bb a2 = ((pawns & 0xfefefefefefefefeL) >> 7) & board->white;
    int sq;
    while (p1) {
        POP_LSB(sq, p1);
        EMIT_MOVE(moves, sq + 8, sq);
    }
    while (p2) {
        POP_LSB(sq, p2);
        EMIT_MOVE(moves, sq + 16, sq);
    }
    while (a1) {
        POP_LSB(sq, a1);
        EMIT_MOVE(moves, sq + 9, sq);
    }
    while (a2) {
        POP_LSB(sq, a2);
        EMIT_MOVE(moves, sq + 7, sq);
    }
    return moves - ptr;
}

int gen_black_knight_moves(Board *board, Move *moves) {
    return gen_knight_moves(
        moves, board->black_knights, ~board->black);
}

int gen_black_bishop_moves(Board *board, Move *moves) {
    return gen_bishop_moves(
        moves, board->black_bishops, ~board->black, board->all);
}

int gen_black_rook_moves(Board *board, Move *moves) {
    return gen_rook_moves(
        moves, board->black_rooks, ~board->black, board->all);
}

int gen_black_queen_moves(Board *board, Move *moves) {
    return gen_queen_moves(
        moves, board->black_queens, ~board->black, board->all);
}

int gen_black_king_moves(Board *board, Move *moves) {
    return gen_king_moves(
        moves, board->black_kings, ~board->black);
}

int gen_black_moves(Board *board, Move *moves) {
    Move *ptr = moves;
    moves += gen_black_pawn_moves(board, moves);
    moves += gen_black_knight_moves(board, moves);
    moves += gen_black_bishop_moves(board, moves);
    moves += gen_black_rook_moves(board, moves);
    moves += gen_black_queen_moves(board, moves);
    moves += gen_black_king_moves(board, moves);
    return moves - ptr;
}

// black attack generators
int gen_black_pawn_attacks(Board *board, Move *moves, bb mask) {
    Move *ptr = moves;
    bb pawns = board->black_pawns;
    bb a1 = ((pawns & 0x7f7f7f7f7f7f7f7fL) >> 9) & mask;
    bb a2 = ((pawns & 0xfefefefefefefefeL) >> 7) & mask;
    int sq;
    while (a1) {
        POP_LSB(sq, a1);
        EMIT_MOVE(moves, sq + 9, sq);
    }
    while (a2) {
        POP_LSB(sq, a2);
        EMIT_MOVE(moves, sq + 7, sq);
    }
    return moves - ptr;
}

int gen_black_knight_attacks(Board *board, Move *moves, bb mask) {
    return gen_knight_moves(
        moves, board->black_knights, mask);
}

int gen_black_bishop_attacks(Board *board, Move *moves, bb mask) {
    return gen_bishop_moves(
        moves, board->black_bishops, mask, board->all);
}

int gen_black_rook_attacks(Board *board, Move *moves, bb mask) {
    return gen_rook_moves(
        moves, board->black_rooks, mask, board->all);
}

int gen_black_queen_attacks(Board *board, Move *moves, bb mask) {
    return gen_queen_moves(
        moves, board->black_queens, mask, board->all);
}

int gen_black_king_attacks(Board *board, Move *moves, bb mask) {
    return gen_king_moves(
        moves, board->black_kings, mask);
}

int gen_black_attacks(Board *board, Move *moves, bb mask) {
    Move *ptr = moves;
    moves += gen_black_pawn_attacks(board, moves, mask);
    moves += gen_black_knight_attacks(board, moves, mask);
    moves += gen_black_bishop_attacks(board, moves, mask);
    moves += gen_black_rook_attacks(board, moves, mask);
    moves += gen_black_queen_attacks(board, moves, mask);
    moves += gen_black_king_attacks(board, moves, mask);
    return moves - ptr;
}

int gen_black_checks(Board *board, Move *moves) {
    return gen_black_attacks(board, moves, board->white_kings);
}

#include <string.h>
#include "gen.h"

#define EMIT_MOVE(m, a, b) \
    (m)->src = (a); \
    (m)->dst = (b); \
    (m)->promotion = EMPTY; \
    (m)++;

#define EMIT_PROMOTION(m, a, b, p) \
    (m)->src = (a); \
    (m)->dst = (b); \
    (m)->promotion = (p); \
    (m)++;

#define EMIT_PROMOTIONS(m, a, b) \
    EMIT_PROMOTION(m, a, b, QUEEN) \
    EMIT_PROMOTION(m, a, b, ROOK) \
    EMIT_PROMOTION(m, a, b, BISHOP) \
    EMIT_PROMOTION(m, a, b, KNIGHT)

// generic move generators
int gen_knight_moves(Move *moves, bb srcs, bb mask) {
    Move *ptr = moves;
    int src, dst;
    while (srcs) {
        POP_LSB(src, srcs);
        bb dsts = BB_KNIGHT[src] & mask;
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
        bb dsts = bb_bishop(src, all) & mask;
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
        bb dsts = bb_rook(src, all) & mask;
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
        bb dsts = bb_queen(src, all) & mask;
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
        bb dsts = BB_KING[src] & mask;
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
    bb mask = board->black | board->ep;
    bb promo = 0xff00000000000000L;
    bb p1 = (pawns << 8) & ~board->all;
    bb p2 = ((p1 & 0x0000000000ff0000L) << 8) & ~board->all;
    bb a1 = ((pawns & 0xfefefefefefefefeL) << 7) & mask;
    bb a2 = ((pawns & 0x7f7f7f7f7f7f7f7fL) << 9) & mask;
    int sq;
    while (p1) {
        POP_LSB(sq, p1);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq - 8, sq);
        }
        else {
            EMIT_MOVE(moves, sq - 8, sq);
        }
    }
    while (p2) {
        POP_LSB(sq, p2);
        EMIT_MOVE(moves, sq - 16, sq);
    }
    while (a1) {
        POP_LSB(sq, a1);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq - 7, sq);
        }
        else {
            EMIT_MOVE(moves, sq - 7, sq);
        }
    }
    while (a2) {
        POP_LSB(sq, a2);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq - 9, sq);
        }
        else {
            EMIT_MOVE(moves, sq - 9, sq);
        }
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

int gen_white_king_castles(Board *board, Move *moves) {
    Move *ptr = moves;
    if (board->castle & CASTLE_WHITE_KING) {
        if (!(board->all & 0x0000000000000060L)) {
            Move dummy[MAX_MOVES];
            bb mask = 0x0000000000000030L;
            if (!gen_black_attacks_against(board, dummy, mask)) {
                EMIT_MOVE(moves, 4, 6);
            }
        }
    }
    if (board->castle & CASTLE_WHITE_QUEEN) {
        if (!(board->all & 0x000000000000000eL)) {
            Move dummy[MAX_MOVES];
            bb mask = 0x0000000000000018L;
            if (!gen_black_attacks_against(board, dummy, mask)) {
                EMIT_MOVE(moves, 4, 2);
            }
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
    moves += gen_white_king_castles(board, moves);
    return moves - ptr;
}

// white attack generators
int gen_white_pawn_attacks_against(Board *board, Move *moves, bb mask) {
    Move *ptr = moves;
    bb pawns = board->white_pawns;
    bb a1 = ((pawns & 0xfefefefefefefefeL) << 7) & mask;
    bb a2 = ((pawns & 0x7f7f7f7f7f7f7f7fL) << 9) & mask;
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

int gen_white_knight_attacks_against(Board *board, Move *moves, bb mask) {
    return gen_knight_moves(
        moves, board->white_knights, mask);
}

int gen_white_bishop_attacks_against(Board *board, Move *moves, bb mask) {
    return gen_bishop_moves(
        moves, board->white_bishops, mask, board->all);
}

int gen_white_rook_attacks_against(Board *board, Move *moves, bb mask) {
    return gen_rook_moves(
        moves, board->white_rooks, mask, board->all);
}

int gen_white_queen_attacks_against(Board *board, Move *moves, bb mask) {
    return gen_queen_moves(
        moves, board->white_queens, mask, board->all);
}

int gen_white_king_attacks_against(Board *board, Move *moves, bb mask) {
    return gen_king_moves(
        moves, board->white_kings, mask);
}

int gen_white_attacks_against(Board *board, Move *moves, bb mask) {
    Move *ptr = moves;
    moves += gen_white_pawn_attacks_against(board, moves, mask);
    moves += gen_white_knight_attacks_against(board, moves, mask);
    moves += gen_white_bishop_attacks_against(board, moves, mask);
    moves += gen_white_rook_attacks_against(board, moves, mask);
    moves += gen_white_queen_attacks_against(board, moves, mask);
    moves += gen_white_king_attacks_against(board, moves, mask);
    return moves - ptr;
}

int gen_white_attacks(Board *board, Move *moves) {
    return gen_white_attacks_against(board, moves, board->black);
}

int gen_white_checks(Board *board, Move *moves) {
    return gen_white_attacks_against(board, moves, board->black_kings);
}

// black move generators
int gen_black_pawn_moves(Board *board, Move *moves) {
    Move *ptr = moves;
    bb pawns = board->black_pawns;
    bb mask = board->white | board->ep;
    bb promo = 0x00000000000000ffL;
    bb p1 = (pawns >> 8) & ~board->all;
    bb p2 = ((p1 & 0x0000ff0000000000L) >> 8) & ~board->all;
    bb a1 = ((pawns & 0x7f7f7f7f7f7f7f7fL) >> 7) & mask;
    bb a2 = ((pawns & 0xfefefefefefefefeL) >> 9) & mask;
    int sq;
    while (p1) {
        POP_LSB(sq, p1);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq + 8, sq);
        }
        else {
            EMIT_MOVE(moves, sq + 8, sq);
        }
    }
    while (p2) {
        POP_LSB(sq, p2);
        EMIT_MOVE(moves, sq + 16, sq);
    }
    while (a1) {
        POP_LSB(sq, a1);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq + 7, sq);
        }
        else {
            EMIT_MOVE(moves, sq + 7, sq);
        }
    }
    while (a2) {
        POP_LSB(sq, a2);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq + 9, sq);
        }
        else {
            EMIT_MOVE(moves, sq + 9, sq);
        }
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

int gen_black_king_castles(Board *board, Move *moves) {
    Move *ptr = moves;
    if (board->castle & CASTLE_BLACK_KING) {
        if (!(board->all & 0x6000000000000000L)) {
            Move dummy[MAX_MOVES];
            bb mask = 0x3000000000000000L;
            if (!gen_white_attacks_against(board, dummy, mask)) {
                EMIT_MOVE(moves, 60, 62);
            }
        }
    }
    if (board->castle & CASTLE_BLACK_QUEEN) {
        if (!(board->all & 0x0e00000000000000L)) {
            Move dummy[MAX_MOVES];
            bb mask = 0x1800000000000000L;
            if (!gen_white_attacks_against(board, dummy, mask)) {
                EMIT_MOVE(moves, 60, 58);
            }
        }
    }
    return moves - ptr;
}

int gen_black_moves(Board *board, Move *moves) {
    Move *ptr = moves;
    moves += gen_black_pawn_moves(board, moves);
    moves += gen_black_knight_moves(board, moves);
    moves += gen_black_bishop_moves(board, moves);
    moves += gen_black_rook_moves(board, moves);
    moves += gen_black_queen_moves(board, moves);
    moves += gen_black_king_moves(board, moves);
    moves += gen_black_king_castles(board, moves);
    return moves - ptr;
}

// black attack generators
int gen_black_pawn_attacks_against(Board *board, Move *moves, bb mask) {
    Move *ptr = moves;
    bb pawns = board->black_pawns;
    bb a1 = ((pawns & 0x7f7f7f7f7f7f7f7fL) >> 7) & mask;
    bb a2 = ((pawns & 0xfefefefefefefefeL) >> 9) & mask;
    int sq;
    while (a1) {
        POP_LSB(sq, a1);
        EMIT_MOVE(moves, sq + 7, sq);
    }
    while (a2) {
        POP_LSB(sq, a2);
        EMIT_MOVE(moves, sq + 9, sq);
    }
    return moves - ptr;
}

int gen_black_knight_attacks_against(Board *board, Move *moves, bb mask) {
    return gen_knight_moves(
        moves, board->black_knights, mask);
}

int gen_black_bishop_attacks_against(Board *board, Move *moves, bb mask) {
    return gen_bishop_moves(
        moves, board->black_bishops, mask, board->all);
}

int gen_black_rook_attacks_against(Board *board, Move *moves, bb mask) {
    return gen_rook_moves(
        moves, board->black_rooks, mask, board->all);
}

int gen_black_queen_attacks_against(Board *board, Move *moves, bb mask) {
    return gen_queen_moves(
        moves, board->black_queens, mask, board->all);
}

int gen_black_king_attacks_against(Board *board, Move *moves, bb mask) {
    return gen_king_moves(
        moves, board->black_kings, mask);
}

int gen_black_attacks_against(Board *board, Move *moves, bb mask) {
    Move *ptr = moves;
    moves += gen_black_pawn_attacks_against(board, moves, mask);
    moves += gen_black_knight_attacks_against(board, moves, mask);
    moves += gen_black_bishop_attacks_against(board, moves, mask);
    moves += gen_black_rook_attacks_against(board, moves, mask);
    moves += gen_black_queen_attacks_against(board, moves, mask);
    moves += gen_black_king_attacks_against(board, moves, mask);
    return moves - ptr;
}

int gen_black_attacks(Board *board, Move *moves) {
    return gen_black_attacks_against(board, moves, board->white);
}

int gen_black_checks(Board *board, Move *moves) {
    return gen_black_attacks_against(board, moves, board->white_kings);
}

// color determined by board
int gen_moves(Board *board, Move *moves) {
    if (board->color) {
        return gen_black_moves(board, moves);
    }
    else {
        return gen_white_moves(board, moves);
    }
}

int gen_legal_moves(Board *board, Move *moves) {
    Move *ptr = moves;
    Undo undo;
    Move temp[MAX_MOVES];
    int count = gen_moves(board, temp);
    for (int i = 0; i < count; i++) {
        Move *move = &temp[i];
        do_move(board, move, &undo);
        if (!is_illegal(board)) {
            memcpy(moves++, move, sizeof(Move));
        }
        undo_move(board, move, &undo);
    }
    return moves - ptr;
}

int gen_attacks(Board *board, Move *moves) {
    if (board->color) {
        return gen_black_attacks(board, moves);
    }
    else {
        return gen_white_attacks(board, moves);
    }
}

int gen_checks(Board *board, Move *moves) {
    if (board->color) {
        return gen_black_checks(board, moves);
    }
    else {
        return gen_white_checks(board, moves);
    }
}

int is_check(Board *board) {
    Move moves[MAX_MOVES];
    if (board->color) {
        return gen_white_checks(board, moves);
    }
    else {
        return gen_black_checks(board, moves);
    }
}

int is_illegal(Board *board) {
    Move moves[MAX_MOVES];
    if (board->color) {
        return gen_black_checks(board, moves);
    }
    else {
        return gen_white_checks(board, moves);
    }
}

int has_legal_moves(Board *board) {
    Move moves[MAX_MOVES];
    return gen_legal_moves(board, moves);
}

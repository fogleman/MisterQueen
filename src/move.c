#include <stdio.h>
#include <string.h>
#include "gen.h"
#include "move.h"

#define TOGGLE_HASH(board) \
    board->hash ^= HASH_CASTLE[board->castle]; \
    if (board->ep) { \
        board->hash ^= HASH_EP[LSB(board->ep) % 8]; \
    }

void do_move(Board *board, Move *move, Undo *undo) {
    TOGGLE_HASH(board);
    undo->piece = board->squares[move->src];
    undo->capture = board->squares[move->dst];
    undo->castle = board->castle;
    undo->ep = board->ep;
    board_set(board, move->src, EMPTY);
    if (move->promotion) {
        board_set(board, move->dst, move->promotion);
    }
    else {
        board_set(board, move->dst, undo->piece);
    }
    board->ep = 0L;
    if (undo->piece == WHITE_PAWN) {
        bb src = BIT(move->src);
        bb dst = BIT(move->dst);
        if ((src & 0x000000000000ff00L) && (dst & 0x00000000ff000000L)) {
            board->ep = BIT(move->src + 8);
        }
        if (dst == undo->ep) {
            board_set(board, move->dst - 8, EMPTY);
        }
    }
    else if (undo->piece == BLACK_PAWN) {
        bb src = BIT(move->src);
        bb dst = BIT(move->dst);
        if ((src & 0x00ff000000000000L) && (dst & 0x000000ff00000000L)) {
            board->ep = BIT(move->src - 8);
        }
        if (dst == undo->ep) {
            board_set(board, move->dst + 8, EMPTY);
        }
    }
    else if (undo->piece == WHITE_KING) {
        board->castle &= ~CASTLE_WHITE;
        if (move->src == 4 && move->dst == 6) {
            board_set(board, 7, EMPTY);
            board_set(board, 5, WHITE_ROOK);
        }
        else if (move->src == 4 && move->dst == 2) {
            board_set(board, 0, EMPTY);
            board_set(board, 3, WHITE_ROOK);
        }
    }
    else if (undo->piece == BLACK_KING) {
        board->castle &= ~CASTLE_BLACK;
        if (move->src == 60 && move->dst == 62) {
            board_set(board, 63, EMPTY);
            board_set(board, 61, BLACK_ROOK);
        }
        else if (move->src == 60 && move->dst == 58) {
            board_set(board, 56, EMPTY);
            board_set(board, 59, BLACK_ROOK);
        }
    }
    if (move->src == 0 || move->dst == 0) {
        board->castle &= ~CASTLE_WHITE_QUEEN;
    }
    if (move->src == 7 || move->dst == 7) {
        board->castle &= ~CASTLE_WHITE_KING;
    }
    if (move->src == 56 || move->dst == 56) {
        board->castle &= ~CASTLE_BLACK_QUEEN;
    }
    if (move->src == 63 || move->dst == 63) {
        board->castle &= ~CASTLE_BLACK_KING;
    }
    board->color ^= BLACK;
    board->hash ^= HASH_COLOR;
    TOGGLE_HASH(board);
}

void undo_move(Board *board, Move *move, Undo *undo) {
    TOGGLE_HASH(board);
    board_set(board, move->src, undo->piece);
    board_set(board, move->dst, undo->capture);
    board->castle = undo->castle;
    board->ep = undo->ep;
    if (undo->piece == WHITE_PAWN) {
        if (BIT(move->dst) == undo->ep) {
            board_set(board, move->dst - 8, BLACK_PAWN);
        }
    }
    else if (undo->piece == BLACK_PAWN) {
        if (BIT(move->dst) == undo->ep) {
            board_set(board, move->dst + 8, WHITE_PAWN);
        }
    }
    else if (undo->piece == WHITE_KING) {
        if (move->src == 4 && move->dst == 6) {
            board_set(board, 7, WHITE_ROOK);
            board_set(board, 5, EMPTY);
        }
        else if (move->src == 4 && move->dst == 2) {
            board_set(board, 0, WHITE_ROOK);
            board_set(board, 3, EMPTY);
        }
    }
    else if (undo->piece == BLACK_KING) {
        if (move->src == 60 && move->dst == 62) {
            board_set(board, 63, BLACK_ROOK);
            board_set(board, 61, EMPTY);
        }
        else if (move->src == 60 && move->dst == 58) {
            board_set(board, 56, BLACK_ROOK);
            board_set(board, 59, EMPTY);
        }
    }
    board->color ^= BLACK;
    board->hash ^= HASH_COLOR;
    TOGGLE_HASH(board);
}

void notate_move(Board *board, Move *move, Move *moves, int count, char *result) {
    int piece = board->squares[move->src];
    int capture = board->squares[move->dst];
    char rank1 = '1' + move->src / 8;
    char file1 = 'a' + move->src % 8;
    char rank2 = '1' + move->dst / 8;
    char file2 = 'a' + move->dst % 8;
    int show_rank1 = 0;
    int show_file1 = 0;
    if (capture && PIECE(piece) == PAWN) {
        show_file1 = 1;
    }
    // castle
    if (PIECE(piece) == KING) {
        if (move->src == 4 && move->dst == 6) {
            strcpy(result, "O-O");
            return;
        }
        if (move->src == 4 && move->dst == 2) {
            strcpy(result, "O-O-O");
            return;
        }
        if (move->src == 60 && move->dst == 62) {
            strcpy(result, "O-O");
            return;
        }
        if (move->src == 60 && move->dst == 58) {
            strcpy(result, "O-O-O");
            return;
        }
    }
    // ambiguity
    for (int i = 0; i < count; i++) {
        Move *other = moves + i;
        if (memcmp(move, other, sizeof(Move)) == 0) {
            continue; // same move
        }
        if (move->dst != other->dst) {
            continue; // different target
        }
        if (piece != board->squares[other->src]) {
            continue; // different piece
        }
        if (move->src % 8 != other->src % 8) {
            show_file1 = 1;
        }
        else if (move->src / 8 != other->src / 8) {
            show_rank1 = 1;
        }
        else {
            show_file1 = 1;
            show_rank1 = 1;
        }
    }
    // piece
    switch (PIECE(piece)) {
        case KNIGHT: *result++ = 'N'; break;
        case BISHOP: *result++ = 'B'; break;
        case ROOK:   *result++ = 'R'; break;
        case QUEEN:  *result++ = 'Q'; break;
        case KING:   *result++ = 'K'; break;
    }
    // source
    if (show_file1) {
        *result++ = file1;
    }
    if (show_rank1) {
        *result++ = rank1;
    }
    // capture
    if (capture) {
        *result++ = 'x';
    }
    // target
    *result++ = file2;
    *result++ = rank2;
    // promotion
    if (move->promotion) {
        *result++ = '=';
        switch (PIECE(move->promotion)) {
            case KNIGHT: *result++ = 'N'; break;
            case BISHOP: *result++ = 'B'; break;
            case ROOK:   *result++ = 'R'; break;
            case QUEEN:  *result++ = 'Q'; break;
        }
    }
    // check
    Undo undo;
    do_move(board, move, &undo);
    if (is_check(board)) {
        if (has_legal_moves(board)) {
            *result++ = '+';
        }
        else {
            *result++ = '#';
        }
    }
    undo_move(board, move, &undo);
    // null terminator
    *result++ = 0;
}

void print_move(Board *board, Move *move) {
    char notation[16];
    Move moves[MAX_MOVES];
    int count = gen_legal_moves(board, moves);
    notate_move(board, move, moves, count, notation);
    printf("%s\n", notation);
}

int parse_move(Board *board, char *notation, Move *move) {
    char temp[16];
    Move moves[MAX_MOVES];
    int count = gen_legal_moves(board, moves);
    for (int i = 0; i < count; i++) {
        notate_move(board, &moves[i], moves, count, temp);
        if (strcmp(notation, temp) == 0) {
            memcpy(move, &moves[i], sizeof(Move));
            return 1;
        }
    }
    return 0;
}

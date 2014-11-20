#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gen.h"
#include "move.h"
#include "util.h"

#define TOGGLE_HASH(board) \
    board->hash ^= HASH_CASTLE[board->castle]; \
    board->pawn_hash ^= HASH_CASTLE[board->castle]; \
    if (board->ep) { \
        board->hash ^= HASH_EP[LSB(board->ep) % 8]; \
        board->pawn_hash ^= HASH_EP[LSB(board->ep) % 8]; \
    }

void make_move(Board *board, Move *move) {
    Undo undo; // throw-away
    do_move(board, move, &undo);
}

void do_null_move(Board *board, Undo *undo) {
    TOGGLE_HASH(board);
    undo->ep = board->ep;
    board->ep = 0L;
    board->color ^= BLACK;
    board->hash ^= HASH_COLOR;
    board->pawn_hash ^= HASH_COLOR;
    TOGGLE_HASH(board);
}

void undo_null_move(Board *board, Undo *undo) {
    TOGGLE_HASH(board);
    board->ep = undo->ep;
    board->color ^= BLACK;
    board->hash ^= HASH_COLOR;
    board->pawn_hash ^= HASH_COLOR;
    TOGGLE_HASH(board);
}

void do_move(Board *board, Move *move, Undo *undo) {
    TOGGLE_HASH(board);
    undo->piece = board->squares[move->src];
    undo->capture = board->squares[move->dst];
    undo->castle = board->castle;
    undo->ep = board->ep;
    board_set(board, move->src, EMPTY);
    if (move->promotion) {
        board_set(board, move->dst, move->promotion | board->color);
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
    board->pawn_hash ^= HASH_COLOR;
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
    board->pawn_hash ^= HASH_COLOR;
    TOGGLE_HASH(board);
}

int score_move(Board *board, Move *move) {
    int result = 0;
    int src = move->src;
    int dst = move->dst;
    int piece = board->squares[src];
    int capture = board->squares[dst];
    int piece_material = 0;
    int capture_material = 0;
    if (COLOR(piece)) {
        switch (PIECE(piece)) {
            case PAWN:
                piece_material = MATERIAL_PAWN;
                result -= POSITION_BLACK_PAWN[src];
                result += POSITION_BLACK_PAWN[dst];
                break;
            case KNIGHT:
                piece_material = MATERIAL_KNIGHT;
                result -= POSITION_BLACK_KNIGHT[src];
                result += POSITION_BLACK_KNIGHT[dst];
                break;
            case BISHOP:
                piece_material = MATERIAL_BISHOP;
                result -= POSITION_BLACK_BISHOP[src];
                result += POSITION_BLACK_BISHOP[dst];
                break;
            case ROOK:
                piece_material = MATERIAL_ROOK;
                result -= POSITION_BLACK_ROOK[src];
                result += POSITION_BLACK_ROOK[dst];
                break;
            case QUEEN:
                piece_material = MATERIAL_QUEEN;
                result -= POSITION_BLACK_QUEEN[src];
                result += POSITION_BLACK_QUEEN[dst];
                break;
            case KING:
                piece_material = MATERIAL_KING;
                result -= POSITION_BLACK_KING[src];
                result += POSITION_BLACK_KING[dst];
                break;
        }
    }
    else {
        switch (PIECE(piece)) {
            case PAWN:
                piece_material = MATERIAL_PAWN;
                result -= POSITION_WHITE_PAWN[src];
                result += POSITION_WHITE_PAWN[dst];
                break;
            case KNIGHT:
                piece_material = MATERIAL_KNIGHT;
                result -= POSITION_WHITE_KNIGHT[src];
                result += POSITION_WHITE_KNIGHT[dst];
                break;
            case BISHOP:
                piece_material = MATERIAL_BISHOP;
                result -= POSITION_WHITE_BISHOP[src];
                result += POSITION_WHITE_BISHOP[dst];
                break;
            case ROOK:
                piece_material = MATERIAL_ROOK;
                result -= POSITION_WHITE_ROOK[src];
                result += POSITION_WHITE_ROOK[dst];
                break;
            case QUEEN:
                piece_material = MATERIAL_QUEEN;
                result -= POSITION_WHITE_QUEEN[src];
                result += POSITION_WHITE_QUEEN[dst];
                break;
            case KING:
                piece_material = MATERIAL_KING;
                result -= POSITION_WHITE_KING[src];
                result += POSITION_WHITE_KING[dst];
                break;
        }
    }
    if (capture) {
        if (COLOR(capture)) {
            switch (PIECE(capture)) {
                case PAWN:
                    capture_material = MATERIAL_PAWN;
                    result += POSITION_BLACK_PAWN[dst];
                    break;
                case KNIGHT:
                    capture_material = MATERIAL_KNIGHT;
                    result += POSITION_BLACK_KNIGHT[dst];
                    break;
                case BISHOP:
                    capture_material = MATERIAL_BISHOP;
                    result += POSITION_BLACK_BISHOP[dst];
                    break;
                case ROOK:
                    capture_material = MATERIAL_ROOK;
                    result += POSITION_BLACK_ROOK[dst];
                    break;
                case QUEEN:
                    capture_material = MATERIAL_QUEEN;
                    result += POSITION_BLACK_QUEEN[dst];
                    break;
                case KING:
                    capture_material = MATERIAL_KING;
                    result += POSITION_BLACK_KING[dst];
                    break;
            }
        }
        else {
            switch (PIECE(capture)) {
                case PAWN:
                    capture_material = MATERIAL_PAWN;
                    result += POSITION_WHITE_PAWN[dst];
                    break;
                case KNIGHT:
                    capture_material = MATERIAL_KNIGHT;
                    result += POSITION_WHITE_KNIGHT[dst];
                    break;
                case BISHOP:
                    capture_material = MATERIAL_BISHOP;
                    result += POSITION_WHITE_BISHOP[dst];
                    break;
                case ROOK:
                    capture_material = MATERIAL_ROOK;
                    result += POSITION_WHITE_ROOK[dst];
                    break;
                case QUEEN:
                    capture_material = MATERIAL_QUEEN;
                    result += POSITION_WHITE_QUEEN[dst];
                    break;
                case KING:
                    capture_material = MATERIAL_KING;
                    result += POSITION_WHITE_KING[dst];
                    break;
            }
        }
        result += capture_material;
    }
    return result;
}

void move_to_string(Move *move, char *str) {
    char rank1 = '1' + move->src / 8;
    char file1 = 'a' + move->src % 8;
    char rank2 = '1' + move->dst / 8;
    char file2 = 'a' + move->dst % 8;
    *str++ = file1;
    *str++ = rank1;
    *str++ = file2;
    *str++ = rank2;
    if (move->promotion) {
        switch (move->promotion) {
            case KNIGHT: *str++ = 'n'; break;
            case BISHOP: *str++ = 'b'; break;
            case ROOK:   *str++ = 'r'; break;
            case QUEEN:  *str++ = 'q'; break;
        }
    }
    *str++ = 0;
}

void move_from_string(Move *move, const char *str) {
    int file1 = str[0] - 'a';
    int rank1 = str[1] - '1';
    int file2 = str[2] - 'a';
    int rank2 = str[3] - '1';
    int src = rank1 * 8 + file1;
    int dst = rank2 * 8 + file2;
    int promotion = EMPTY;
    switch (str[4]) {
        case 'n': promotion = KNIGHT; break;
        case 'b': promotion = BISHOP; break;
        case 'r': promotion = ROOK; break;
        case 'q': promotion = QUEEN; break;
    }
    move->src = src;
    move->dst = dst;
    move->promotion = promotion;
}

void notate_move(Board *board, Move *move, char *result) {
    Move moves[MAX_MOVES];
    int count = gen_legal_moves(board, moves);
    int piece = board->squares[move->src];
    int capture = board->squares[move->dst];
    char rank1 = '1' + move->src / 8;
    char file1 = 'a' + move->src % 8;
    char rank2 = '1' + move->dst / 8;
    char file2 = 'a' + move->dst % 8;
    int show_rank1 = 0;
    int show_file1 = 0;
    if (PIECE(piece) == PAWN) {
        if (file1 != file2) {
            capture = 1;
        }
        if (capture) {
            show_file1 = 1;
        }
    }
    // ambiguity
    int ambiguous = 0;
    int unique_rank = 1;
    int unique_file = 1;
    for (int i = 0; i < count; i++) {
        Move *other = moves + i;
        if (move->dst != other->dst) {
            continue; // different target
        }
        if (move->src == other->src) {
            continue; // same move
        }
        if (piece != board->squares[other->src]) {
            continue; // different piece
        }
        ambiguous = 1;
        if (move->src % 8 == other->src % 8) {
            unique_file = 0;
        }
        if (move->src / 8 == other->src / 8) {
            unique_rank = 0;
        }
    }
    if (ambiguous) {
        if (unique_rank && unique_file) {
            show_file1 = 1;
        }
        else if (unique_rank) {
            show_rank1 = 1;
        }
        else if (unique_file) {
            show_file1 = 1;
        }
        else {
            show_rank1 = 1;
            show_file1 = 1;
        }
    }
    // castle
    int castle = 0;
    if (PIECE(piece) == KING) {
        castle = 1;
        if (move->src == 4 && move->dst == 6) {
            strcpy(result, "O-O");
            result += 3;
        }
        else if (move->src == 4 && move->dst == 2) {
            strcpy(result, "O-O-O");
            result += 5;
        }
        else if (move->src == 60 && move->dst == 62) {
            strcpy(result, "O-O");
            result += 3;
        }
        else if (move->src == 60 && move->dst == 58) {
            strcpy(result, "O-O-O");
            result += 5;
        }
        else {
            castle = 0;
        }
    }
    if (!castle) {
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
            switch (move->promotion) {
                case KNIGHT: *result++ = 'N'; break;
                case BISHOP: *result++ = 'B'; break;
                case ROOK:   *result++ = 'R'; break;
                case QUEEN:  *result++ = 'Q'; break;
            }
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
    notate_move(board, move, notation);
    printf("%s", notation);
}

int parse_move(Board *board, const char *notation, Move *move) {
    char temp[16];
    Move moves[MAX_MOVES];
    int count = gen_legal_moves(board, moves);
    for (int i = 0; i < count; i++) {
        notate_move(board, &moves[i], temp);
        if (strcmp(notation, temp) == 0) {
            memcpy(move, &moves[i], sizeof(Move));
            return 1;
        }
    }
    return 0;
}

int parse_pgn(Board *board, const char *pgn) {
    board_reset(board);
    char *temp = calloc(strlen(pgn) + 1, sizeof(char));
    strcpy(temp, pgn);
    char *key;
    char *token = tokenize(temp, " ", &key);
    while (token) {
        Move move;
        if (parse_move(board, token, &move)) {
            make_move(board, &move);
        }
        else {
            return 0;
        }
        token = tokenize(NULL, " ", &key);
    }
    free(temp);
    return 1;
}

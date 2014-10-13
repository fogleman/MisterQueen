#include "move.h"

void do_move(Board *board, Move *move, Undo *undo) {
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
    board->ep = -1;
    if (undo->piece == WHITE_PAWN) {
        bb src = BIT(move->src);
        bb dst = BIT(move->dst);
        if ((src & 0x000000000000ff00L) && (dst & 0x00000000ff000000L)) {
            board->ep = move->src + 8;
        }
        if (move->dst == undo->ep) {
            board_set(board, move->dst - 8, EMPTY);
        }
    }
    else if (undo->piece == BLACK_PAWN) {
        bb src = BIT(move->src);
        bb dst = BIT(move->dst);
        if ((src & 0x00ff000000000000L) && (dst & 0x000000ff00000000L)) {
            board->ep = move->src - 8;
        }
        if (move->dst == undo->ep) {
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
}

void undo_move(Board *board, Move *move, Undo *undo) {
    board_set(board, move->src, undo->piece);
    board_set(board, move->dst, undo->capture);
    board->castle = undo->castle;
    board->ep = undo->ep;
    if (undo->piece == WHITE_PAWN) {
        if (move->dst == undo->ep) {
            board_set(board, move->dst - 8, BLACK_PAWN);
        }
    }
    else if (undo->piece == BLACK_PAWN) {
        if (move->dst == undo->ep) {
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
}

void move_notation(Board *board, Move *move, char *result) {
    int piece = board->squares[move->src];
    int capture = board->squares[move->dst] != EMPTY;
    // char rank1 = '1' + move->src / 8;
    char file1 = 'a' + move->src % 8;
    char rank2 = '1' + move->dst / 8;
    char file2 = 'a' + move->dst % 8;
    switch (PIECE(piece)) {
        case PAWN:
            if (capture) {
                *result++ = file1;
            }
            break;
        case KNIGHT: *result++ = 'N'; break;
        case BISHOP: *result++ = 'B'; break;
        case ROOK:   *result++ = 'R'; break;
        case QUEEN:  *result++ = 'Q'; break;
        case KING:   *result++ = 'K'; break;
    }
    if (capture) {
        *result++ = 'x';
    }
    *result++ = file2;
    *result++ = rank2;
    if (move->promotion) {
        *result++ = '=';
        switch (PIECE(move->promotion)) {
            case KNIGHT: *result++ = 'N'; break;
            case BISHOP: *result++ = 'B'; break;
            case ROOK:   *result++ = 'R'; break;
            case QUEEN:  *result++ = 'Q'; break;
        }
    }
    *result++ = 0;
}

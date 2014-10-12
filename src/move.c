#include "move.h"

void do_move(Board *board, Move *move, Undo *undo) {
    undo->piece = board->squares[move->src];
    undo->capture = board->squares[move->dst];
    undo->castle = board->castle;
    undo->ep = board->ep;
    board_set(board, move->src, EMPTY);
    board_set(board, move->dst, undo->piece);
    board->ep = 0;
    if (undo->piece == (WHITE | PAWN)) {
        bb src = BIT(move->src);
        bb dst = BIT(move->dst);
        if ((src & 0x000000000000ff00L) && (dst & 0x00000000ff000000L)) {
            board->ep = move->src + 8;
        }
        if (move->dst == undo->ep) {
            board_set(board, move->dst - 8, EMPTY);
        }
    }
    else if (undo->piece == (BLACK | PAWN)) {
        bb src = BIT(move->src);
        bb dst = BIT(move->dst);
        if ((src & 0x00ff000000000000L) && (dst & 0x000000ff00000000L)) {
            board->ep = move->src - 8;
        }
        if (move->dst == undo->ep) {
            board_set(board, move->dst + 8, EMPTY);
        }
    }
    board->color ^= BLACK;
}

void undo_move(Board *board, Move *move, Undo *undo) {
    board_set(board, move->src, undo->piece);
    board_set(board, move->dst, undo->capture);
    board->castle = undo->castle;
    board->ep = undo->ep;
    if (undo->piece == (WHITE | PAWN)) {
        if (move->dst == undo->ep) {
            board_set(board, move->dst - 8, BLACK | PAWN);
        }
    }
    else if (undo->piece == (BLACK | PAWN)) {
        if (move->dst == undo->ep) {
            board_set(board, move->dst + 8, WHITE | PAWN);
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
    *result++ = 0;
}

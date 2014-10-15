#ifndef GEN_H
#define GEN_H

#include "bb.h"
#include "board.h"
#include "move.h"

int gen_knight_moves(Move *moves, bb srcs, bb mask);
int gen_bishop_moves(Move *moves, bb srcs, bb mask, bb all);
int gen_rook_moves(Move *moves, bb srcs, bb mask, bb all);
int gen_queen_moves(Move *moves, bb srcs, bb mask, bb all);
int gen_king_moves(Move *moves, bb srcs, bb mask);

int gen_white_pawn_moves(Board *board, Move *moves);
int gen_white_knight_moves(Board *board, Move *moves);
int gen_white_bishop_moves(Board *board, Move *moves);
int gen_white_rook_moves(Board *board, Move *moves);
int gen_white_queen_moves(Board *board, Move *moves);
int gen_white_king_moves(Board *board, Move *moves);
int gen_white_moves(Board *board, Move *moves);

int gen_white_pawn_attacks_against(Board *board, Move *moves, bb mask);
int gen_white_knight_attacks_against(Board *board, Move *moves, bb mask);
int gen_white_bishop_attacks_against(Board *board, Move *moves, bb mask);
int gen_white_rook_attacks_against(Board *board, Move *moves, bb mask);
int gen_white_queen_attacks_against(Board *board, Move *moves, bb mask);
int gen_white_king_attacks_against(Board *board, Move *moves, bb mask);
int gen_white_attacks_against(Board *board, Move *moves, bb mask);
int gen_white_attacks(Board *board, Move *moves);
int gen_white_checks(Board *board, Move *moves);

int gen_black_pawn_moves(Board *board, Move *moves);
int gen_black_knight_moves(Board *board, Move *moves);
int gen_black_bishop_moves(Board *board, Move *moves);
int gen_black_rook_moves(Board *board, Move *moves);
int gen_black_queen_moves(Board *board, Move *moves);
int gen_black_king_moves(Board *board, Move *moves);
int gen_black_moves(Board *board, Move *moves);

int gen_black_pawn_attacks_against(Board *board, Move *moves, bb mask);
int gen_black_knight_attacks_against(Board *board, Move *moves, bb mask);
int gen_black_bishop_attacks_against(Board *board, Move *moves, bb mask);
int gen_black_rook_attacks_against(Board *board, Move *moves, bb mask);
int gen_black_queen_attacks_against(Board *board, Move *moves, bb mask);
int gen_black_king_attacks_against(Board *board, Move *moves, bb mask);
int gen_black_attacks_against(Board *board, Move *moves, bb mask);
int gen_black_attacks(Board *board, Move *moves);
int gen_black_checks(Board *board, Move *moves);

int gen_moves(Board *board, Move *moves);
int gen_legal_moves(Board *board, Move *moves);
int gen_attacks(Board *board, Move *moves);
int gen_checks(Board *board, Move *moves);
int is_check(Board *board);
int is_illegal(Board *board);
int has_legal_moves(Board *board);

#endif

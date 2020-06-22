#include "../../src/board.h"
#include "../../src/move.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define string char *

void print_commands(void);
int play_round(int currentPlayer, Board *board);
void lower(char **s);
int handle_player_choice(Board *board, string s);
void process_move(Board *board, string s);
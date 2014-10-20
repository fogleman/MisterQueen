#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "move.h"
#include "search.h"
#include "uci.h"
#include "util.h"

static Board board;

char *strip(char *str) {
    while (isspace(*str)) {
        str++;
    }
    if (*str == 0) {
        return str;
    }
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end--;
    }
    *(end + 1) = 0;
    return str;
}

void handle_uci() {
    printf("id name Chess\n");
    printf("id author Michael Fogleman\n");
    printf("uciok\n");
}

void handle_startpos() {
    board_reset(&board);
}

void handle_moves(char *moves) {
    Move move;
    board_reset(&board);
    char *key;
    char *token = tokenize(moves, " ", &key);
    while (token) {
        move_from_string(&move, token);
        make_move(&board, &move);
        token = tokenize(NULL, " ", &key);
    }
}

void handle_go() {
    Move move;
    search(&board, 100, &move);
}

void parse_line() {
    char data[1024];
    if (fgets(data, 1024, stdin) == NULL) {
        return;
    }
    //position startpos moves e2e4 e7e5
    char moves[1024];
    char *line = strip(data);
    if (strcmp(line, "uci") == 0) {
        handle_uci();
    }
    if (strcmp(line, "position startpos") == 0) {
        handle_startpos();
    }
    if (sscanf(line, "position startpos moves %[^]", moves) == 1) {
        handle_moves(moves);
    }
    if (strcmp(line, "go infinite") == 0) {
        handle_go();
    }
}

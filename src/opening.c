#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "move.h"
#include "opening.h"
#include "util.h"

Node *node_alloc(const char *move) {
    Node *node = calloc(1, sizeof(Node));
    strcpy(node->move, move);
    return node;
}

void node_free(Node *node) {
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i]) {
            node_free(node->children[i]);
        }
    }
    free(node);
}

Node *node_move(Node *node, char *move) {
    int index = -1;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        Node *child = node->children[i];
        if (child) {
            if (strcmp(child->move, move) == 0) {
                return child;
            }
        }
        else {
            index = i;
            break;
        }
    }
    if (index < 0) {
        return NULL;
    }
    Node *child = node_alloc(move);
    node->children[index] = child;
    return child;
}

void node_result(Node *node, int result) {
    node->total++;
    if (result > 0) {
        node->white++;
    }
    else if (result < 0) {
        node->black++;
    }
    else {
        node->draw++;
    }
}

void node_print(Node *node) {
    printf("%d, %d, %d, %d\n",
        node->white, node->draw, node->black, node->total);
    int total = 0;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        Node *child = node->children[i];
        if (child) {
            total += child->total;
        }
    }
    for (int i = 0; i < MAX_CHILDREN; i++) {
        Node *child = node->children[i];
        if (child) {
            int white = 100 * child->white / child->total;
            int draw = 100 * child->draw / child->total;
            int black = 100 * child->black / child->total;
            int pct = 100 * child->total / total;
            int total = child->total;
            printf("%8s [%3d%% %3d%% %3d%%] %3d%% %d\n",
                child->move, white, draw, black, pct, total);
        }
    }
}

void node_visit(Node *node, Board *board, int depth) {
    int total = 0;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        Node *child = node->children[i];
        if (child) {
            total += child->total;
        }
    }
    if (total < 1000) {
        return;
    }
    for (int i = 0; i < MAX_CHILDREN; i++) {
        Node *child = node->children[i];
        if (!child) {
            continue;
        }
        Move move;
        Undo undo;
        if (!parse_move(board, child->move, &move)) {
            continue;
        }
        printf("(0x%016llx, %2d, %2d, %d),\n",
            board->hash, move.src, move.dst, child->total);
        do_move(board, &move, &undo);
        node_visit(child, board, depth + 1);
        undo_move(board, &move, &undo);
    }
}

void handle_line(Node *root, char *line) {
    line = strip(line);
    int result = 0;
    switch (line[0]) {
        case 'W': result = 1; break;
        case 'B': result = -1; break;
    }
    Node *node = root;
    node_result(node, result);
    char *key;
    char *token = tokenize(line + 2, " ", &key);
    int depth = 0;
    while (token) {
        node = node_move(node, token);
        if (!node) {
            break;
        }
        node_result(node, result);
        token = tokenize(NULL, " ", &key);
        depth++;
        if (depth == 30) {
            break;
        }
    }
}

int opening_main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: main <input pgn file>\n");
        return 0;
    }
    Node *root = node_alloc("");
    char line[65536];
    FILE *file = fopen(argv[1], "r");
    while (fgets(line, sizeof(line), file)) {
        handle_line(root, line);
    }
    Board board;
    board_reset(&board);
    node_visit(root, &board, 0);
    fclose(file);
    // node_free(root);
    return 0;
}

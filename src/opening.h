#ifndef OPENING_H
#define OPENING_H

#define MAX_CHILDREN 32

struct Node {
    char move[8];
    int white;
    int black;
    int draw;
    int total;
    struct Node *children[MAX_CHILDREN];
};

typedef struct Node Node;

int opening_main(int argc, char **argv);

#endif

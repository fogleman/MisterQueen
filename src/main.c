#include "bb.h"
#include "perft.h"

int main(int argc, char **argv) {
    bb_init();
    perft_tests();
    return 0;
}

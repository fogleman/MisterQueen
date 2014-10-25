#include <time.h>
#include "bb.h"
#include "uci.h"
#include "util.h"

int main(int argc, char **argv) {
    bb_init();
    prng_seed(time(NULL));
    uci_main();
    return 0;
}

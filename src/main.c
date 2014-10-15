#include "bb.h"
#include "board.h"
#include "move.h"
#include "perft.h"
#include "search.h"

int main(int argc, char **argv) {
    bb_init();
    // perft_tests();
    Board board;
    Move move;
    Undo undo;
    board_reset(&board);
    // board_load_fen(&board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
    parse_pgn(&board, "d4 Nf6 c4 g6 Nc3 d5 Nf3 Bg7 Qb3 dxc4 Qxc4 O-O e4 a6 e5 b5 Qb3 Nfd7 e6 fxe6 h4 c5 Qxe6+ Kh8 Ng5 Ne5 Qd5 Qxd5 Nxd5 cxd4 h5 Bf5 hxg6 Bxg6 Nxe7 Bc2 f4 Nbc6 Nxc6 Nxc6 Kd2 Bf5 Bd3 h6 Bxf5 Rxf5 Kd1 Re8 Bd2 Kg8 Nf3 d3 g4 Rc5 Rc1 Rxc1+ Kxc1 Nb4 a3 Nd5 f5 Ne3 Re1 Rc8+ Kb1 Nxg4 Re6 Nf6 Bc3 Rf8 Nh4 Nd5 Bxg7 Kxg7 Rxa6 Rc8 Rd6 Ne3 f6+ Kf7 Rxd3 Nc4 Nf5 Kxf6 Nxh6 Rh8 Ng4+ Kg5 Nf2 Rh2 Ne4+ Kf4 Nc3 Rxb2+ Ka1 Rb3 Nd5+ Ke4 Rxb3 Kxd5 Rxb5+ Kd4 Rb3 Nd2 Rb4+ Kc3");
    while (0) {
        board_print(&board);
        search(&board, &move);
        do_move(&board, &move, &undo);
    }
    return 0;
}

#ifndef BB_H
#define BB_H

#define SQ(sq) (1L << (sq))
#define RF(rank, file) ((rank) * 8 + (file))

#define LSB(x) (__builtin_ctzll(x))
#define MSB(x) (__builtin_clzll(x))
// #define BITS(x) (__builtin_popcountll(x))

#define POP_LSB(b, x) b = LSB(x); x &= ~SQ(b);
#define POP_MSB(b, x) b = MSB(x); x &= ~SQ(b);

typedef unsigned long long bb;

extern bb SQ_KNIGHT[64];
extern bb SQ_KING[64];

extern bb SQ_BISHOP_6[64];
extern bb SQ_ROOK_6[64];

extern const bb MAGIC_BISHOP[64];
extern const bb MAGIC_ROOK[64];

extern const int SHIFT_BISHOP[64];
extern const int SHIFT_ROOK[64];

extern int OFFSET_BISHOP[64];
extern int OFFSET_ROOK[64];

extern bb ATTACK_BISHOP[5248];
extern bb ATTACK_ROOK[102400];

void bb_init();
void bb_print(bb value);

bb sq_bishop(int sq, bb obstacles);
bb sq_rook(int sq, bb obstacles);
bb sq_queen(int sq, bb obstacles);

#endif

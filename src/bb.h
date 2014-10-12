#ifndef BB_H
#define BB_H

#define BIT(sq) (1L << (sq))
#define RF(rank, file) ((rank) * 8 + (file))

#define LSB(x) (__builtin_ctzll(x))
#define MSB(x) (__builtin_clzll(x))
// #define BITS(x) (__builtin_popcountll(x))

#define POP_LSB(b, x) b = LSB(x); x &= ~BIT(b);
#define POP_MSB(b, x) b = MSB(x); x &= ~BIT(b);

typedef unsigned long long bb;

extern bb BB_KNIGHT[64];
extern bb BB_KING[64];

extern bb BB_BISHOP_6[64];
extern bb BB_ROOK_6[64];

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

bb bb_bishop(int sq, bb obstacles);
bb bb_rook(int sq, bb obstacles);
bb bb_queen(int sq, bb obstacles);

#endif

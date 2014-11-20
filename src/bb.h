#ifndef BB_H
#define BB_H

#define BIT(sq) (1L << (sq))
#define RF(rank, file) ((rank) * 8 + (file))

#define LSB(x) (__builtin_ctzll(x))
#define MSB(x) (__builtin_clzll(x))
#define BITS(x) (__builtin_popcountll(x))

#define POP_LSB(b, x) b = LSB(x); x &= ~BIT(b);
#define POP_MSB(b, x) b = MSB(x); x &= ~BIT(b);

#define RANK_1 0x00000000000000ffL
#define RANK_2 0x000000000000ff00L
#define RANK_3 0x0000000000ff0000L
#define RANK_4 0x00000000ff000000L
#define RANK_5 0x000000ff00000000L
#define RANK_6 0x0000ff0000000000L
#define RANK_7 0x00ff000000000000L
#define RANK_8 0xff00000000000000L

#define FILE_A 0x0101010101010101L
#define FILE_B 0x0202020202020202L
#define FILE_C 0x0404040404040404L
#define FILE_D 0x0808080808080808L
#define FILE_E 0x1010101010101010L
#define FILE_F 0x2020202020202020L
#define FILE_G 0x4040404040404040L
#define FILE_H 0x8080808080808080L

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

extern bb HASH_WHITE_PAWN[64];
extern bb HASH_BLACK_PAWN[64];
extern bb HASH_WHITE_KNIGHT[64];
extern bb HASH_BLACK_KNIGHT[64];
extern bb HASH_WHITE_BISHOP[64];
extern bb HASH_BLACK_BISHOP[64];
extern bb HASH_WHITE_ROOK[64];
extern bb HASH_BLACK_ROOK[64];
extern bb HASH_WHITE_QUEEN[64];
extern bb HASH_BLACK_QUEEN[64];
extern bb HASH_WHITE_KING[64];
extern bb HASH_BLACK_KING[64];
extern bb HASH_CASTLE[16];
extern bb HASH_EP[8];
extern bb HASH_COLOR;

void bb_init();
void bb_print(bb value);
bb bb_random();

bb bb_bishop(int sq, bb obstacles);
bb bb_rook(int sq, bb obstacles);
bb bb_queen(int sq, bb obstacles);

#endif

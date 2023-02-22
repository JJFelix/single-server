/* lcgrand.c */

#define MODLUS 2147483647
#define MULT1       24112
#define MULT2       26143

static long seed_table[256] = {0};
static int stream_num = 0;

double lcgrand(int stream) {
    long zi, lowprd, hi31;

    zi     = seed_table[stream];
    lowprd = (zi & 65535) * MULT1;
    hi31   = (zi >> 16) * MULT1 + (lowprd >> 16);
    zi     = ((lowprd & 65535) - MODLUS) +
             ((hi31 & 32767) << 16) + (hi31 >> 15);
    if (zi < 0) {
        zi += MODLUS;
    }
    seed_table[stream] = zi;
    return (zi >> 7 | 1) / 16777216.0;
}

void lcgrandst(long zset, int stream) {
    seed_table[stream] = zset;
}

long lcgrandgt(int stream) {
    return seed_table[stream];
}

void lcgrandst_tbl(long *tbl, int size) {
    int i;
    for (i = 0; i < size; i++) {
        seed_table[i] = tbl[i];
    }
}

void lcgrandst_tbl_idx(long *tbl, int size, int idx) {
    int i;
    for (i = 0; i < size; i++) {
        seed_table[i+idx] = tbl[i];
    }
}

void lcgrandst_tbl_single(long val, int idx) {
    seed_table[idx] = val;
}

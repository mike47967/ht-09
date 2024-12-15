#include "random_source.h"

enum {
    FIRST_BIT = 0x80000000,
    LOWER_BITS = 0x7fffffff,
    TEMP_MASK_B = 0x9d2c5680,
    TEMP_MASK_C = 0xefc60000,
    UINT32_MASK = 0xffffffff,
    MAG_MASK_1 = 0x0,
    MAG_MASK_2 = 0x9908b0df,
    INIT_SEED = 4357
};

RandomSource *random_factory_mt(const char *params);

static void
initseed(mt_rnd *rnd, uint32_t seed)
{
    rnd->mt[0] = seed & UINT32_MASK;
    for (rnd->idx = 1; rnd->idx < STATE_LEN; rnd->idx++) {
        rnd->mt[rnd->idx] = (6069 * rnd->mt[rnd->idx - 1]) & UINT32_MASK;
    }
}

mt_rnd
seedrnd(uint32_t seed)
{
    mt_rnd rnd;
    initseed(&rnd, seed);
    return rnd;
}

uint32_t
longrnd(mt_rnd *rnd)
{
    uint32_t y;
    uint32_t mag[2] = {MAG_MASK_1, MAG_MASK_2};
    if (rnd->idx >= STATE_LEN || rnd->idx < 0) {
        int32_t i;
        if (rnd->idx >= STATE_LEN + 1 || rnd->idx < 0) {
            initseed(rnd, INIT_SEED);
        }
        for (i = 0; i < STATE_LEN - STATE_M; i++) {
            y = (rnd->mt[i] & FIRST_BIT) | (rnd->mt[i + 1] & LOWER_BITS);
            rnd->mt[i] = rnd->mt[i + STATE_M] ^ (y >> 1) ^ mag[y & 0x1];
        }
        for (; i < STATE_LEN - 1; i++) {
            y = (rnd->mt[i] & FIRST_BIT) | (rnd->mt[i + 1] & LOWER_BITS);
            rnd->mt[i] = rnd->mt[i + (STATE_M - STATE_LEN)] ^ (y >> 1) ^ mag[y & 0x1];
        }
        y = (rnd->mt[STATE_LEN - 1] & FIRST_BIT) | (rnd->mt[0] & LOWER_BITS);
        rnd->mt[STATE_LEN - 1] = rnd->mt[STATE_M - 1] ^ (y >> 1) ^ mag[y & 0x1];
        rnd->idx = 0;
    }
    y = rnd->mt[rnd->idx++];
    y ^= (y >> 11);
    y ^= (y << 7) & TEMP_MASK_B;
    y ^= (y << 15) & TEMP_MASK_C;
    y ^= (y >> 18);
    return y;
}

double
rnd_gen(mt_rnd *rnd)
{
    return (double) ((double) longrnd(rnd) / (uint32_t) UINT32_MASK);
}
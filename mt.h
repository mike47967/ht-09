#include "random_source.h"

enum {
    FIRST_BIT = 0x80000000,
    LOWER_BITS = 0x7fffffff,
    TEMPERING_MASK_B = 0x9d2c5680,
    TEMPERING_MASK_C = 0xefc60000,
    UINT32_MASK = 0xffffffff
};

RandomSource *random_factory_mt(const char *params);

inline static void
initseed(mt_rnd *rand, uint32_t seed)
{
    rand->mt[0] = seed & UINT32_MASK;
    for (rand->idx = 1; rand->idx < STATE_LEN; rand->idx++) {
        rand->mt[rand->idx] = (6069 * rand->mt[rand->idx - 1]) & UINT32_MASK;
    }
}

mt_rnd
seedrnd(uint32_t seed)
{
    mt_rnd rand;
    initseed(&rand, seed);
    return rand;
}

uint32_t
longrnd(mt_rnd *rand)
{
    uint32_t y;
    static uint32_t mag[2] = {0x0, 0x9908b0df};
    if (rand->idx >= STATE_LEN || rand->idx < 0) {
        int32_t kk;
        if (rand->idx >= STATE_LEN + 1 || rand->idx < 0) {
            initseed(rand, 4357);
        }
        for (kk = 0; kk < STATE_LEN - STATE_M; kk++) {
            y = (rand->mt[kk] & FIRST_BIT) | (rand->mt[kk + 1] & LOWER_BITS);
            rand->mt[kk] = rand->mt[kk + STATE_M] ^ (y >> 1) ^ mag[y & 0x1];
        }
        for (; kk < STATE_LEN - 1; kk++) {
            y = (rand->mt[kk] & FIRST_BIT) | (rand->mt[kk + 1] & LOWER_BITS);
            rand->mt[kk] = rand->mt[kk + (STATE_M - STATE_LEN)] ^ (y >> 1) ^ mag[y & 0x1];
        }
        y = (rand->mt[STATE_LEN - 1] & FIRST_BIT) | (rand->mt[0] & LOWER_BITS);
        rand->mt[STATE_LEN - 1] = rand->mt[STATE_M - 1] ^ (y >> 1) ^ mag[y & 0x1];
        rand->idx = 0;
    }
    y = rand->mt[rand->idx++];
    y ^= (y >> 11);
    y ^= (y << 7) & TEMPERING_MASK_B;
    y ^= (y << 15) & TEMPERING_MASK_C;
    y ^= (y >> 18);
    return y;
}

double
rnd(mt_rnd *rand)
{
    return (double) ((double) longrnd(rand) / (uint32_t) UINT32_MASK);
}
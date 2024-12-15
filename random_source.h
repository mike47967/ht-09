#include <stdint.h>

#define EXIT_ERROR NULL

enum
{
    DEC_RADIX = 10,
    STATE_LEN = 624,
    STATE_M = 397
};

typedef struct s_mt_rnd
{
    uint32_t mt[STATE_LEN];
    int32_t idx;
} mt_rnd;

mt_rnd seedrnd(uint32_t seed);
double rnd(mt_rnd *rand);
uint32_t longrnd(mt_rnd *rand);

typedef struct RandomSourceOps
{
    struct RandomSource *(*destroy)(struct RandomSource *src);
    double (*next)(struct RandomSource *src);
} RandomSourceOps;

typedef struct RandomSource
{
    RandomSourceOps *ops;
    void *gen;
} RandomSource;

#include <stdint.h>

#define EXIT_ERROR NULL

enum
{
    NUM_STR_LEN = 10,
    STATE_LEN = 624,
    STATE_M = 397
};

typedef struct s_mt_rnd
{
    uint32_t mt[STATE_LEN];
    int32_t idx;
} mt_rnd;

mt_rnd seedrnd(uint32_t seed);
uint32_t longrnd(mt_rnd *rand);
double rnd_gen(mt_rnd *rand);

typedef struct RandomSourceOps
{
    struct RandomSource *(*destroy)(struct RandomSource *source);
    double (*next)(struct RandomSource *source);
} RandomSourceOps;

typedef struct RandomSource
{
    RandomSourceOps *ops;
    void *gen;
} RandomSource;

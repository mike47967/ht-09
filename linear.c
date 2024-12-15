#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "random_source.h"

RandomSource *random_factory_linear(const char *params);

enum
{
    MOD = 1LL << 31,
    MUL = 1103515245LL,
    INCR = 12345LL,
};

typedef struct linear_rnd
{
    long long prev;
} linear_rnd;

static double
linear_next(RandomSource *source)
{
    linear_rnd *gen = source->gen;
    gen->prev = (MUL * gen->prev + INCR) % MOD;
    source->gen = gen;
    return (double) gen->prev / ((double) INT_MAX + 1);
}

static RandomSource *
linear_destroy(RandomSource *source)
{
    free(source->ops);
    free(source->gen);
    free(source);
    return EXIT_ERROR;
}

RandomSource *
random_factory_linear(const char *params)
{
    RandomSource *res = calloc(1, sizeof(*res));
    RandomSourceOps *ops = calloc(1, sizeof(*ops));
    if (!ops || !res) {
        perror("calloc");
        return EXIT_ERROR;
    }

    ops->next = linear_next;
    ops->destroy = linear_destroy;
    res->ops = ops;

    errno = 0;
    char *eptr = NULL;
    long seed = strtol(params, &eptr, NUM_STR_LEN);
    if (errno || *eptr || (int) seed != seed || eptr == params || seed < 0) {
        perror("strtol");
        return EXIT_ERROR;
    }

    seed &= INT_MAX;

    linear_rnd *gen = calloc(1, sizeof(*gen));
    if (gen == EXIT_ERROR) {
        perror("calloc");
        return EXIT_ERROR;
    }

    gen->prev = seed;
    res->gen = gen;
    return res;
}

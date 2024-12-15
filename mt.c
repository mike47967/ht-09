#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "mt.h"

static double
mt_next(RandomSource *src)
{
    return rnd(src->gen);
}

static RandomSource *
mt_destroy(RandomSource *src)
{
    free(src->ops);
    free(src->gen);
    free(src);
    return EXIT_ERROR;
}

RandomSource *
random_factory_mt(const char *params)
{
    errno = 0;
    char *eptr = NULL;
    long seed = strtol(params, &eptr, DEC_RADIX);
    if (errno || *eptr || (int) seed != seed || eptr == params || seed < 0) {
        fprintf(stderr, "Incorrect seed");

        return EXIT_ERROR;
    }

    seed = seed & INT_MAX;

    RandomSource *result = calloc(1, sizeof(*result));
    if (result == NULL) {
        fprintf(stderr, "Error allocating RandomSource\n");

        return EXIT_ERROR;
    }

    RandomSourceOps *ops = calloc(1, sizeof(*ops));
    if (ops == NULL) {
        fprintf(stderr, "Error allocating RandomSourceOps\n");

        return EXIT_ERROR;
    }

    ops->next = mt_next;
    ops->destroy = mt_destroy;
    result->ops = ops;

    mt_rnd *gen = calloc(1, sizeof(*gen));
    if (gen == NULL) {
        fprintf(stderr, "Error allocating mt_rnd\n");
        return EXIT_ERROR;
    }

    *gen = seedrnd(seed);
    result->gen = gen;
    return result;
}

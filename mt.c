#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "mt.h"

static double
mt_next(RandomSource *source)
{
    return rnd_gen(source->gen);
}

static RandomSource *
mt_destroy(RandomSource *source)
{
    free(source->ops);
    free(source->gen);
    free(source);
    return EXIT_ERROR;
}

RandomSource *
random_factory_mt(const char *num_str)
{
    errno = 0;
    char *buf = NULL;
    long seed = strtol(num_str, &buf, NUM_STR_LEN);
    if (errno || *buf || (int) seed != seed || buf == num_str || seed < 0) {
        fprintf(stderr, "Incorrect seed");

        return EXIT_ERROR;
    }

    seed = seed & INT_MAX;

    RandomSource *res = calloc(1, sizeof(*res));
    if (res == NULL) {
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
    res->ops = ops;

    mt_rnd *gen = calloc(1, sizeof(*gen));
    if (gen == NULL) {
        fprintf(stderr, "Error allocating mt_rnd\n");
        return EXIT_ERROR;
    }

    *gen = seedrnd(seed);
    res->gen = gen;
    return res;
}

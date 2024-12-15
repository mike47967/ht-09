#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "random_source.h"

enum {
    DEFAULT_ARGS_NUM = 4,
    MIN_N = 0,
    MAX_N = 100000,
    NUM_STR_LEN = 10
};

const char *linear_mode_enabler = "linear";
const char *mt_mode_enabler = "mt";

int main(int argc, char **argv) {
    if (argc < DEFAULT_ARGS_NUM) {
        fprintf(stderr, "Usage: %s <generator type [linear|mt]> <n> <seed>\n", argv[0]);
        return EXIT_FAILURE;
    }

    void *handle = dlopen(NULL, RTLD_LAZY);
    if (handle == NULL) {
        perror("dlopen");
        return EXIT_FAILURE;
    }

    const char *GENERATOR_TYPE = argv[1];
    RandomSource *(*fn)(const char *) = NULL;

    if (!strcmp(GENERATOR_TYPE, linear_mode_enabler)) {
        void *sym = dlsym(handle, "random_factory_linear");
        if (!sym) {
            perror("dlsym linear");
            dlclose(handle);
            return EXIT_FAILURE;
        }
        fn = (RandomSource *(*)(const char *)) sym;
    } else if (!strcmp(GENERATOR_TYPE, mt_mode_enabler)) {
        void *sym = dlsym(handle, "random_factory_mt");
        if (!sym) {
            perror("dlsym mt");
            dlclose(handle);
            return EXIT_FAILURE;
        }
        fn = (RandomSource *(*)(const char *)) sym;
    } else {
        fprintf(stderr, "Error: Invalid generator type.\n");
        dlclose(handle);
        return EXIT_FAILURE;
    }

    const char *N = argv[2];
    errno = 0;
    char *eptr = NULL;
    long n = strtol(N, &eptr, NUM_STR_LEN);
    if (errno || *eptr != '\0' || eptr == N || n < MIN_N || n > MAX_N) {
        perror("strtol");
        dlclose(handle);
        return EXIT_FAILURE;
    }

    const char *SEED = argv[3];
    RandomSource *generator = fn(SEED);
    if (generator == NULL) {
        perror("fn");
        dlclose(handle);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++) {
        printf("%.10g\n", generator->ops->next(generator));
    }

    generator = generator->ops->destroy(generator);

    if (dlclose(handle) != 0) {
        fprintf(stderr, "Error: Failed to close shared object.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "random_source.h"

enum {
    DEFAULT_ARGS_NUM = 4
};

const char *linear_mode_enabler = "linear";
const char *mt_mode_enabler = "mt";

int main(int argc, char **argv) {
    if (argc < DEFAULT_ARGS_NUM) {
        fprintf(stderr, "Usage: %s <gen type [linear|mt]> <n> <seed>\n", argv[0]);
        return 1;
    }

    void *hndl = dlopen(NULL, RTLD_LAZY);
    if (hndl == NULL) {
        perror("dlopen");
        return 1;
    }

    const char *type = argv[1];
    RandomSource *(*f_rnd)(const char *) = NULL;

    if (!strcmp(type, linear_mode_enabler)) {
        void *sym = dlsym(hndl, "random_factory_linear");
        if (!sym) {
            perror("dlsym linear");
            dlclose(hndl);
            return 1;
        }
        f_rnd = (RandomSource *(*)(const char *)) sym;
    } else if (!strcmp(type, mt_mode_enabler)) {
        void *sym = dlsym(hndl, "random_factory_mt");
        if (!sym) {
            perror("dlsym mt");
            dlclose(hndl);
            return 1;
        }
        f_rnd = (RandomSource *(*)(const char *)) sym;
    } else {
        fprintf(stderr, "Error: Invalid gen type.\n");
        dlclose(hndl);
        return 1;
    }

    const char *N = argv[2];
    errno = 0;
    char *buf = NULL;
    long n = strtol(N, &buf, NUM_STR_LEN);
    if (errno || *buf != '\0' || buf == N || n < 0) {
        perror("strtol");
        dlclose(hndl);
        return 1;
    }

    const char *seed = argv[3];
    RandomSource *gen = f_rnd(seed);
    if (gen == NULL) {
        perror("f_rnd");
        dlclose(hndl);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("%.10g\n", gen->ops->next(gen));
    }

    gen = gen->ops->destroy(gen);

    if (dlclose(hndl) != 0) {
        fprintf(stderr, "Error: Failed to close shared object.\n");
        return 1;
    }

    return 0;
}

#ifndef OSPREY_C_REPEAT_H
#define OSPREY_C_REPEAT_H
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

static uint64_t osprey_repetitions(int argc, char **argv) {
    if (argc == 1) return 1;
    if (argc == 3 && strcmp(argv[1], "--repeat") == 0 && *argv[2]) {
        uint64_t n = 0;
        for (const char *p = argv[2]; *p; ++p) {
            if (*p < '0' || *p > '9') goto invalid;
            n = n * 10 + (unsigned)(*p - '0');
            if (n > 100000000) goto invalid;
        }
        if (n) return n;
    }
invalid:
    fprintf(stderr, "usage: %s [--repeat N]\n", argv[0]);
    return 0;
}
#endif

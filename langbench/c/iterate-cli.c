#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* seed -> a * seed + b, applied n times (unsigned 64-bit wrap). */
static uint64_t iterate(uint64_t n, uint64_t a, uint64_t b, uint64_t seed) {
    for (uint64_t i = 0; i < n; ++i) seed = a * seed + b;
    return seed;
}

static int parse(const char *text, uint64_t *value) {
    if (!*text) return 0;
    for (const char *p = text; *p; ++p) if (*p < '0' || *p > '9') return 0;
    errno = 0;
    char *end;
    uintmax_t parsed = strtoumax(text, &end, 10);
    if (errno || *end || parsed > UINT64_MAX) return 0;
    *value = (uint64_t)parsed;
    return 1;
}

int main(int argc, char **argv) {
    uint64_t n = UINT64_C(5000000000), a = 3, b = 5, seed = 7;  /* defaults: the benchmark case */
    if (argc > 5 || (argc > 1 && !parse(argv[1], &n)) || (argc > 2 && !parse(argv[2], &a))
        || (argc > 3 && !parse(argv[3], &b)) || (argc > 4 && !parse(argv[4], &seed))) {
        fputs("usage: iterate [n [a [b [seed]]]] (unsigned 64-bit decimal integers; defaults 5000000000 3 5 7)\n", stderr);
        return 2;
    }
    printf("%" PRIu64 "\n", iterate(n, a, b, seed));
    return 0;
}

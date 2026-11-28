#include "benchmark-input.h"

typedef struct { uint16_t *units; size_t length; } text;

/* Same construction and counted UTF-16 representation as the recursive
   original, built by a loop over levels 1..n instead of recursing from n to 0.
   Each level still allocates a new buffer, copies the previous text and frees
   it; clang -O3 may eliminate their storage. */
static text words(uint64_t n) {
    uint16_t *units = malloc(3 * sizeof(uint16_t));
    if (!units) abort();
    units[0] = 'e'; units[1] = 'n'; units[2] = 'd';
    size_t length = 3;
    for (uint64_t level = 1; level <= n; ++level) {
        size_t suffix = level % 2 ? 1 : 2, next_length = length + suffix;
        uint16_t *next = malloc(next_length * sizeof(uint16_t));
        if (!next) abort();
        memcpy(next, units, length * sizeof(uint16_t));
        if (suffix == 1) next[length] = 'x';
        else { next[length] = 0xd83d; next[length + 1] = 0xde00; }
        free(units);
        units = next;
        length = next_length;
    }
    return (text){units, length};
}

int main(int argc, char **argv) {
    uint64_t depth = 65536;  /* default: the benchmark case */
    if (argc > 2 || (argc == 2 && !bench_parse(argv[1], &depth)) || depth > 65536)
        return bench_usage("usage: string-length [recursive depth, 0..65536] (default 65536)");
    text result = words(depth);
    printf("%zu\n", result.length);
    free(result.units);
    return 0;
}

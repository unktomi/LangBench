#include "benchmark-input.h"

/* Insertion sort, back to front: each element is inserted before the already
 * sorted suffix by shifting smaller suffix elements down one cell. Same
 * comparisons and moves as the recursive original, with no recursion (so no
 * stack growth with n). */
static void sort(uint64_t *values, size_t n) {
    for (size_t i = n; i-- > 0;) {
        uint64_t x = values[i];
        size_t j = i;
        while (j + 1 < n && x > values[j + 1]) {
            values[j] = values[j + 1];
            ++j;
        }
        values[j] = x;
    }
}

static char default_count[] = "240000", default_seed[] = "42";

int main(int argc, char **argv) {
    char *filled[4];
    /* --generate [count [seed]]: absent count/seed take the benchmark defaults. */
    if (argc >= 2 && argc < 4 && strcmp(argv[1], "--generate") == 0) {
        filled[0] = argv[0];
        filled[1] = argv[1];
        filled[2] = argc > 2 ? argv[2] : default_count;
        filled[3] = default_seed;
        argv = filled;
        argc = 4;
    }
    uint64_t *values;
    size_t n;
    if (!bench_input(argc, argv, 1, &values, &n))
        return bench_usage("usage: sorted-first [values...] | --generate [count [seed]] (defaults 240000 42; no arguments: empty list returns 0)");
    sort(values, n);
    printf("%" PRIu64 "\n", n ? values[0] : UINT64_C(0));
    free(values);
    return 0;
}

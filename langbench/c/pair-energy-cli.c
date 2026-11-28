#include "benchmark-input.h"

/* Sum over all ordered pairs (x, y) of (x - y)^2, unsigned 64-bit wrap.
 * Nested loops replace the recursive row_energy/all_energy of the original. */
static uint64_t all_energy(const uint64_t *values, size_t n) {
    uint64_t total = 0;
    for (size_t i = 0; i < n; ++i) {
        uint64_t x = values[i];
        for (size_t j = 0; j < n; ++j) {
            uint64_t difference = x - values[j];
            total += difference * difference;
        }
    }
    return total;
}

static char default_count[] = "131072", default_seed[] = "42";

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
        return bench_usage("usage: pair-energy [values...] | --generate [count [seed]] (defaults 131072 42)");
    printf("%" PRIu64 "\n", all_energy(values, n));
    free(values);
    return 0;
}

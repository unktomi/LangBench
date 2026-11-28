#include "benchmark-input.h"

/* SortedFirst.lang inserts each element into its already sorted suffix.
 * Walk from the back and shift the suffix in place to perform the same sort.
 * The result is observed at the zero-based rank supplied on the command line.
 */
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

int main(int argc, char **argv) {
    const char *usage = "usage: sorted-k <k> [values...] | sorted-k <k> --generate [count [seed]] (zero-based rank; out of range returns 0; generate defaults 240000 42)";
    uint64_t rank;
    if (argc < 2 || !bench_parse(argv[1], &rank))
        return bench_usage(usage);

    char default_count[] = "240000", default_seed[] = "42";
    char *filled[5];
    if (argc >= 3 && argc < 5 && strcmp(argv[2], "--generate") == 0) {
        filled[0] = argv[0];
        filled[1] = argv[1];
        filled[2] = argv[2];
        filled[3] = argc > 3 ? argv[3] : default_count;
        filled[4] = default_seed;
        argv = filled;
        argc = 5;
    }

    uint64_t *values;
    size_t n;
    if (!bench_input(argc, argv, 2, &values, &n))
        return bench_usage(usage);
    sort(values, n);
    printf("%" PRIu64 "\n", rank < n ? values[(size_t)rank] : UINT64_C(0));
    free(values);
    return 0;
}

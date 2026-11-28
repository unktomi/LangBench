#include "benchmark-input.h"

static uint64_t inversion_count(const uint64_t *a, size_t n) {
    uint64_t count = 0;
    for (size_t i = 0; i < n; ++i)
        for (size_t j = i + 1; j < n; ++j)
            if (a[i] > a[j]) ++count;
    return count;
}

int main(int argc, char **argv) {
    char *filled[4];
    if (argc >= 2 && argc < 4 && strcmp(argv[1], "--generate") == 0) {
        filled[0] = argv[0];
        filled[1] = argv[1];
        filled[2] = argc > 2 ? argv[2] : "131072";
        filled[3] = "42";
        argv = filled;
        argc = 4;
    }
    uint64_t *values;
    size_t n;
    if (!bench_input(argc, argv, 1, &values, &n))
        return bench_usage("usage: inversion-count [values...] | --generate [count [seed]] (defaults 131072 42)");
    printf("%" PRIu64 "\n", inversion_count(values, n));
    free(values);
    return 0;
}

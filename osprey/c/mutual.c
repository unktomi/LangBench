/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/mutual/mutual.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
#include <stdint.h>
#include <stdio.h>

static int is_odd(int64_t n);

static int is_even(int64_t n) {
    if (n == 0) {
        return 1;
    }
    return is_odd(n - 1);
}

static int is_odd(int64_t n) {
    if (n == 0) {
        return 0;
    }
    return is_even(n - 1);
}

int main(int argc, char **argv) {
    int64_t parameters[1];
    if (!osprey_arguments(argc, argv, 1, parameters)) return 2;
    const int64_t limit = parameters[0];
    int64_t acc = 0;
    for (int64_t i = 1; i <= limit; i++) {
        if (is_even(i % 1000)) {
            acc += 1;
        }
    }
    printf("%lld\n", (long long)acc);
    return 0;
}

/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/isqrt/isqrt.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
/* Sum of integer square roots over 1..N — Newton's method, integer division heavy. */
#include <stdio.h>
#include <stdint.h>

static int64_t isqrt(int64_t n) {
    if (n < 2) return n;
    int64_t x = n;
    for (;;) {
        int64_t y = (x + n / x) / 2;
        if (y < x) x = y; else return x;
    }
}

int main(int argc, char **argv) {
    int64_t parameters[1];
    if (!osprey_arguments(argc, argv, 1, parameters)) return 2;
    const int64_t limit = parameters[0];
    int64_t acc = 0;
    for (int64_t i = 1; i <= limit; i++) acc += isqrt(i);
    printf("%lld\n", (long long)acc);
    return 0;
}

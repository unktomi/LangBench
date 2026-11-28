/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/gcdsum/gcdsum.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
/* Sum of gcd(i, K) for i in 1..N — Euclidean-algorithm recursion (modulo heavy). */
#include <stdio.h>
#include <stdint.h>

static int64_t gcd(int64_t a, int64_t b) {
    return b == 0 ? a : gcd(b, a % b);
}

int main(int argc, char **argv) {
    int64_t parameters[1];
    if (!osprey_arguments(argc, argv, 1, parameters)) return 2;
    const int64_t limit = parameters[0];
    int64_t acc = 0;
    for (int64_t i = 1; i <= limit; i++) {
        acc += gcd(i, 1234567);
    }
    printf("%lld\n", (long long)acc);
    return 0;
}

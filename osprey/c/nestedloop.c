/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/nestedloop/nestedloop.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
/* Triple-nested counting loop accumulating (i*j*k) mod P — nested iteration. */
#include <stdio.h>
#include <stdint.h>

static const int64_t P = 1000000007;

static int64_t loop_k(int64_t i, int64_t j, int64_t k, int64_t acc) {
    while (k != 0) {
        acc = (acc + i * j * k) % P;
        k -= 1;
    }
    return acc;
}

static int64_t loop_j(int64_t i, int64_t j, int64_t n, int64_t acc) {
    while (j != 0) {
        acc = loop_k(i, j, n, acc);
        j -= 1;
    }
    return acc;
}

static int64_t loop_i(int64_t i, int64_t n, int64_t acc) {
    while (i != 0) {
        acc = loop_j(i, n, n, acc);
        i -= 1;
    }
    return acc;
}

int main(int argc, char **argv) {
    int64_t parameters[1];
    if (!osprey_arguments(argc, argv, 1, parameters)) return 2;
    const int64_t n = parameters[0];
    printf("%lld\n", (long long)loop_i(n, n, 0));
    return 0;
}

/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/pascal/pascal.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
/* Binomial coefficient via naive (un-memoised) Pascal recursion C(n,k)=C(n-1,k-1)+C(n-1,k). */
#include <stdio.h>
#include <stdint.h>

static int64_t binom(int64_t n, int64_t k) {
    if (k == 0) {
        return 1;
    } else if (k == n) {
        return 1;
    } else {
        return binom(n - 1, k - 1) + binom(n - 1, k);
    }
}

int main(int argc, char **argv) {
    int64_t parameters[2];
    if (!osprey_arguments(argc, argv, 2, parameters)) return 2;
    const int64_t n = parameters[0];
    const int64_t k = parameters[1];
    printf("%lld\n", (long long)binom(n, k));
    return 0;
}

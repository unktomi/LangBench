/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/hanoi/hanoi.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
#include <stdint.h>
#include <stdio.h>

int64_t hanoi(int64_t n, int64_t acc) {
    if (n == 0) {
        return acc;
    }
    return hanoi(n - 1, hanoi(n - 1, acc) + 1);
}

int main(int argc, char **argv) {
    int64_t parameters[1];
    if (!osprey_arguments(argc, argv, 1, parameters)) return 2;
    const int64_t n = parameters[0];
    printf("%lld\n", (long long)hanoi(n, 0));
    return 0;
}

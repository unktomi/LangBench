/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/coins/coins.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
#include <stdio.h>
#include <stdint.h>

static int64_t coin(int64_t k) {
    switch (k) {
        case 1: return 1;
        case 2: return 5;
        case 3: return 10;
        case 4: return 25;
        default: return 50;
    }
}

static int64_t ways(int64_t amount, int64_t kind) {
    if (amount == 0) return 1;
    if (amount < 0) return 0;
    if (kind == 0) return 0;
    return ways(amount - coin(kind), kind) + ways(amount, kind - 1);
}

int main(int argc, char **argv) {
    int64_t parameters[2];
    if (!osprey_arguments(argc, argv, 2, parameters)) return 2;
    const int64_t amount = parameters[0];
    const int64_t kind = parameters[1];
    printf("%lld\n", (long long)ways(amount, kind));
    return 0;
}

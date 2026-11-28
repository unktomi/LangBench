/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/factorial/factorial.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
/* Factorial-style product 1*2*...*N taken mod 1000000007 (matches factorial.osp). */
#include <stdio.h>
#include <stdint.h>

#define MOD 1000000007LL

int main(int argc, char **argv) {
    int64_t parameters[1];
    if (!osprey_arguments(argc, argv, 1, parameters)) return 2;
    const int64_t limit = parameters[0];
    int64_t acc = 1;
    for (int64_t i = 1; i <= limit; i++) {
        acc = (acc * i) % MOD;
    }
    printf("%lld\n", (long long)acc);
    return 0;
}

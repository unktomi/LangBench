/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/josephus/josephus.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
/* Josephus problem — survivor index for n people, step k=7, via the modular recurrence. */
#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv) {
    int64_t parameters[1];
    if (!osprey_arguments(argc, argv, 1, parameters)) return 2;
    const int64_t n = parameters[0];
    int64_t acc = 0;
    for (int64_t i = 2; i <= n; i++) {
        acc = (acc + 7) % i;
    }
    printf("%lld\n", (long long)acc);
    return 0;
}

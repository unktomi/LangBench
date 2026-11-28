/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/collatz/collatz.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
/* Collatz (3n+1) stopping time summed over 1..N — integer division (n/2) in deep recursion. */
#include <stdio.h>
#include <stdint.h>

static int64_t collatz(int64_t n) {
    if (n == 1) return 0;
    return (n % 2 == 0) ? 1 + collatz(n / 2) : 1 + collatz(3 * n + 1);
}

int main(int argc, char **argv) {
    int64_t parameters[1];
    if (!osprey_arguments(argc, argv, 1, parameters)) return 2;
    const int64_t limit = parameters[0];
    int64_t acc = 0;
    for (int64_t i = 1; i <= limit; i++) acc += collatz(i);
    printf("%lld\n", (long long)acc);
    return 0;
}

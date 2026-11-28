/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/digitsum/digitsum.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
/* Sum of decimal digit-sums over 1..N — integer division (n/10) and modulo in recursion. */
#include <stdio.h>
#include <stdint.h>

static int64_t digsum(int64_t n) {
    return n < 10 ? n : (n % 10) + digsum(n / 10);
}

int main(int argc, char **argv) {
    int64_t parameters[1];
    if (!osprey_arguments(argc, argv, 1, parameters)) return 2;
    const int64_t limit = parameters[0];
    int64_t acc = 0;
    for (int64_t i = 1; i <= limit; i++) acc += digsum(i);
    printf("%lld\n", (long long)acc);
    return 0;
}

/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/primes/primes.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
/* Count primes below a limit by trial division — integer % in a tight loop. */
#include <stdio.h>
#include <stdint.h>

static int has_factor(int64_t n, int64_t d) {
    if (d * d > n) {
        return 0;
    } else if (n % d == 0) {
        return 1;
    } else {
        return has_factor(n, d + 1);
    }
}

static int is_prime(int64_t n) {
    if (n < 2) {
        return 0;
    }
    return !has_factor(n, 2);
}

int main(int argc, char **argv) {
    int64_t parameters[1];
    if (!osprey_arguments(argc, argv, 1, parameters)) return 2;
    const int64_t limit = parameters[0];
    int64_t acc = 0;
    for (int64_t n = 2; n <= limit; n++) {
        if (is_prime(n)) {
            acc += 1;
        }
    }
    printf("%lld\n", (long long)acc);
    return 0;
}

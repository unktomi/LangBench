/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/tak/tak.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
#include <stdio.h>
#include <stdint.h>

static int64_t sub(int64_t a, int64_t b) {
    return a - b;
}

static int64_t tak(int64_t x, int64_t y, int64_t z) {
    if (x > y) {
        return tak(
            tak(sub(x, 1), y, z),
            tak(sub(y, 1), z, x),
            tak(sub(z, 1), x, y));
    }
    return z;
}

int main(int argc, char **argv) {
    int64_t parameters[3];
    if (!osprey_arguments(argc, argv, 3, parameters)) return 2;
    const int64_t x = parameters[0];
    const int64_t y = parameters[1];
    const int64_t z = parameters[2];
    printf("%lld\n", (long long)tak(x, y, z));
    return 0;
}

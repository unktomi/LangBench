/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/fib/fib.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
/* Naive recursive Fibonacci — exercises raw function-call + recursion overhead. */
#include <stdio.h>
#include <stdint.h>

static int64_t add(int64_t a, int64_t b) { return a + b; }
static int64_t sub(int64_t a, int64_t b) { return a - b; }

static int64_t fib(int64_t n) {
    switch (n) {
        case 0: return 0;
        case 1: return 1;
        default: return add(fib(sub(n, 1)), fib(sub(n, 2)));
    }
}

int main(int argc, char **argv) {
    int64_t parameters[1];
    if (!osprey_arguments(argc, argv, 1, parameters)) return 2;
    const int64_t n = parameters[0];
    printf("%lld\n", (long long)fib(n));
    return 0;
}

/* CLI adaptation of the pinned Osprey reference:
 * tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/
 * reference/cases/ackermann/ackermann.c
 * The recurrence is unchanged. Unsigned 64-bit operations match Lang Uint64. */
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t add(uint64_t a, uint64_t b) { return a + b; }
static uint64_t sub(uint64_t a, uint64_t b) { return a - b; }

static uint64_t ack(uint64_t m, uint64_t n) {
    if (m == 0) {
        return add(n, 1);
    } else if (n == 0) {
        return ack(sub(m, 1), 1);
    } else {
        return ack(sub(m, 1), ack(m, sub(n, 1)));
    }
}

static int argument(const char *text, uint64_t *result) {
    if (!*text) return 0;
    for (const char *p = text; *p; ++p) if (*p < '0' || *p > '9') return 0;
    errno = 0;
    char *end;
    uintmax_t value = strtoumax(text, &end, 10);
    if (errno || *end || value > UINT64_MAX) return 0;
    *result = (uint64_t)value;
    return 1;
}

int main(int argc, char **argv) {
    uint64_t m, n;
    if (argc != 3 || !argument(argv[1], &m) || !argument(argv[2], &n)) {
        fputs("usage: ackermann-c <m> <n> (unsigned 64-bit decimal integers)\n", stderr);
        return 2;
    }
    printf("%" PRIu64 "\n", ack(m, n));
    return 0;
}

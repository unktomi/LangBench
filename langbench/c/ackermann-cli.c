/* Loop version of the CLI adaptation of the pinned Osprey reference ackermann.c.
 * The recurrence is unchanged; the call stack is replaced by an explicit stack
 * of pending first arguments, so there is no recursion. Unsigned 64-bit
 * operations match Lang Uint64. */
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t add(uint64_t a, uint64_t b) { return a + b; }
static uint64_t sub(uint64_t a, uint64_t b) { return a - b; }

/* ack(m, n) = n + 1                       if m == 0
 *           = ack(m - 1, 1)               if n == 0
 *           = ack(m - 1, ack(m, n - 1))   otherwise
 * `pending` holds the first arguments of calls still waiting for the value n. */
static uint64_t ack(uint64_t m, uint64_t n) {
    size_t capacity = 64, top = 0;
    uint64_t *pending = malloc(capacity * sizeof *pending);
    if (!pending) abort();
    pending[top++] = m;
    while (top) {
        m = pending[--top];
        if (m == 0) {
            n = add(n, 1);
            continue;
        }
        if (top + 2 > capacity) {
            capacity *= 2;
            pending = realloc(pending, capacity * sizeof *pending);
            if (!pending) abort();
        }
        if (n == 0) {
            n = 1;
            pending[top++] = sub(m, 1);
        } else {
            pending[top++] = sub(m, 1);
            pending[top++] = m;
            n = sub(n, 1);
        }
    }
    free(pending);
    return n;
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
    uint64_t m = 3, n = 13;  /* defaults: the benchmark case A(3,13) */
    if (argc > 3 || (argc > 1 && !argument(argv[1], &m)) || (argc > 2 && !argument(argv[2], &n))) {
        fputs("usage: ackermann [m [n]] (unsigned 64-bit decimal integers; defaults 3 13)\n", stderr);
        return 2;
    }
    printf("%" PRIu64 "\n", ack(m, n));
    return 0;
}

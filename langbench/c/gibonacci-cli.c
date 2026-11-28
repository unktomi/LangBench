/* Loop version of the direct BER Gibonacci recurrence over uint64_t (wrapping
   modulo 2^64) with optional runtime seeds and depth (defaults 2, 3 and 45).
   Same call tree as the recursive original, walked with an explicit stack of
   pending depths (no recursion). Depth is limited to 62 so the explicit stack
   stays within its bound. */
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
static uint64_t gib(uint64_t x, uint64_t y, unsigned n) {
    unsigned pending[64];
    size_t top = 0;
    uint64_t total = 0;
    pending[top++] = n;
    while (top) {
        unsigned k = pending[--top];
        if (k == 0) total += x;
        else if (k == 1) total += y;
        else { pending[top++] = k - 2; pending[top++] = k - 1; }
    }
    return total;
}
static int parse(const char *text, uint64_t *value) {
    if (!*text) return 0;
    for (const char *p = text; *p; ++p) if (*p < '0' || *p > '9') return 0;
    errno = 0;
    char *end;
    uintmax_t parsed = strtoumax(text, &end, 10);
    if (errno || *end || parsed > UINT64_MAX) return 0;
    *value = (uint64_t)parsed;
    return 1;
}
int main(int argc, char **argv) {
    uint64_t x = 2, y = 3, depth = 45;  /* defaults: the benchmark case */
    if (argc > 4 || (argc > 1 && !parse(argv[1], &x)) || (argc > 2 && !parse(argv[2], &y))
        || (argc > 3 && !parse(argv[3], &depth)) || depth > 62) {
        fputs("usage: gibonacci [first seed [second seed [depth]]] (unsigned 64-bit decimal integers, wrapping; depth <= 62; defaults 2 3 45)\n", stderr);
        return 2;
    }
    printf("%" PRIu64 "\n", gib(x, y, (unsigned)depth));
    return 0;
}

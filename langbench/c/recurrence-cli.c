#include <stdint.h>
#include <stdlib.h>

// Exact translation of the user-supplied recurrence, including unsigned wrap.
// Loop version of the user-supplied recurrence (same call tree, unsigned wrap):
// explicit frames replace the call stack, so there is no recursion.
//   direct(0) = a, direct(1) = b, direct(n) = 3*direct(n-1) + 2*direct(n-2) + 5
uint64_t direct(uint64_t n, uint64_t a, uint64_t b) {
    typedef struct { uint64_t n; uint64_t x; int stage; } frame;
    if (n == 0) return a;
    if (n == 1) return b;
    frame *frames = malloc((size_t)(n + 1) * sizeof *frames);
    if (!frames) abort();
    size_t top = 0;
    uint64_t result = 0;
    frames[top++] = (frame){n, 0, 0};
    while (top) {
        frame *f = &frames[top - 1];
        if (f->stage == 0) {
            if (f->n == 0) { result = a; --top; }
            else if (f->n == 1) { result = b; --top; }
            else { f->stage = 1; frames[top++] = (frame){f->n - 1, 0, 0}; }
        } else if (f->stage == 1) {
            f->x = result; f->stage = 2; frames[top++] = (frame){f->n - 2, 0, 0};
        } else {
            result = f->x + f->x + f->x + result + result + 5;
            --top;
        }
    }
    free(frames);
    return result;
}

uint64_t iterate(uint64_t n, uint64_t a, uint64_t b) {
    if (n == 0) return a;
    while (--n) {
        uint64_t next = 3 * b + 2 * a + 5;
        a = b; b = next;
    }
    return b;
}

#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t direct(uint64_t, uint64_t, uint64_t);
uint64_t iterate(uint64_t, uint64_t, uint64_t);

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
    uint64_t n = 44, a = 2, b = 3;  /* defaults: the benchmark case */
    if (argc > 4 || (argc > 1 && !parse(argv[1], &n)) || (argc > 2 && !parse(argv[2], &a))
        || (argc > 3 && !parse(argv[3], &b))) {
        fputs("usage: recurrence [n [a [b]]] (unsigned 64-bit decimal integers; defaults 44 2 3)\n", stderr);
        return 2;
    }
#ifdef RECURRENCE_SEQUENTIAL
    uint64_t answer = iterate(n, a, b);
#else
    uint64_t answer = direct(n, a, b);
#endif
    printf("%" PRIu64 "\n", answer);
    return 0;
}

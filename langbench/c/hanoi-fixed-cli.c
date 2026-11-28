/* Loop version of the recursive definition in lang/hanoi-fixed.lang:
 *   hanoi(0, moves) = moves
 *   hanoi(d, moves) = hanoi(d - 1, hanoi(d - 1, moves) + 1)
 * The same call tree is walked with an explicit stack of actions: a
 * non-negative entry is a pending hanoi(d, .) call, -1 is a pending "+ 1".
 * The disk count is an optional argument (default 31, at most 63 so the move
 * count fits in uint64_t and the explicit stack stays within its bound). */
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t hanoi(int disks) {
    int pending[256];
    int top = 0;
    uint64_t moves = 0;
    pending[top++] = disks;
    while (top) {
        int action = pending[--top];
        if (action < 0) { moves += 1; continue; }
        if (action == 0) continue;
        pending[top++] = action - 1;   /* second call, after the increment */
        pending[top++] = -1;
        pending[top++] = action - 1;   /* first call runs next */
    }
    return moves;
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
    uint64_t disks = 31;  /* default: the benchmark case */
    if (argc > 2 || (argc == 2 && !parse(argv[1], &disks)) || disks > 63) {
        fputs("usage: hanoi [disks, 0..63] (default 31)\n", stderr);
        return 2;
    }
    printf("%" PRIu64 "\n", hanoi((int)disks));
    return 0;
}

#include "sequence-input.h"

static size_t lcs(const uint64_t *left, size_t n, const uint64_t *right, size_t m) {
    if (n == 0 || m == 0) return 0;
    if (*left == *right) return 1 + lcs(left + 1, n - 1, right + 1, m - 1);
    size_t a = lcs(left + 1, n - 1, right, m);
    size_t b = lcs(left, n, right + 1, m - 1);
    return a > b ? a : b;
}

int main(int argc, char **argv) {
    uint64_t *words;
    size_t length, split;
    if (!sequence_input(argc, argv, "400", &words, &length, &split))
        return bench_usage("usage: lcs <left-count> [values...] | lcs <left-count> --generate [count [seed]]");
    for (size_t i = 0; i < length; ++i) words[i] %= 4;
    size_t answer = length ? lcs(words, split, words + split, length - split) : 0;
    printf("%zu\n", answer);
    free(words);
    return 0;
}

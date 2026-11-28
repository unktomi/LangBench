#include "sequence-input.h"

static uint64_t embedding_pairs(const uint64_t *left, size_t n,
                                const uint64_t *right, size_t m) {
    if (n == 0 || m == 0) return UINT64_C(1);

    uint64_t a = embedding_pairs(left + 1, n - 1, right, m);
    uint64_t b = embedding_pairs(left, n, right + 1, m - 1);
    uint64_t d = embedding_pairs(left + 1, n - 1, right + 1, m - 1);
    uint64_t without_both = ((a + b + UINT64_C(65521)) - d) % UINT64_C(65521);
    if (*left == *right) return (without_both + d) % UINT64_C(65521);
    return without_both;
}

int main(int argc, char **argv) {
    uint64_t *words;
    size_t length, split;
    if (!sequence_input(argc, argv, "400", &words, &length, &split))
        return bench_usage("usage: embedding-pairs <left-count> [values...] | <left-count> --generate [total-count [seed]]");
    if (argc > 2 && strcmp(argv[2], "--generate") == 0)
        for (size_t i = 0; i < length; ++i) words[i] %= UINT64_C(4);

    const uint64_t *right = split ? words + split : words;
    uint64_t answer = embedding_pairs(words, split, right, length - split);
    printf("%" PRIu64 "\n", answer);
    free(words);
    return 0;
}

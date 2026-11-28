#include "sequence-input.h"

struct token {
    uint64_t label;
    uint64_t gap;
};

static uint64_t cheaper(uint64_t a, uint64_t b) {
    return a > b ? b : a;
}

static uint64_t alignment_cost(const struct token *left, size_t n,
                               const struct token *right, size_t m) {
    if (n == 0) {
        if (m == 0) return 0;
        return right->gap + alignment_cost(left, n, right + 1, m - 1);
    }
    if (m == 0)
        return left->gap + alignment_cost(left + 1, n - 1, right, m);

    uint64_t drop_left = left->gap + alignment_cost(left + 1, n - 1, right, m);
    uint64_t drop_right = right->gap + alignment_cost(left, n, right + 1, m - 1);
    uint64_t toll = left->label == right->label ? UINT64_C(0)
        : UINT64_C(2) + (left->gap * right->gap) % UINT64_C(5);
    uint64_t diagonal = toll + alignment_cost(left + 1, n - 1, right + 1, m - 1);
    return cheaper(diagonal, cheaper(drop_left, drop_right));
}

int main(int argc, char **argv) {
    const char *usage = "usage: weighted-alignment <left-count> [values...] | <left-count> --generate [total-count [seed]]";
    /* Fill this program's own generator defaults before the shared parser. */
    char *filled[5];
    if ((argc == 3 || argc == 4) && strcmp(argv[2], "--generate") == 0) {
        filled[0] = argv[0];
        filled[1] = argv[1];
        filled[2] = argv[2];
        filled[3] = argc == 4 ? argv[3] : "361";
        filled[4] = "43";
        argv = filled;
        argc = 5;
    }
    uint64_t *words;
    size_t length, split;
    if (!sequence_input(argc, argv, "361", &words, &length, &split))
        return bench_usage(usage);

    struct token *tokens = NULL;
    if (length <= SIZE_MAX / sizeof(*tokens) && length)
        tokens = malloc(length * sizeof(*tokens));
    if (length && !tokens) {
        free(words);
        return bench_usage(usage);
    }
    for (size_t i = 0; i < length; ++i) {
        tokens[i].label = words[i] % UINT64_C(4);
        tokens[i].gap = UINT64_C(1) + (words[i] >> 2) % UINT64_C(7);
    }
    free(words);

    const struct token *right = split ? tokens + split : tokens;
    uint64_t answer = alignment_cost(tokens, split, right, length - split);
    printf("%" PRIu64 "\n", answer);
    free(tokens);
    return 0;
}

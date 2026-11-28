#include "sequence-input.h"

static void convolution(const uint64_t *left, size_t n, const uint64_t *right, size_t m,
                        uint64_t *restrict coefficients) {
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < m; ++j)
            coefficients[i + j] += left[i] * right[j];
}

int main(int argc, char **argv) {
    int show = argc > 1 && strcmp(argv[1], "--coefficients") == 0;
    uint64_t *words;
    size_t length, split;
    if (!sequence_input(argc - show, argv + show, "2048", &words, &length, &split))
        return bench_usage("usage: convolution [--coefficients] <left-count> [values... | --generate [count [seed]]]");

    size_t count = split && split < length ? length - 1 : 0;
    uint64_t *coefficients = NULL;
    if (count) {
        coefficients = calloc(count, sizeof(*coefficients));
        if (!coefficients) {
            perror("coefficient allocation");
            free(words);
            return 1;
        }
        convolution(words, split, words + split, length - split, coefficients);
    }
    if (show) {
        for (size_t i = 0; i < count; ++i) {
            if (i) putchar(' ');
            printf("%" PRIu64, coefficients[i]);
        }
        putchar('\n');
    } else {
        uint64_t digest = UINT64_C(14695981039346656037);
        for (size_t i = 0; i < count; ++i) digest = bench_sample(digest ^ coefficients[i]);
        printf("%" PRIu64 "\n", digest);
    }
    free(coefficients);
    free(words);
    return 0;
}

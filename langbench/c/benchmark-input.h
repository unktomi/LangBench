/* Shared command-line input support matching lang/BenchmarkInput.lang.
 * Programs supply their own defaults for optional generator arguments.
 */
#ifndef LANGBENCH6_BENCHMARK_INPUT_H
#define LANGBENCH6_BENCHMARK_INPUT_H

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* integer_parse_digits integer_u64 radix_decimal UINT64_MAX false text 0:
 * require at least one ASCII decimal digit and reject signs, whitespace,
 * separators, suffixes, and overflow before multiplying the accumulator.
 */
static inline int bench_parse(const char *text, uint64_t *result) {
    if (!text || !*text) return 0;
    uint64_t value = 0;
    for (const unsigned char *p = (const unsigned char *)text; *p; ++p) {
        if (*p < '0' || *p > '9') return 0;
        uint64_t digit = (uint64_t)(*p - '0');
        if (value > (UINT64_MAX - digit) / UINT64_C(10)) return 0;
        value = value * UINT64_C(10) + digit;
    }
    *result = value;
    return 1;
}

static inline uint64_t bench_sample(uint64_t seed) {
    uint64_t a = (seed ^ (seed >> 30)) * UINT64_C(13787848793156543929);
    uint64_t b = (a ^ (a >> 27)) * UINT64_C(10723151780598845931);
    return b ^ (b >> 31);
}

/* Preserve the Lang list's order. An absent tail is an empty input. The
 * --generate form requires exactly count and seed, with count <= 1048576.
 * Every generator addition/multiplication uses Uint64 wrapping semantics.
 */
static inline int bench_input(int argc, char **argv, int start,
                              uint64_t **result, size_t *length) {
    *result = NULL;
    *length = 0;
    if (start < 0) return 0;
    if (start >= argc) return 1;

    uint64_t count, seed = 0;
    int generated = strcmp(argv[start], "--generate") == 0;
    if (generated) {
        if (argc - start != 3 || !bench_parse(argv[start + 1], &count)
            || count > UINT64_C(1048576)
            || !bench_parse(argv[start + 2], &seed)) return 0;
    } else {
        count = (uint64_t)(argc - start);
    }
    if (count > SIZE_MAX / sizeof(uint64_t)) return 0;
    uint64_t *values = count ? malloc((size_t)count * sizeof(*values)) : NULL;
    if (count && !values) return 0;
    for (size_t i = 0; i < (size_t)count; ++i) {
        if (generated) {
            seed += UINT64_C(11400714819323198485);
            values[i] = bench_sample(seed);
        } else if (!bench_parse(argv[start + (int)i], &values[i])) {
            free(values);
            return 0;
        }
    }
    *result = values;
    *length = (size_t)count;
    return 1;
}

static inline int bench_usage(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    return 2;
}

#endif

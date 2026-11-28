/* Word-frequency count over a tiny vocab — exercises hashing + counting. */
#include "Osprey.Repeat.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MINSTD 16807
#define MODULUS 2147483647

static int64_t r1(int64_t x) { return (x * MINSTD) % MODULUS; }

static int64_t hash_at(int64_t s, int64_t i) { return r1(r1(s + i)); }

static int64_t read_seed(void) {
    char line[256];
    int64_t m = 0;
    if (fgets(line, sizeof line, stdin) != NULL) {
        char *end = NULL;
        long long parsed = strtoll(line, &end, 10);
        if (end != line) {
            m = (int64_t)parsed;
        }
    }
    if (m == 0) {
        return 1;
    }
    uint64_t val = 0;
    FILE *f = fopen("/dev/urandom", "rb");
    if (f != NULL) {
        if (fread(&val, sizeof val, 1, f) != 1) {
            val = 0;
        }
        fclose(f);
    }
    return (int64_t)(val % 2147483646) + 1;
}

static int64_t benchmark(int64_t seed) {
    const char *vocab[8] = {"the", "quick", "brown", "fox",
                            "jumps", "over", "lazy", "dog"};
    (void)vocab;

    int64_t counts[8] = {0};
    for (int64_t i = 0; i < 200000; i++) {
        counts[hash_at(seed, i) % 8] += 1;
    }

    int64_t result = 0;
    for (int64_t k = 0; k < 8; k++) {
        result += (k + 1) * counts[k];
    }

    return result;
}

int main(int argc, char **argv) {
    const uint64_t reps = osprey_repetitions(argc, argv);
    if (!reps) return 2;
    const int64_t seed = read_seed();
    uint64_t digest = 0;
    for (uint64_t i = 0; i < reps; ++i)
        digest = digest * UINT64_C(1099511628211) + (uint64_t)benchmark(seed + (int64_t)i);
    printf("%" PRIu64 "\n", digest);
    return 0;
}

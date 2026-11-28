#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
extern void fibers_setup(int64_t);
extern int32_t benchmark(int64_t);
extern void fibers_teardown(void);
int main(int argc, char **argv) {
    int64_t count = 8388608;  /* default: the benchmark case */
    if (argc > 2) return 2;
    if (argc == 2) {
        char *end;
        if (!*argv[1]) return 2;
        count = strtoll(argv[1], &end, 10);
        if (*end || count < 0 || count > 10000000) return 2;
    }
    fibers_setup(count);
    int32_t answer = benchmark(count);
    fibers_teardown();
    printf("%" PRId32 "\n", answer);
    return 0;
}

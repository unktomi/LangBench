#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Ordinary tagged union corresponding to Fibers.lang. Storage is reserved
 * before timing; construction and the order-sensitive fold are timed. */
typedef struct Row {
    enum { SINGLE, MIXED } tag;
    union {
        uint32_t single;
        struct { uint32_t word; double fraction; } mixed;
    } payload;
} Row;

static Row *rows;
static int64_t prepared_count;
static size_t storage_bytes;

#ifdef FIBERS_TRACK
#include <stdio.h>
static size_t allocations, frees;
#endif

void fibers_setup(int64_t count) {
    prepared_count = count;
    if (count <= 0) return;
    if ((uint64_t)count > SIZE_MAX / sizeof(Row)) abort();
    storage_bytes = (size_t)count * sizeof(Row);
    rows = malloc(storage_bytes);
    if (!rows) abort();
#ifdef FIBERS_TRACK
    ++allocations;
#endif
}

void fibers_teardown(void) {
    if (rows) {
        free(rows);
#ifdef FIBERS_TRACK
        ++frees;
#endif
    }
    rows = NULL;
    storage_bytes = 0;
}

static uint32_t build(Row *output, int64_t count) {
    uint32_t coefficient = 7;
    for (int64_t index = 0; index < count; ++index) {
        uint32_t word = (uint32_t)index * 7919u - 97u;
        /* The Lang builder prepends each constructor. */
        Row *row = &output[count - 1 - index];
        if (index % 3 == 0) {
            row->tag = SINGLE;
            row->payload.single = word;
        } else {
            row->tag = MIXED;
            row->payload.mixed.word = word;
            row->payload.mixed.fraction = index % 2 == 0 ? -0.5 : 0.25;
        }
        coefficient ^= word;
    }
    return coefficient;
}

static uint32_t consume(uint32_t coefficient, const Row *input, int64_t count) {
    uint32_t result = 17;
    /* Right fold in list order. Unsigned arithmetic implements Int32 wrapping. */
    for (int64_t index = count; index > 0;) {
        const Row *row = &input[--index];
        switch (row->tag) {
        case SINGLE:
            result = row->payload.single * coefficient - result;
            break;
        case MIXED:
            result = row->payload.mixed.word * 3u - result
                + (row->payload.mixed.fraction < 0.0 ? coefficient : 0u - coefficient);
            break;
        }
    }
    return result;
}

int32_t benchmark(int64_t count) {
    if (count > prepared_count && count > 0) abort();
    uint32_t result = consume(build(rows, count), rows, count);
    int32_t answer;
    memcpy(&answer, &result, sizeof answer);
    return answer;
}

/* Separate instrumentation executable; no counters enter the timed object. */
#ifdef FIBERS_TRACK
int main(int argc, char **argv) {
    if (argc != 2) return 2;
    int64_t count = strtoll(argv[1], NULL, 10);
    fibers_setup(count);
    size_t bytes = storage_bytes, setup_allocations = allocations, setup_frees = frees;
    int32_t answer = 0;
    for (int call = 0; call < 3; ++call) answer = benchmark(count);
    size_t workload_allocations = allocations - setup_allocations;
    size_t workload_frees = frees - setup_frees;
    fibers_teardown();
    printf("{\"answer\":\"%d\",\"setupAllocations\":%zu,\"workloadAllocations\":%zu,"
           "\"workloadFrees\":%zu,\"frees\":%zu,\"allocatedBytes\":%zu,\"peakBytes\":%zu,\"liveBytes\":%zu}\n",
           answer, setup_allocations, workload_allocations, workload_frees, frees, bytes, bytes, storage_bytes);
    return 0;
}
#endif

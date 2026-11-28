#ifndef SKEPTIC_SEQUENCE_INPUT_H
#define SKEPTIC_SEQUENCE_INPUT_H

#include "../../langbench/c/benchmark-input.h"

/* Fill the optional generator arguments before using the shared parser. */
static inline int sequence_input(int argc, char **argv, char *default_count,
                                 uint64_t **values, size_t *length, size_t *split) {
    uint64_t requested;
    if (argc < 2 || !bench_parse(argv[1], &requested)) return 0;
    if ((argc == 3 || argc == 4) && strcmp(argv[2], "--generate") == 0) {
        char *filled[] = {argv[0], argv[1], argv[2], argc == 4 ? argv[3] : default_count, "42"};
        if (!bench_input(5, filled, 2, values, length)) return 0;
    } else if (!bench_input(argc, argv, 2, values, length)) {
        return 0;
    }
    *split = requested < *length ? (size_t)requested : *length;
    return 1;
}

#endif

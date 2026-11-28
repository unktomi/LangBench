/* Shared decimal CLI parsing for the C Osprey workload parameters.
 * Preserve the pinned references' signed 64-bit arithmetic domain. */
#ifndef OSPREY_C_ARGUMENTS_H
#define OSPREY_C_ARGUMENTS_H
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int osprey_arguments(int argc, char **argv, int count, int64_t *values) {
    if (argc != count + 1) goto invalid;
    for (int i = 0; i < count; ++i) {
        const char *text = argv[i + 1];
        if (!*text) goto invalid;
        for (const char *p = text; *p; ++p)
            if (*p < '0' || *p > '9') goto invalid;
        errno = 0;
        char *end;
        uintmax_t value = strtoumax(text, &end, 10);
        if (errno || *end || value > INT64_MAX) goto invalid;
        values[i] = (int64_t)value;
    }
    return 1;
invalid:
    fprintf(stderr, "usage: %s", argv[0]);
    for (int i = 0; i < count; ++i) fprintf(stderr, " <parameter-%d>", i + 1);
    fputs(" (nonnegative signed 64-bit decimal integers)\n", stderr);
    return 0;
}
#endif

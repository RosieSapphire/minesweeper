#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "error.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"

void _assertf_internal(const bool cond,
                       const char *const __restrict cond_str,
                       const char *const __restrict fmt,
                       ...)
{
        va_list args;

        if (cond)
                return;

        assert(cond_str);
        assert(fmt);

        va_start(args, fmt);

        (void)fprintf(stderr, "ASSERTION (%s) FAILED:\n\t", cond_str);
        (void)vfprintf(stderr, fmt, args);
        (void)fprintf(stderr, "\n");

        va_end(args);

        exit(EXIT_FAILURE);
}

#pragma clang diagnostic pop

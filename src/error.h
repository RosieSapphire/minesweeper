#ifndef __ERROR_H
#define __ERROR_H

/************
 * INCLUDES *
 ************/

#include <stdbool.h>

/**********
 * MACROS *
 **********/

#ifndef NDEBUG
        #define assertf(_cond, ...) \
                _assertf_internal(!!(_cond), #_cond, __VA_ARGS__)
#else /* #ifndef NDEBUG */
        #define assertf(_cond, ...) ((void)0)
#endif /* #ifndef NDEBUG #else */

/******************************
 * PUBLIC FUNCTION PROTOTYPES *
 ******************************/

extern void _assertf_internal(const bool cond,
                              const char *const __restrict cond_str,
                              const char *const __restrict fmt,
                              ...);

#endif /* #ifndef __ERROR_H */

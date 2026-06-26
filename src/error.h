#ifndef __ERROR_H
#define __ERROR_H

/************
 * INCLUDES *
 ************/

#include <stdbool.h>

/**********
 * MACROS *
 **********/

#ifdef DEBUG
        #define assertf(_cond, ...) \
                _assertf_internal(!!(_cond), #_cond, __VA_ARGS__)
#else /* #ifdef DEBUG */
        #define assertf(_cond, ...) ((void)0)
#endif /* #ifdef DEBUG #else */

/******************************
 * PUBLIC FUNCTION PROTOTYPES *
 ******************************/

#ifdef DEBUG
extern void _assertf_internal(const bool cond,
                              const char *const __restrict cond_str,
                              const char *const __restrict fmt,
                              ...);
#endif /* #ifdef DEBUG */

#endif /* #ifndef __ERROR_H */

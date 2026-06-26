#ifndef __ERROR_H
#define __ERROR_H

/************
 * INCLUDES *
 ************/

#include <stdbool.h>

/**********
 * MACROS *
 **********/

#define assertf(_cond, ...) _assertf_internal(!!(_cond), #_cond, __VA_ARGS__)

/******************************
 * PUBLIC FUNCTION PROTOTYPES *
 ******************************/

extern void _assertf_internal(const bool cond,
                              const char *const __restrict cond_str,
                              const char *const __restrict fmt,
                              ...);

#endif /* #ifndef __ERROR_H */

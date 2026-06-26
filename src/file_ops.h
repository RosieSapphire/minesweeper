#ifndef __FILE_OPS_H
#define __FILE_OPS_H

/*
 * Just a way to manage files while being safer about it; such as 
 * automatic error handling and all that fancy fucken' shit! :D
 */

#include <stdio.h>

extern FILE  *fopen_check(const char *const __restrict path,
                          const char *const __restrict mode);
extern void   fseek_check(FILE *const fp, const int off, const int whence);
extern size_t ftell_check(FILE *const fp);
extern void   fclose_check(FILE *const fp);

#endif /* #ifndef __FILE_OPS_H */

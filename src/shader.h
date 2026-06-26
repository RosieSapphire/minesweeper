#ifndef __SHADER_H
#define __SHADER_H

#include <stdint.h>

extern uint32_t shader_load(const char *const __restrict vpath,
                            const char *const __restrict fpath);
extern void     shader_unload(const uint32_t shader);

#endif /* #ifndef __SHADER_H */

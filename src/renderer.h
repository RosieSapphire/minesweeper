#ifndef __RENDERER_H
#define __RENDERER_H

/************
 * INCLUDES *
 ************/

#include "window.h"

/***********
 * DEFINES *
 ***********/

#define REND_FLAGS_NONE   (0u)
#define REND_FLAG_IS_INIT (1u << REND_FLAG_IT_INIT_SHIFT)

/*********
 * ENUMS *
 *********/

enum {
        REND_FLAG_IT_INIT_SHIFT = 0,
};

/**************
 * STRUCTURES *
 **************/

struct renderer {
        float    clear_col[4];
        uint32_t flags;
};

extern void renderer_init(struct renderer *const restrict ren,
                          const struct window *const restrict wnd);
extern void renderer_terminate(struct renderer *const ren);

extern uint32_t shader_load(const char *const restrict vpath,
                            const char *const restrict fpath);
extern void     shader_unload(uint32_t *const id);

extern uint32_t texture_load(const char *const path);
extern void     texture_unload(uint32_t *const id);

extern void renderer_clear(const float r,
                           const float g,
                           const float b,
                           const float a,
                           const bool  depth);
extern void renderer_display_to_window(
        const struct window *const restrict wnd,
        const struct renderer *const restrict rnd);

#endif /* #ifndef __RENDERER_H */

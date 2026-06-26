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
        uint32_t flags;
};

extern void renderer_init(struct renderer *const ren);
extern void renderer_terminate(struct renderer *const ren);

extern void renderer_clear(const float r,
                           const float g,
                           const float b,
                           const float a,
                           const bool  depth);
extern void renderer_display_to_window(
        const struct window *const restrict wnd,
        const struct renderer *const restrict rnd);

#endif /* #ifndef __RENDERER_H */

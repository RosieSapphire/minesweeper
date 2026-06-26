#ifndef TILE_H
#define TILE_H

/************
 * INCLUDES *
 ************/

#include "window.h"
#include "input.h"
#include "renderer.h"

/***********
 * DEFINES *
 ***********/

#define TILES_X   16u
#define TILES_Y   16u
#define TILE_SIZE 48u

#define TILE_FLAGS_NONE       (0u)
#define TILE_FLAG_HAS_BOMB    (1u << TILE_FLAG_HAS_BOMB_SHIFT)
#define TILE_FLAG_IS_FLAGGED  (1u << TILE_FLAG_IS_FLAGGED_SHIFT)
#define TILE_FLAG_IS_REVEALED (1u << TILE_FLAG_IS_REVEALED_SHIFT)

/*********
 * ENUMS *
 *********/

enum {
        TILE_FLAG_HAS_BOMB_SHIFT = 0,
        TILE_FLAG_IS_FLAGGED_SHIFT,
        TILE_FLAG_IS_REVEALED_SHIFT,
};

/******************************
 * PUBLIC FUNCTION PROTOTYPES *
 ******************************/

extern void tiles_init(struct renderer *const rnd);
extern void tiles_update(const struct window *const wnd,
                         const struct input         inp);
extern void tiles_draw(const struct renderer *const restrict rnd,
                       const struct window *const restrict wnd);
extern void tiles_terminate(struct renderer *const rnd);

#endif

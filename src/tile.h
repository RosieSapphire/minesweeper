#ifndef __TILE_H
#define __TILE_H

/************
 * INCLUDES *
 ************/

#include "window.h"
#include "input.h"

/***********
 * DEFINES *
 ***********/

#define TILES_X   16u
#define TILES_Y   16u
#define TILE_SIZE 48u

#define TILES_NONE       (0u)
#define TILE_HAS_BOMB    (1u << TILE_HAS_BOMB_SHIFT)
#define TILE_IS_FLAGGED  (1u << TILE_IS_FLAGGED_SHIFT)
#define TILE_IS_REVEALED (1u << TILE_IS_REVEALED_SHIFT)

/*********
 * ENUMS *
 *********/

enum {
        TILE_HAS_BOMB_SHIFT = 0,
        TILE_IS_FLAGGED_SHIFT,
        TILE_IS_REVEALED_SHIFT,
};

/******************************
 * PUBLIC FUNCTION PROTOTYPES *
 ******************************/

extern void tiles_init(void);
extern void tiles_update(const struct window *const wnd,
                         const struct input         inp);
extern void tiles_draw(const uint32_t shd,
                       const uint32_t wnd_wid,
                       const uint32_t wnd_hei);
extern void tiles_terminate(void);

#endif /* #ifndef __TILE_H */

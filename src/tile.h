#ifndef TILE_H
#define TILE_H

/************
 * INCLUDES *
 ************/

#include <stdint.h>
#include <stdbool.h>

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

extern void tiles_init(void);
extern void tiles_update(const uint16_t win_wid, const uint16_t win_hei);
extern void tiles_draw(const uint16_t win_wid, const uint16_t win_hei);
extern void tiles_terminate(void);

#endif

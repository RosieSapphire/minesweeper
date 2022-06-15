#ifndef MINEFIELD_H
#define MINEFIELD_H

#include "buffer.h"
#include <stdbool.h>

#define TILE_SIZE		32

#define TILE_DARK		sfColor_fromRGB(60, 60, 60)
#define TILE_MID		sfColor_fromRGB(140, 140, 140)
#define TILE_LIGHT		sfColor_fromRGB(210, 210, 210)

#define FIELD_WIDTH		20
#define FIELD_HEIGHT	16
#define FIELD_DIMENSION	(FIELD_WIDTH * FIELD_HEIGHT)

typedef enum {
	TS_HIDDEN,
	TS_REVEALED,
	TS_FLAGGED
} tile_state_t;

typedef struct {
    vec2u_t pos;
    tile_state_t st;
    bool has_mine;/* = false; */
} tile_t;

void tile_draw(const tile_t t, buffer_t *b);

typedef struct {
    tile_t tiles[FIELD_DIMENSION];
    vec2u_t screen_pos;
} minefield_t;

minefield_t minefield_create(const vec2u_t pos, const uint32_t mines);
void minefield_draw(minefield_t *m, buffer_t *b);

#endif

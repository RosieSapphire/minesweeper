#ifndef MINEFIELD_H
#define MINEFIELD_H

#include "buffer.h"

#define TILE_SIZE		32

#define TILE_DARK		{60, 60, 60}
#define TILE_MID		{140, 140, 140}
#define TILE_LIGHT		{210, 210, 210}

#define FIELD_WIDTH		20
#define FIELD_HEIGHT	16
#define FIELD_DIMENSION	(FIELD_WIDTH * FIELD_HEIGHT)

enum tile_state_t {
	TS_HIDDEN,
	TS_REVEALED,
	TS_FLAGGED
};

struct tile_t {
    vec2u_t pos;
    tile_state_t st;
    bool has_mine = false;
};

void tile_draw(const tile_t t, buffer_t *b);

struct minefield_t {
    tile_t tiles[FIELD_DIMENSION];
    vec2u_t screen_pos;

};

minefield_t minefield_create(const vec2u_t &pos, const uint32_t &mines);
void minefield_draw(minefield_t *m, buffer_t *b);

#endif

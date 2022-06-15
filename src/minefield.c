#include <stdlib.h>
#include <time.h>

#include "minefield.h"

void tile_draw(const tile_t t, buffer_t *b) {
    const uint32_t edge_size = t.st == TS_REVEALED ? TILE_SIZE / 12 : TILE_SIZE / 8;
    const uint32_t all_but_edge = TILE_SIZE - edge_size;

    switch (t.st) {
        case TS_REVEALED:
            buffer_draw_rect(b, t.pos, vec2u_create(TILE_SIZE, edge_size), TILE_DARK);
            buffer_draw_rect(b, vec2u_create(t.pos.x, t.pos.y + all_but_edge), vec2u_create(TILE_SIZE, edge_size), TILE_DARK);
            buffer_draw_rect(b, vec2u_create(t.pos.x, t.pos.y + edge_size), vec2u_create(edge_size, all_but_edge), TILE_DARK);
            buffer_draw_rect(b, vec2u_create(t.pos.x + all_but_edge, t.pos.y + edge_size), vec2u_create(edge_size, all_but_edge), TILE_DARK);

            buffer_draw_rect(b, vec2u_add(t.pos, vec2u_create_both(edge_size)), vec2u_create_both(TILE_SIZE - (edge_size * 2)), TILE_MID);

            if (t.has_mine)
				buffer_draw_rect(b, vec2u_add(t.pos, vec2u_create_both(edge_size * 4)), vec2u_create_both(TILE_SIZE - (edge_size * 8)), sfBlack);

            break;
        case TS_HIDDEN:
            buffer_draw_rect(b, t.pos, vec2u_create(edge_size, all_but_edge), TILE_DARK);
            buffer_draw_rect(b, vec2u_create(t.pos.x, t.pos.y + all_but_edge), vec2u_create(TILE_SIZE, edge_size), TILE_DARK);

            buffer_draw_tri_tr(b, t.pos, edge_size, TILE_LIGHT);
            buffer_draw_rect(b, vec2u_create(t.pos.x + edge_size, t.pos.y), vec2u_create(all_but_edge, edge_size), TILE_LIGHT);
            buffer_draw_rect(b, vec2u_create(t.pos.x + all_but_edge, t.pos.y + edge_size), vec2u_create(edge_size, all_but_edge - edge_size), TILE_LIGHT); buffer_draw_tri_tr(b, vec2u_create(t.pos.x + all_but_edge, t.pos.y + all_but_edge), edge_size, TILE_LIGHT);

            buffer_draw_rect(b, vec2u_create(t.pos.x + edge_size, t.pos.y + edge_size), vec2u_create_both(all_but_edge - edge_size), TILE_MID);
            break;

        case TS_FLAGGED:
            buffer_draw_rect(b, t.pos, vec2u_create(edge_size, all_but_edge), TILE_DARK);
            buffer_draw_rect(b, vec2u_create(t.pos.x, t.pos.y + all_but_edge), vec2u_create(TILE_SIZE, edge_size), TILE_DARK);

            buffer_draw_tri_tr(b, t.pos, edge_size, TILE_LIGHT);
            buffer_draw_rect(b, vec2u_create(t.pos.x + edge_size, t.pos.y), vec2u_create(all_but_edge, edge_size), TILE_LIGHT);
            buffer_draw_rect(b, vec2u_create(t.pos.x + all_but_edge, t.pos.y + edge_size), vec2u_create(edge_size, all_but_edge - edge_size), TILE_LIGHT);
            buffer_draw_tri_tr(b, vec2u_create(t.pos.x + all_but_edge, t.pos.y + all_but_edge), edge_size, TILE_LIGHT);

            buffer_draw_rect(b, vec2u_create(t.pos.x + edge_size, t.pos.y + edge_size), vec2u_create_both(all_but_edge - edge_size), TILE_MID);
            buffer_draw_tri_br(b, vec2u_add(t.pos, vec2u_create_both(edge_size * 2)), TILE_SIZE - (edge_size * 4), sfRed);
            break;
    }    
}

minefield_t minefield_create(const vec2u_t pos, const uint32_t mines) {
	uint32_t x, y, i;
	minefield_t m;
	m.screen_pos = pos;

    for (y = 0; y < FIELD_HEIGHT; y++) {
        for (x = 0; x < FIELD_WIDTH; x++) {
            m.tiles[y * FIELD_WIDTH + x].pos = vec2u_add(m.screen_pos, vec2u_create(x * TILE_SIZE, y * TILE_SIZE));
            m.tiles[y * FIELD_WIDTH + x].st = TS_HIDDEN;
        }
	}

	srand((uint32_t)time(NULL));

    for (i = 0; i < mines; i++) {
        vec2u_t spawn_pos;
        tile_t *current = NULL;

        do {
            spawn_pos = vec2u_create(rand() % FIELD_WIDTH, rand() % FIELD_HEIGHT);
            current = &m.tiles[spawn_pos.y * FIELD_WIDTH + spawn_pos.x];
        } while (current->has_mine);

        current->has_mine = true;
    }

	return m;
}

void minefield_draw(minefield_t *m, buffer_t *b) {
	uint32_t x, y;
    for (y = 0; y < FIELD_HEIGHT; y++) {
        for (x = 0; x < FIELD_WIDTH; x++) {
			tile_draw(m->tiles[y * FIELD_WIDTH + x], b);
        }
	}
}

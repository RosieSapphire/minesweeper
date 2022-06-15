#include <random>

#include "minefield.h"

void tile_draw(const tile_t t, buffer_t *b) {
    const uint32_t edge_size = t.st == TS_REVEALED ? TILE_SIZE / 12 : TILE_SIZE / 8;
    const uint32_t all_but_edge = TILE_SIZE - edge_size;

    switch (t.st) {
        case TS_REVEALED:
            buffer_draw_rect(b, t.pos, {TILE_SIZE, edge_size}, TILE_DARK);
            buffer_draw_rect(b, {t.pos.x, t.pos.y + all_but_edge}, {TILE_SIZE, edge_size}, TILE_DARK);
            buffer_draw_rect(b, {t.pos.x, t.pos.y + edge_size}, {edge_size, all_but_edge}, TILE_DARK);
            buffer_draw_rect(b, {t.pos.x + all_but_edge, t.pos.y + edge_size}, {edge_size, all_but_edge}, TILE_DARK);

            buffer_draw_rect(b, vec2u_add(t.pos, vec2u_create_both(edge_size)), vec2u_create_both(TILE_SIZE - (edge_size * 2)), TILE_MID);

            if (t.has_mine)
				buffer_draw_rect(b, vec2u_add(t.pos, vec2u_create_both(edge_size * 4)), vec2u_create_both(TILE_SIZE - (edge_size * 8)), sf::Color::Black);

            break;
        case TS_HIDDEN:
            buffer_draw_rect(b, t.pos, {edge_size, all_but_edge}, TILE_DARK);
            buffer_draw_rect(b, {t.pos.x, t.pos.y + all_but_edge}, {TILE_SIZE, edge_size}, TILE_DARK);

            buffer_draw_tri_tr(b, t.pos, edge_size, TILE_LIGHT);
            buffer_draw_rect(b, {t.pos.x + edge_size, t.pos.y}, {all_but_edge, edge_size}, TILE_LIGHT);
            buffer_draw_rect(b, {t.pos.x + all_but_edge, t.pos.y + edge_size}, {edge_size, all_but_edge - edge_size}, TILE_LIGHT);
            buffer_draw_tri_tr(b, {t.pos.x + all_but_edge, t.pos.y + all_but_edge}, edge_size, TILE_LIGHT);

            buffer_draw_rect(b, {t.pos.x + edge_size, t.pos.y + edge_size}, vec2u_create_both(all_but_edge - edge_size), TILE_MID);
            break;

        case TS_FLAGGED:
            buffer_draw_rect(b, t.pos, {edge_size, all_but_edge}, TILE_DARK);
            buffer_draw_rect(b, {t.pos.x, t.pos.y + all_but_edge}, {TILE_SIZE, edge_size}, TILE_DARK);

            buffer_draw_tri_tr(b, t.pos, edge_size, TILE_LIGHT);
            buffer_draw_rect(b, {t.pos.x + edge_size, t.pos.y}, {all_but_edge, edge_size}, TILE_LIGHT);
            buffer_draw_rect(b, {t.pos.x + all_but_edge, t.pos.y + edge_size}, {edge_size, all_but_edge - edge_size}, TILE_LIGHT);
            buffer_draw_tri_tr(b, {t.pos.x + all_but_edge, t.pos.y + all_but_edge}, edge_size, TILE_LIGHT);

            buffer_draw_rect(b, {t.pos.x + edge_size, t.pos.y + edge_size}, vec2u_create_both(all_but_edge - edge_size), TILE_MID);
            buffer_draw_tri_br(b, vec2u_add(t.pos, vec2u_create_both(edge_size * 2)), {TILE_SIZE - (edge_size * 4)}, sf::Color::Red);
            break;
    }    
}

minefield_t minefield_create(const vec2u_t &pos, const uint32_t &mines) {
	minefield_t m;
	m.screen_pos = pos;

    for (uint32_t y = 0; y < FIELD_HEIGHT; y++) {
        for (uint32_t x = 0; x < FIELD_WIDTH; x++) {
            m.tiles[y * FIELD_WIDTH + x].pos = vec2u_add(m.screen_pos, vec2u_create(x * TILE_SIZE, y * TILE_SIZE));
            m.tiles[y * FIELD_WIDTH + x].st = TS_HIDDEN;
        }
    }  

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<uint32_t> xrand(0, FIELD_WIDTH - 1);
    std::uniform_int_distribution<uint32_t> yrand(0, FIELD_HEIGHT - 1);

    for (uint32_t i = 0; i < mines; i++) {        
        vec2u_t spawn_pos;
        tile_t *current = nullptr;

        do {
            spawn_pos = {xrand(rng), yrand(rng)};
            current = &m.tiles[spawn_pos.y * FIELD_WIDTH + spawn_pos.x];
        } while (current->has_mine);

        current->has_mine = true;
    }

	return m;
}

void minefield_draw(minefield_t *m, buffer_t *b) {
    for (uint32_t y = 0; y < FIELD_HEIGHT; y++) {
        for (uint32_t x = 0; x < FIELD_WIDTH; x++) {
			tile_draw(m->tiles[y * FIELD_WIDTH + x], b);
        }
    }        
}

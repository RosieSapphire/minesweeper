#include "buffer.h"

#include <assert.h>
#include <stdint.h>

void buffer_clear(buffer_t *b, const sfColor color) {
	uint32_t x, y;
    for(x = 0; x < BUFFER_WIDTH; x++) {
        for(y = 0; y < BUFFER_HEIGHT; y++) {
            buffer_put_pixel(b, vec2u_create(x, y), color);
        }
    }
}

void buffer_put_pixel(buffer_t *b, const vec2u_t pos, const sfColor color) {
	sfVertex *current_pixel;
    assert(pos.x >= 0);
    assert(pos.x < BUFFER_WIDTH);
    assert(pos.y >= 0);
    assert(pos.y < BUFFER_HEIGHT);

    current_pixel = sfVertexArray_getVertex(b->vertex_array, pos.y * BUFFER_WIDTH + pos.x);
    current_pixel->position.x = (float)pos.x;
    current_pixel->position.y = (float)pos.y;
    current_pixel->color = color;
}

void buffer_draw_rect(buffer_t *b, const vec2u_t pos, const vec2u_t size, const sfColor color) {
	uint32_t x, y;
    for(x = pos.x; x < pos.x + size.x; x++) {
        for(y = pos.y; y < pos.y + size.y; y++) {
            buffer_put_pixel(b, vec2u_create(x, y), color);
        }
    }
}

void buffer_draw_tri_tl(buffer_t *b, const vec2u_t pos, const uint32_t size, const sfColor color) {
	uint32_t x, y;
    uint32_t current_length = size;
    for(y = 0; y < size; y++) {
        for(x = 0; x < current_length; x++) {
            buffer_put_pixel(b, vec2u_create(pos.x + x, pos.y + y), color);
        }

        current_length--;
    }
}

void buffer_draw_tri_tr(buffer_t *b, const vec2u_t pos, const uint32_t size, const sfColor color) {
	uint32_t x, y;
    uint32_t current_length = size;
    for(y = 0; y < size; y++) {
        for(x = 0; x < current_length; x++) {
            buffer_put_pixel(b, vec2u_create(pos.x + (size - x) - 1, pos.y + y), color);
        }

        current_length--;
    }
}

void buffer_draw_tri_bl(buffer_t *b, const vec2u_t pos, const uint32_t size, const sfColor color) {
	uint32_t x, y;
    uint32_t current_length = 1;
    for(y = 0; y < size; y++) {
        for(x = 0; x < current_length; x++) {
            buffer_put_pixel(b, vec2u_create(pos.x + x, pos.y + y), color);
        }

        current_length++;
    }
}

void buffer_draw_tri_br(buffer_t *b, const vec2u_t pos, const uint32_t size, const sfColor color) {
	uint32_t x, y;
    uint32_t current_length = 1;
    for(y = 0; y < size; y++) {
        for(x = 0; x < current_length; x++) {
            buffer_put_pixel(b, vec2u_create(pos.x + (size - x) - 1, pos.y + y), color);
        }

        current_length++;
    }
}

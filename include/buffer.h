#ifndef BUFFER_H
#define BUFFER_H

#include <SFML/Graphics.h>
#include "vec2.h"

#define BUFFER_WIDTH		704
#define BUFFER_HEIGHT		576
#define BUFFER_DIMENSION	(BUFFER_WIDTH * BUFFER_HEIGHT)

typedef struct {
    sfVertexArray *vertex_array;
} buffer_t;

void buffer_clear(buffer_t *b, const sfColor color);
void buffer_put_pixel(buffer_t *b, const vec2u_t pos, const sfColor color);

void buffer_draw_rect(buffer_t *b, const vec2u_t pos, const vec2u_t size, const sfColor color);

void buffer_draw_tri_tl(buffer_t *b, const vec2u_t pos, const uint32_t size, const sfColor color);
void buffer_draw_tri_tr(buffer_t *b, const vec2u_t pos, const uint32_t size, const sfColor color);
void buffer_draw_tri_bl(buffer_t *b, const vec2u_t pos, const uint32_t size, const sfColor color);
void buffer_draw_tri_br(buffer_t *b, const vec2u_t pos, const uint32_t size, const sfColor color);

#endif

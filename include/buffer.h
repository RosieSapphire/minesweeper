#ifndef BUFFER_H
#define BUFFER_H

#include <SFML/Graphics.hpp>
#include "vec2.h"

#define BUFFER_WIDTH		704
#define BUFFER_HEIGHT		576
#define BUFFER_DIMENSION	(BUFFER_WIDTH * BUFFER_HEIGHT)

struct buffer_t {
    sf::VertexArray vertex_array;
};

void buffer_clear(buffer_t *b, const sf::Color &color = sf::Color::Black);
void buffer_put_pixel(buffer_t *b, const vec2u_t &pos, const sf::Color &color);

void buffer_draw_rect(buffer_t *b, const vec2u_t &pos, const vec2u_t &size, const sf::Color &color = sf::Color::White);

void buffer_draw_tri_tl(buffer_t *b, const vec2u_t &pos, const uint32_t &size, const sf::Color &color);
void buffer_draw_tri_tr(buffer_t *b, const vec2u_t &pos, const uint32_t &size, const sf::Color &color);
void buffer_draw_tri_bl(buffer_t *b, const vec2u_t &pos, const uint32_t &size, const sf::Color &color);
void buffer_draw_tri_br(buffer_t *b, const vec2u_t &pos, const uint32_t &size, const sf::Color &color);

#endif

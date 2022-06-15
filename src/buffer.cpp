#include "buffer.h"

#include <cassert>

void buffer_clear(buffer_t *b, const sf::Color &color) {
    for(unsigned int x = 0; x < BUFFER_WIDTH; x++) {
        for(unsigned int y = 0; y < BUFFER_HEIGHT; y++) {
            buffer_put_pixel(b, {static_cast<unsigned int>(x), static_cast<unsigned int>(y)}, color);
        }
    }
}

void buffer_put_pixel(buffer_t *b, const vec2u_t &pos, const sf::Color &color) {
    assert(pos.x >= 0);
    assert(pos.x < BUFFER_WIDTH);
    assert(pos.y >= 0);
    assert(pos.y < BUFFER_HEIGHT);

    sf::Vertex &current_pixel = b->vertex_array[pos.y * BUFFER_WIDTH + pos.x];
    current_pixel.position = {static_cast<float>(pos.x), static_cast<float>(pos.y)};
    current_pixel.color = color;
}

void buffer_draw_rect(buffer_t *b, const vec2u_t &pos, const vec2u_t &size, const sf::Color &color) {
    for(unsigned int x = pos.x; x < pos.x + size.x; x++) {
        for(unsigned int y = pos.y; y < pos.y + size.y; y++) {
            buffer_put_pixel(b, {static_cast<unsigned int>(x), static_cast<unsigned int>(y)}, color);
        }
    }
}

void buffer_draw_tri_tl(buffer_t *b, const vec2u_t &pos, const unsigned int &size, const sf::Color &color) {
    unsigned int current_length = size;
    for(unsigned int y = 0; y < size; y++) {
        for(unsigned int x = 0; x < current_length; x++) {
            buffer_put_pixel(b, {pos.x + x, pos.y + y}, color);
        }

        current_length--;
    }
}

void buffer_draw_tri_tr(buffer_t *b, const vec2u_t &pos, const unsigned int &size, const sf::Color &color) {
    unsigned int current_length = size;
    for(unsigned int y = 0; y < size; y++) {
        for(unsigned int x = 0; x < current_length; x++) {
            buffer_put_pixel(b, {pos.x + (size - x) - 1, pos.y + y}, color);
        }

        current_length--;
    }
}

void buffer_draw_tri_bl(buffer_t *b, const vec2u_t &pos, const unsigned int &size, const sf::Color &color) {
    unsigned int current_length = 1;
    for(unsigned int y = 0; y < size; y++) {
        for(unsigned int x = 0; x < current_length; x++) {
            buffer_put_pixel(b, {pos.x + x, pos.y + y}, color);
        }

        current_length++;
    }
}

void buffer_draw_tri_br(buffer_t *b, const vec2u_t &pos, const unsigned int &size, const sf::Color &color) {
    unsigned int current_length = 1;
    for(unsigned int y = 0; y < size; y++) {
        for(unsigned int x = 0; x < current_length; x++) {
            buffer_put_pixel(b, {pos.x + (size - x) - 1, pos.y + y}, color);
        }

        current_length++;
    }
}

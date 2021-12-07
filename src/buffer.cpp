#include "buffer.h"

#include <cassert>

buffer::buffer():
    array_(sf::Points, buffer_dim) {}

void buffer::clear(const sf::Color &color) {
    for (int x = 0; x < buffer_size.x; x++) {
        for (int y = 0; y < buffer_size.y; y++) {
            draw_pixel({static_cast<unsigned int>(x), static_cast<unsigned int>(y)}, color);
        }
    }
}

void buffer::draw_pixel(const vec2<unsigned int> &pos, const sf::Color &color) {
    assert(pos.x >= 0);
    assert(pos.x < buffer_size.x);
    assert(pos.y >= 0);
    assert(pos.y < buffer_size.y);

    sf::Vertex &current_pixel = array_[pos.y * buffer_size.x + pos.x];
    current_pixel.position = {static_cast<float>(pos.x), static_cast<float>(pos.y)};
    current_pixel.color = color;
}

void buffer::draw_rect(const vec2<unsigned int> &pos, const vec2<unsigned int> &size, const sf::Color &color) {
    for (int x = pos.x; x < pos.x + size.x; x++) {
        for (int y = pos.y; y < pos.y + size.y; y++) {
            draw_pixel({static_cast<unsigned int>(x), static_cast<unsigned int>(y)}, color);
        }
    }
}

void buffer::draw_rect(const rect<unsigned int> &r) {
    draw_rect(r.pos, r.size, r.color);
}
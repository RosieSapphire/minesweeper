#include "buffer.h"

#include <cassert>

buffer::buffer():
    array_(sf::Points, buffer_dim) {}

void buffer::clear(const sf::Color &color) {
    for (unsigned int x = 0; x < buffer_size.x; x++) {
        for (unsigned int y = 0; y < buffer_size.y; y++) {
            put_pixel({static_cast<unsigned int>(x), static_cast<unsigned int>(y)}, color);
        }
    }
}

void buffer::put_pixel(const vec2<unsigned int> &pos, const sf::Color &color) {
    assert(pos.x >= 0);
    assert(pos.x < buffer_size.x);
    assert(pos.y >= 0);
    assert(pos.y < buffer_size.y);

    sf::Vertex &current_pixel = array_[pos.y * buffer_size.x + pos.x];
    current_pixel.position = {static_cast<float>(pos.x), static_cast<float>(pos.y)};
    current_pixel.color = color;
}

void buffer::draw_rect(const vec2<unsigned int> &pos, const vec2<unsigned int> &size, const sf::Color &color) {
    for (unsigned int x = pos.x; x < pos.x + size.x; x++) {
        for (unsigned int y = pos.y; y < pos.y + size.y; y++) {
            put_pixel({static_cast<unsigned int>(x), static_cast<unsigned int>(y)}, color);
        }
    }
}

void buffer::draw_rect(const rect<unsigned int> &r) {
    draw_rect(r.pos, r.size, r.color);
}

void buffer::draw_tri_tl(const vec2<unsigned int> &pos, const unsigned int &size, const sf::Color &color) {
    unsigned int current_length = size;
    for (unsigned int y = 0; y < size; y++) {
        for (unsigned int x = 0; x < current_length; x++) {
            put_pixel({pos.x + x, pos.y + y}, color);
        }

        current_length--;
    }
}

void buffer::draw_tri_tr(const vec2<unsigned int> &pos, const unsigned int &size, const sf::Color &color) {
    unsigned int current_length = size;
    for (unsigned int y = 0; y < size; y++) {
        for (unsigned int x = 0; x < current_length; x++) {
            put_pixel({pos.x + (size - x) - 1, pos.y + y}, color);
        }

        current_length--;
    }
}

void buffer::draw_tri_bl(const vec2<unsigned int> &pos, const unsigned int &size, const sf::Color &color) {
    unsigned int current_length = 1;
    for (unsigned int y = 0; y < size; y++) {
        for (unsigned int x = 0; x < current_length; x++) {
            put_pixel({pos.x + x, pos.y + y}, color);
        }

        current_length++;
    }
}

void buffer::draw_tri_br(const vec2<unsigned int> &pos, const unsigned int &size, const sf::Color &color) {
    unsigned int current_length = 1;
    for (unsigned int y = 0; y < size; y++) {
        for (unsigned int x = 0; x < current_length; x++) {
            put_pixel({pos.x + (size - x) - 1, pos.y + y}, color);
        }

        current_length++;
    }
}

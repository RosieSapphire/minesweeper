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

void buffer::draw_circle(const vec2<unsigned int> &pos, const float &radius, const sf::Color &color) {
    for (int x = pos.x; x < pos.x + radius * 2.0f + 1; x++) {
        for (int y = pos.y; y < pos.y + radius * 2.0f + 1; y++) {
            const vec2<float> center{pos.x + radius, pos.y + radius};
            const vec2<unsigned int> pixel_pos{static_cast<unsigned int>(x), static_cast<unsigned int>(y)};
            const float pixel_mag_sq = (static_cast<vec2<float>>(pixel_pos) - center).get_mag_sq();
            
            if (pixel_mag_sq <= radius * radius) {
                draw_pixel(pixel_pos, color);
            }
        }
    }
}
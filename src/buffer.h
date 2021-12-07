#ifndef BUFFER_H
#define BUFFER_H

#include "rect.h"

struct buffer {
    explicit buffer();

    void clear(const sf::Color &color = sf::Color::Black);
    void put_pixel(const vec2<unsigned int> &pos, const sf::Color &color);
    void draw_rect(const vec2<unsigned int> &pos, const vec2<unsigned int> &size, const sf::Color &color = sf::Color::White);
    void draw_rect(const rect<unsigned int> &r);

    sf::VertexArray get_array() const {return array_;}
    static constexpr vec2<unsigned int> get_size() {return buffer_size;}

private:
    sf::VertexArray array_;
    static constexpr vec2<unsigned int> buffer_size{800, 600};
    static constexpr unsigned int buffer_dim = buffer_size.x * buffer_size.y;
};

#endif
#ifndef RECT_H
#define RECT_H

#include <SFML/Graphics.hpp>
#include "vec2.h"

template <typename T>
struct rect {
    rect(): pos({0, 0}), size({64, 64}), color(sf::Color::White) {}
    explicit rect(const vec2<T> &pos, const vec2<T> &size, const sf::Color &color = sf::Color::White): pos(pos), size(size), color(color) {}
    explicit rect(const T &left, const T &right, const T &top, const T &bottom, const sf::Color &color = sf::Color::White): pos(left, top), size(right - left, bottom - top), color(color) {}

    template <typename U>
    explicit rect(const rect<U> &r): pos(static_cast<vec2<U>>(r.pos)), size(static_cast<vec2<U>>(r.size)), color(r.color) {}

    vec2<T> pos;
    vec2<T> size;
    sf::Color color;

    inline float left() const {return pos.x;}
    inline float right() const {return pos.x + size.x;}
    inline float top() const {return pos.y;}
    inline float bottom() const {return pos.y + size.y;}

    vec2<T> get_center() const {
        return pos + (size * 0.5f);
    }

    rect<T> get_expanded(const float &offset) const {
        return rect<T>{left() - offset, right() + offset, top() - offset, bottom() + offset, color};
    }

    bool overlapping_x(const rect &other) const {
        return left() <= other.right() && right() >= other.left();
    }

    bool overlapping_y(const rect &other) const {
        return top() <= other.bottom() && bottom() >= other.top();
    }

    bool overlapping(const rect &other) const {
        return overlapping_x(other) && overlapping_y(other);
    }
};

#endif
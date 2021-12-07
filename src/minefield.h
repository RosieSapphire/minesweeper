#ifndef MINEFIELD_H
#define MINEFIELD_H

#include "buffer.h"

struct minefield {
    struct tile {
        enum state : const unsigned char {
            hidden,
            revealed,
            flagged
        };

        explicit tile(const vec2<unsigned int> &pos, const sf::Color &color, const state &st): pos(pos), color(color), st(st) {}

        static constexpr int size = 16;
        vec2<unsigned int> pos;
        sf::Color color;
        state st;

        void draw(buffer& buff);
    };

    minefield field_();
};

#endif
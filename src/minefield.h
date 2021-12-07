#ifndef MINEFIELD_H
#define MINEFIELD_H

#include "buffer.h"

struct minefield {
    struct tile {
        static constexpr int size = 16;
        const vec2<unsigned int> pos = {25, 25};
        const sf::Color color = sf::Color::Blue;

        void draw(buffer& buff);
    };

    tile single_tile;
};

#endif
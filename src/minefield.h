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

        tile() = default;

        static constexpr int size = 16;
        const sf::Color dark =  { 60,  60,  60};
        const sf::Color mid =   {140, 140, 140};
        const sf::Color light = {210, 210, 210};
        vec2<unsigned int> pos;
        
        state st;
        bool has_mine = false;

        void draw(buffer& buff);
    };

    minefield(const vec2<unsigned int> &pos, const unsigned int &mines);

    static constexpr vec2<unsigned int> dimensions{20, 16};
    tile tiles[dimensions.x * dimensions.y];

    void draw(buffer &buff);
};

#endif
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

        static constexpr unsigned int size = 32;
        const sf::Color dark =  { 60,  60,  60};
        const sf::Color mid =   {140, 140, 140};
        const sf::Color light = {210, 210, 210};
        vec2<unsigned int> pos;
        
        state st;
        bool has_mine = false;

        inline void reveal() {st = state::revealed;}
        void draw(buffer& buff);
    };

    minefield(const vec2<unsigned int> &pos, const unsigned int &mines);

    static constexpr vec2<unsigned int> dimensions{20, 16};
    tile tiles[dimensions.x * dimensions.y];
    vec2<unsigned int> screen_pos;

    void draw(buffer &buff);

    inline vec2<unsigned int> screen_to_grid(const vec2<unsigned int> &mouse_pos) {return (mouse_pos - screen_pos) / tile::size;}
    inline tile &tile_at(const vec2<unsigned int> &pos) {return tiles[pos.y * dimensions.x + pos.x];}
};

#endif
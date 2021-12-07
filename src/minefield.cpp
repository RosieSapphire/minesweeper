#include "minefield.h"

void minefield::tile::draw(buffer& buff) {
    const rect<unsigned int> tile_rect(pos, size, color);
    buff.draw_rect(tile_rect);
}
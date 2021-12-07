#include "minefield.h"

void minefield::tile::draw(buffer& buff) {
    constexpr unsigned int edge_size = size / 8;
    constexpr unsigned int all_but_edge = size - edge_size;

    buff.draw_rect(pos, {edge_size, all_but_edge}, dark);
    buff.draw_rect({pos.x, pos.y + all_but_edge}, {size, edge_size}, dark);

    buff.draw_tri_tr(pos, edge_size, light);
    buff.draw_rect({pos.x + edge_size, pos.y}, {all_but_edge, edge_size}, light);
    buff.draw_rect({pos.x + all_but_edge, pos.y + edge_size}, {edge_size, all_but_edge - edge_size}, light);
    buff.draw_tri_tr({pos.x + all_but_edge, pos.y + all_but_edge}, edge_size, light);
    
    buff.draw_rect({pos.x + edge_size, pos.y + edge_size}, {all_but_edge - edge_size}, mid);
}

minefield::minefield(const vec2<unsigned int> &pos, const unsigned int &mines) {
    for (unsigned int y = 0; y < dimensions.y; y++) {
        for (unsigned int x = 0; x < dimensions.x; x++) {
            tiles[y * dimensions.x + x].pos = pos + vec2<unsigned int>{x * tile::size, y * tile::size};
            tiles[y * dimensions.x + x].st = tile::state::hidden;
        }
    }  
}

void minefield::draw(buffer &buff) {
    for (unsigned int y = 0; y < dimensions.y; y++) {
        for (unsigned int x = 0; x < dimensions.x; x++) {
            tiles[y * dimensions.x + x].draw(buff);
        }
    }        
}
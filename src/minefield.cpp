#include <random>

#include "minefield.h"

void minefield::tile::draw(buffer& buff) {
    const bool visible = !(st == state::hidden);
    const unsigned int edge_size = visible ? size / 12 : size / 8;
    const unsigned int all_but_edge = size - edge_size;

    if (visible) {
        buff.draw_rect(pos, {size, edge_size}, dark);
        buff.draw_rect({pos.x, pos.y + all_but_edge}, {size, edge_size}, dark);
        buff.draw_rect({pos.x, pos.y + edge_size}, {edge_size, all_but_edge}, dark);
        buff.draw_rect({pos.x + all_but_edge, pos.y + edge_size}, {edge_size, all_but_edge}, dark);

        if (has_mine) {
            buff.draw_rect(pos + edge_size, {size - (edge_size * 2)}, mid);
            buff.draw_rect(pos + (edge_size * 4), {size - (edge_size * 8)}, sf::Color::Black);

        }
        else buff.draw_rect(pos + edge_size, {size - (edge_size * 2)}, mid);
    } else {
        buff.draw_rect(pos, {edge_size, all_but_edge}, dark);
        buff.draw_rect({pos.x, pos.y + all_but_edge}, {size, edge_size}, dark);

        buff.draw_tri_tr(pos, edge_size, light);
        buff.draw_rect({pos.x + edge_size, pos.y}, {all_but_edge, edge_size}, light);
        buff.draw_rect({pos.x + all_but_edge, pos.y + edge_size}, {edge_size, all_but_edge - edge_size}, light);
        buff.draw_tri_tr({pos.x + all_but_edge, pos.y + all_but_edge}, edge_size, light);

        buff.draw_rect({pos.x + edge_size, pos.y + edge_size}, {all_but_edge - edge_size}, mid);        
    }    
}

minefield::minefield(const vec2<unsigned int> &pos, const unsigned int &mines) {
    for (unsigned int y = 0; y < dimensions.y; y++) {
        for (unsigned int x = 0; x < dimensions.x; x++) {
            tiles[y * dimensions.x + x].pos = pos + vec2<unsigned int>{x * tile::size, y * tile::size};
            tiles[y * dimensions.x + x].st = tile::state::hidden;
        }
    }  

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<unsigned int> xrand(0, dimensions.x - 1);
    std::uniform_int_distribution<unsigned int> yrand(0, dimensions.y - 1);

    for (int i = 0; i < mines; i++) {        
        vec2<unsigned int> spawn_pos;
        tile *current = nullptr;

        do {
            spawn_pos = {xrand(rng), yrand(rng)};
            current = &tiles[spawn_pos.y * dimensions.x + spawn_pos.x];
        } while (current->has_mine);

        current->has_mine = true;
    }

    for (int i = 0; i < 160; i++) {
        vec2<unsigned int> current_pos;
        tile *current = nullptr;

        do {
            current_pos = {xrand(rng), yrand(rng)};
            current = &tiles[current_pos.y * dimensions.x + current_pos.x];
        } while (current->st == tile::state::revealed);

        current->st = tile::state::revealed;
    }
}

void minefield::draw(buffer &buff) {
    for (unsigned int y = 0; y < dimensions.y; y++) {
        for (unsigned int x = 0; x < dimensions.x; x++) {
            tiles[y * dimensions.x + x].draw(buff);
        }
    }        
}
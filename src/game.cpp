#include "game.h"

game::game(const char *title):
    buffer_(),
    window_(sf::VideoMode(buffer::get_size().x, buffer::get_size().y), title, sf::Style::Titlebar | sf::Style::Close) {
    window_.setPosition({buffer::get_size().x, buffer::get_size().y});
    run();
}

game::~game() {
    window_.close();
}

void game::run() {
    while (window_.isOpen()) {
        poll();
        update();
        draw();
    }
}

void game::poll() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }
    }
}

void game::update() {
    const float delta_time = clock_.restart().asSeconds();
}

void game::draw() {
    buffer_.clear();

    const sf::Color dark =   {60,  60,  60};
    const sf::Color mid =    {140, 140, 140};
    const sf::Color light = {210, 210, 210};
    constexpr vec2<unsigned int> tile_pos(16);
    constexpr unsigned int tile_size = 16;
    constexpr unsigned int edge_size = tile_size / 8;
    constexpr unsigned int all_but_edge = tile_size - edge_size;

    // triangle drawing routine

    // dark
    buffer_.draw_rect(tile_pos, {edge_size, all_but_edge}, dark);
    buffer_.draw_rect({tile_pos.x, tile_pos.y + all_but_edge}, {tile_size, edge_size}, dark);

    // light
    buffer_.draw_tri_tr(tile_pos, edge_size, light);
    buffer_.draw_rect({tile_pos.x + edge_size, tile_pos.y}, {all_but_edge, edge_size}, light);
    buffer_.draw_rect({tile_pos.x + all_but_edge, tile_pos.y + edge_size}, {edge_size, all_but_edge - edge_size}, light);
    buffer_.draw_tri_tr({tile_pos.x + all_but_edge, tile_pos.y + all_but_edge}, edge_size, light);
    
    buffer_.draw_rect({tile_pos.x + edge_size, tile_pos.y + edge_size}, {all_but_edge - edge_size}, mid);

    // middle

    window_.clear();
    window_.draw(buffer_.get_array());
    window_.display();
}
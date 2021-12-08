#include "game.h"

game::game(const char *title):
    buffer_(),
    window_(sf::VideoMode(buffer::get_size().x, buffer::get_size().y), title, sf::Style::Titlebar | sf::Style::Close),
    field_({32}, 32) 
    {
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
        switch (event.type) {
            case sf::Event::Closed:
                window_.close();
                break;

            case sf::Event::MouseButtonPressed:
                const auto mouse_pos = vec2<unsigned int>(static_cast<unsigned int>(sf::Mouse::getPosition(window_).x), static_cast<unsigned int>(sf::Mouse::getPosition(window_).y));
                minefield::tile &tile_at_mouse_pos = field_.tile_at(field_.screen_to_grid(mouse_pos));
                if (tile_at_mouse_pos.st == minefield::tile::state::hidden) {
                    tile_at_mouse_pos.st = minefield::tile::state::revealed;
                }

                break;
        }
    }
}

void game::update() {}

void game::draw() {
    buffer_.clear();
    field_.draw(buffer_);    

    window_.clear();
    window_.draw(buffer_.get_array());
    window_.display();
}
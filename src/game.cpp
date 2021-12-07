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
    buffer_.draw_tri_tl({24, 24}, 16, sf::Color::Green);
    buffer_.draw_tri_tr({48, 24}, 16, sf::Color::Green);
    buffer_.draw_tri_bl({24, 48}, 16, sf::Color::Green);
    buffer_.draw_tri_br({48, 48}, 16, sf::Color::Green);

    window_.clear();
    window_.draw(buffer_.get_array());
    window_.display();
}
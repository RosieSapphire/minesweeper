#include "game.h"

game::game(const char *title):
    buffer_(),
    window_(sf::VideoMode(buffer::get_size().x, buffer::get_size().y), title, sf::Style::Close) {
    window_.setPosition({buffer::get_size().x, buffer::get_size().y});
    run();
}

game::~game() {
    window_.close();
}

void game::run() {
    while (window_.isOpen()) {
        poll();

        // This is fucking minesweeper. We don't need a super percise delta-time, that's just a goddamn waste of resources.
        float elapsed_time = clock_.restart().asSeconds();
        while (elapsed_time > 0.0f) {
            const float delta_time = std::min(0.0025f, elapsed_time);
            update(delta_time);
            elapsed_time -= delta_time;
        }
        
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

void game::update(const float &delta_time) {}

void game::draw() {
    buffer_.clear();
    
    window_.clear();
    window_.draw(buffer_.get_array());
    window_.display();
}
#ifndef GAME_H
#define GAME_H

#include "buffer.h"

struct game {
    explicit game(const char *title);
    ~game();

    void run();

private:
    sf::Clock clock_;

    buffer buffer_;
    sf::RenderWindow window_;

    void poll();
    void update(const float &delta_time);
    void draw();
};

#endif
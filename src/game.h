#ifndef GAME_H
#define GAME_H

#include "minefield.h"

struct game {
    explicit game(const char *title);
    ~game();

    void run();

private:
    buffer buffer_;
    sf::RenderWindow window_;

    minefield field_;

    void poll();
    void update();
    void draw();
};

#endif
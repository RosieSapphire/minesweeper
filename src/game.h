#ifndef GAME_H
#define GAME_H

#include <random>

#include "brick.h"
#include "ball.h"
#include "paddle.h"

struct game {
    explicit game(const char *title);
    ~game();

    void run();

private:
    sf::Clock clock_;

    buffer buffer_;
    sf::RenderWindow window_;

    rect<float> walls_;

    static constexpr vec2<int> game_over_res{23, 11};
    static constexpr int game_over_dim = game_over_res.x * game_over_res.y;
    static constexpr bool game_over_pixels[game_over_dim] = {
        true,true,true,true,true,false,true,true,true,true,true,false,true,false,false,false,true,false,true,true,true,true,true,
        true,false,false,false,false,false,true,false,false,false,true,false,true,true,false,true,true,false,true,false,false,false,false,
        true,false,true,true,true,false,true,true,true,true,true,false,true,false,true,false,true,false,true,true,true,false,false,
        true,false,false,false,true,false,true,false,false,false,true,false,true,false,false,false,true,false,true,false,false,false,false,
        true,true,true,true,true,false,true,false,false,false,true,false,true,false,false,false,true,false,true,true,true,true,true,
        false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
        true,true,true,true,true,false,true,false,false,false,true,false,true,true,true,true,true,false,true,true,true,true,false,
        true,false,false,false,true,false,true,false,false,false,true,false,true,false,false,false,false,false,true,false,false,false,true,
        true,false,false,false,true,false,true,false,false,false,true,false,true,true,true,false,false,false,true,false,false,false,true,
        true,false,false,false,true,false,false,true,false,true,false,false,true,false,false,false,false,false,true,true,true,true,false,
        true,true,true,true,true,false,false,false,true,false,false,false,true,true,true,true,true,false,true,false,true,false,false
    };

    bool game_over = false;

    static constexpr vec2<int> bricks_dim{18, 4};
    static constexpr vec2<int> bricks_offset{40, 40};
    static constexpr int brick_count = bricks_dim.x * bricks_dim.y;
    brick bricks_[brick_count];
    ball ball_;
    paddle paddle_;

    void poll();
    void update(const float &delta_time);
    void draw();
};

#endif
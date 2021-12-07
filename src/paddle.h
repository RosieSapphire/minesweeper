#ifndef PADDLE_H
#define PADDLE_H

#include "buffer.h"
#include "ball.h"

struct paddle {
    explicit paddle(const vec2<float> &pos);

    void move(const rect<float> &walls, const float &delta_time);
    bool check_ball_collision(const ball &b) const;
    void execute_ball_collision(ball &b) const;
    void reset_cooldown() {cooldown = false;}
    void draw(buffer &buff);

private:
    static constexpr float speed = 650.0f;
    static constexpr float fixed_range_angle = 0.2f;
    static constexpr float angle_multiplier = 0.04f;
    static constexpr int wing_size = 18;
    const sf::Color wing_color{sf::Color::Red};
    rect<float> rect_;

    mutable bool cooldown = false;
};

#endif
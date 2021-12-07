#include "ball.h"

ball::ball(const vec2<float> &pos, const vec2<float> &vel):
    rect_(pos, {12.0f, 12.0f}, sf::Color::Yellow),
    vel_(vel.get_normalized()) {}

void ball::move(const float &delta_time) {
    rect_.pos += vel_ * speed * delta_time;
}

void ball::draw(buffer &buff) {
    buff.draw_circle(static_cast<vec2<unsigned int>>(rect_.pos), rect_.size.x * 0.5f, rect_.color);
}

int ball::do_wall_collision(const rect<float> &walls) {
    // 0 = nothing, 1 = wall, 2 = floor
    int collided = 0;
    if (rect_.left() < walls.left()) {
        rect_.pos.x = walls.left();
        bounce_x();
        collided = 1;
    } else if (rect_.right() >= walls.right()) {
        rect_.pos.x = static_cast<float>(walls.right()) - rect_.size.x - 1.0f;
        bounce_x();
        collided = 1;
    }

    if (rect_.top() < walls.top()) {
        rect_.pos.y = walls.top();
        bounce_y();
        collided = 1;
    } else if (rect_.bottom() >= walls.bottom()) {
        rect_.pos.y = static_cast<float>(walls.bottom()) - rect_.size.y - 1.0f;
        bounce_y();
        collided = 2;
    }

    return collided;
}
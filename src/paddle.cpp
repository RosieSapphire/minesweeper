#include "paddle.h"

#include <cassert>

paddle::paddle(const vec2<float> &pos):
    rect_(pos, {100.0f, 30.0f}) {
}

void paddle::move(const rect<float> &walls, const float &delta_time) {
    int direction = 0;
    direction += sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    rect_.pos.x += direction * speed * delta_time;
    
    if (rect_.left() < walls.left()) {
        rect_.pos.x = walls.left();
    } else if (rect_.right() >= walls.right()) {
        rect_.pos.x = walls.right() - (rect_.size.x + 1);
    }
}

void paddle::draw(buffer &buff) {
    rect<unsigned int> outer_rect{rect_};
    outer_rect.color = wing_color;
    rect<unsigned int> inner_rect{outer_rect.pos + vec2<unsigned int>{wing_size, 0}, outer_rect.size - vec2<unsigned int>{wing_size * 2, 0}, rect_.color};

    buff.draw_rect(outer_rect);
    buff.draw_rect(inner_rect);
}

bool paddle::check_ball_collision(const ball &b) const {
    return !cooldown && rect_.overlapping(b.get_rect());
}

void paddle::execute_ball_collision(ball &b) const {
    assert(check_ball_collision(b));

    const vec2<float> ball_center(b.get_rect().get_center());
    const vec2<float> paddle_center(rect_.get_center());

    const bool rebounding_from_inside = std::signbit(b.get_vel().x) == std::signbit(vec2<float>{ball_center - paddle_center}.x);
    const bool inside_x_range = ball_center.x >= rect_.left() && ball_center.x <= rect_.right();

    if (rebounding_from_inside || inside_x_range) {
        const float diff_x = b.get_rect().get_center().x - rect_.get_center().x;
        const float fixed_bounce_half_range = rect_.size.x * 0.25f;
        const bool in_fixed_range = std::abs(diff_x) <= fixed_bounce_half_range;
        
        if (in_fixed_range) {
            if (diff_x > 0.0f) {
                b.set_vel({fixed_range_angle, -1.0f});
            } else {
                b.set_vel({-fixed_range_angle, -1.0f});
            }
        } else {
            b.set_vel({diff_x * angle_multiplier, -1.0f});
        }
    } else {
        b.bounce_x();
    }

    cooldown = true;
}
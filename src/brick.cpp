#include "brick.h"

#include <cassert>

brick::brick(const rect<float> &r):
    rect_(r),
    destroyed(false) {}

bool brick::check_ball_collision(const ball &b) {
    if (destroyed) return false;
    return rect_.overlapping(b.get_rect());
}

void brick::execute_ball_collision(ball &b) {
    assert(check_ball_collision(b));

    const vec2<float> ball_center(b.get_rect().pos + (b.get_rect().size * 0.5f));
    const vec2<float> brick_center(rect_.pos + (rect_.size * 0.5f));
    if (std::signbit(b.get_vel().x) == std::signbit(vec2<float>{ball_center - brick_center}.x)) {
        b.bounce_y();
    } else if (ball_center.x >= rect_.left() && ball_center.x <= rect_.right()) {
        b.bounce_y();
    } else {
        b.bounce_x();
    }
    
    destroyed = true;
}

void brick::draw(buffer &buff) {
    if (destroyed) return;
    buff.draw_rect(static_cast<rect<unsigned int>>(rect_.get_expanded(-2.0f)));
}
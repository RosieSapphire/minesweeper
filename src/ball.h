#ifndef BALL_H
#define BALL_H

#include "buffer.h"

struct ball {
    explicit ball(const vec2<float> &pos, const vec2<float> &vel);

    inline void bounce_x() {vel_.x *= -1.0f;}
    inline void bounce_y() {vel_.y *= -1.0f;}
    void move(const float &delta_time);
    void draw(buffer &buff);

    int do_wall_collision(const rect<float> &walls);

    inline void set_vel(const vec2<float> &new_vel) {vel_ = new_vel.get_normalized();}
    inline rect<float> get_rect() const {return rect_;}
    inline vec2<float> get_vel() const {return vel_;}

private:
    static constexpr float speed = 600.0f;
    rect<float> rect_;
    vec2<float> vel_;
};

#endif
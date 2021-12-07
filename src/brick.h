#ifndef BRICK_H
#define BRICK_H

#include "ball.h"

struct brick {
    brick() = default;
    explicit brick(const rect<float> &r);

    inline void destroy() {destroyed = true;}
    bool check_ball_collision(const ball &b);
    void execute_ball_collision(ball &b);
    void draw(buffer &buff);

    inline rect<float> get_rect() const {return rect_;}

private:
    rect<float> rect_;
    bool destroyed = true;
};

#endif
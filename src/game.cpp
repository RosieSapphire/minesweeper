#include "game.h"

game::game(const char *title):
    buffer_(),
    window_(sf::VideoMode(buffer::get_size().x, buffer::get_size().y), title, sf::Style::Close),
    walls_({0.0f, 0.0f}, static_cast<vec2<float>>(buffer_.get_size())),
    ball_(324.0f, {1.0f, 1.0f}),
    paddle_({400.0f, 500.0f}) {
    window_.setPosition({buffer::get_size().x, buffer::get_size().y});

    const sf::Color colors[4] = {sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Cyan};
    const vec2<float> bricks_pos{40.0f, 40.0f};
    int i = 0;
    for (int x = 0; x < bricks_dim.x; x++) {
        for (int y = 0; y < bricks_dim.y; y++) {
            const rect<float> brick_rect(bricks_pos + vec2<float>{static_cast<float>(x * 40.0f), static_cast<float>(y * 24.0f)}, {40.0f, 24.0f}, colors[y]);
            bricks_[i] = brick(brick_rect);
            i++;
        }
    }

    run();
}

game::~game() {
    window_.close();
}

void game::run() {
    while (window_.isOpen()) {
        poll();

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

void game::update(const float &delta_time) {
    if (game_over) return;

    paddle_.move(walls_, delta_time);
    ball_.move(delta_time);

    if (paddle_.check_ball_collision(ball_)) {
        paddle_.execute_ball_collision(ball_);
    }

    switch (ball_.do_wall_collision(walls_)) {
        case 1:
            paddle_.reset_cooldown();
            break;

        case 2:
            game_over = true;
    }

    bool col_flag = false;
    float min_col_distance;
    int min_col_index;
    for (int i = 0; i < brick_count; i++) {
        brick &current = bricks_[i];
        if (current.check_ball_collision(ball_)) {
            const vec2<float> ball_center{ball_.get_rect().pos + (ball_.get_rect().size * 0.5f)};
            const vec2<float> brick_center{current.get_rect().pos + (current.get_rect().size * 0.5f)};
            const float new_col_distance = (ball_center - brick_center).get_mag_sq();

            if (col_flag) {
                
                if (new_col_distance < min_col_distance) {
                    min_col_distance = new_col_distance;
                    min_col_index = i;
                }
            } else {
                min_col_distance = new_col_distance;
                min_col_index = i;
                col_flag = true;
            }
        }
    }

    if (col_flag) {
        bricks_[min_col_index].execute_ball_collision(ball_);
        paddle_.reset_cooldown();
    }
}

void game::draw() {
    if(game_over) {
        buffer_.clear();
        constexpr int pixel_size = 20;
        for (unsigned int y = 0; y < game_over_res.y; y++) {
            for (unsigned int x = 0; x < game_over_res.x; x++) {
                if (game_over_pixels[y * game_over_res.x + x]) {
                    buffer_.draw_rect({200 + (x * pixel_size), 150 + (y * pixel_size)}, {pixel_size, pixel_size}, sf::Color::Blue);
                }
            }
        }
        window_.clear();
        window_.draw(buffer_.get_array());
        window_.display();

        return;
    } else {
        buffer_.clear();
        paddle_.draw(buffer_);
        ball_.draw(buffer_);

        for (auto& b : bricks_) {
            b.draw(buffer_);
        }

        window_.clear();
        window_.draw(buffer_.get_array());
        window_.display();
    }
}
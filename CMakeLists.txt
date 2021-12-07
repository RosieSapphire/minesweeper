cmake_minimum_required(VERSION 3.22.0)
project(fart-annoyed)

add_executable(fart-annoyed src/vec2.h
                   src/rect.h
                   src/brick.h src/brick.cpp
                   src/ball.h src/ball.cpp
                   src/paddle.h src/paddle.cpp
                   src/game.h src/game.cpp
                   src/buffer.h src/buffer.cpp
                   src/main.cpp)

target_include_directories(fart-annoyed PUBLIC /usr/include/SFML)
target_link_libraries(fart-annoyed sfml-system sfml-graphics sfml-window)

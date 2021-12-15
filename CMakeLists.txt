cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(minesweeper)

add_executable(minesweeper
                   src/vec2.h
                   src/rect.h
                   src/game.h src/game.cpp
                   src/buffer.h src/buffer.cpp
                   src/minefield.h src/minefield.cpp
                   src/main.cpp)

target_include_directories(minesweeper PUBLIC /usr/include/SFML)
target_link_libraries(minesweeper sfml-system sfml-graphics sfml-window)

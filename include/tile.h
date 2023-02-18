#ifndef TILE_H
#define TILE_H

#include <stdbool.h>

#define TILES_X 16
#define TILES_Y 16
#define TILE_SIZE 48

void tiles_init(void);
void tiles_update(int window_height);
void tiles_draw(int win_width, int win_height);
void tiles_terminate(void);

#endif

#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>
#include "color.h"

void window_init(int width, int height);
void window_set_is_running(bool is_running);
bool window_is_running(void);
bool window_mouse_down_left(void);
bool window_mouse_down_right(void);
void window_mouse_pos_get(int *out);
void window_clear(struct rgba col);
void window_display(void);
void window_terminate(void);

#endif

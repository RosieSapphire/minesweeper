#ifndef __WINDOW_H
#define __WINDOW_H

/************
 * INCLUDES *
 ************/

#include <stdint.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>

/******************************
 * PUBLIC FUNCTION PROTOTYPES *
 ******************************/

extern void window_init(const uint16_t width, const uint16_t height);
extern void window_running_set(const bool t);
extern bool window_running_get(void);
extern void window_terminate(void);

extern bool window_lmb_held(void);
extern bool window_rmb_held(void);
extern void window_mouse_pos_get(int16_t *const o);

extern void window_clear(const float r,
                         const float g,
                         const float b,
                         const float a);
extern void window_display(void);

#endif /* #ifndef __WINDOW_H */

#ifndef __WINDOW_H
#define __WINDOW_H

/************
 * INCLUDES *
 ************/

#include <stdint.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>

/***********
 * DEFINES *
 ***********/

#define WINDOW_NAME_MAX (64u)

/**************
 * STRUCTURES *
 **************/

struct window {
        char        name[WINDOW_NAME_MAX];
        GLFWwindow *handle;
        uint32_t    width;
        uint32_t    height;
};

/******************************
 * PUBLIC FUNCTION PROTOTYPES *
 ******************************/

extern void window_init(struct window *const restrict wnd,
                        const char *const restrict name,
                        const uint32_t width,
                        const uint32_t height);
extern void window_running_set(struct window *const wnd, const bool t);
extern bool window_running_get(const struct window *const wnd);
extern void window_terminate(struct window *const wnd);

/* TODO: Extract these to another file! */
extern bool window_lmb_held(const struct window *const wnd);
extern bool window_rmb_held(const struct window *const wnd);
extern void window_mouse_pos_get(const struct window *const restrict wnd,
                                 int16_t *const restrict o);

extern void window_poll(const struct window *const wnd);

#endif /* #ifndef __WINDOW_H */

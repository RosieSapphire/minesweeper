/************
 * INCLUDES *
 ************/

#define _POSIX_C_SOURCE (200809L)

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "error.h"

#include "window.h"

/*********************
 * PRIVATE VARIABLES *
 *********************/

/* FIXME: Move these to `input.c` or something. */
static bool lmb_held_prev = false;
static bool rmb_held_prev = false;

static void window_key_callback(GLFWwindow *const w,
                                int               k,
                                int               sc,
                                int               a,
                                int               m)
{
        (void)sc;
        (void)m;

        if (a != GLFW_PRESS)
                return;

        if (k != GLFW_KEY_ESCAPE)
                return;

        glfwSetWindowShouldClose(w, true);
}

static void __attribute__((noreturn)) glfw_err_cbfn(const int         c,
                                                    const char *const msg)
{
        fprintf(stderr, "GLFW::ERROR::%d: %s.\n", c, msg);
        exit(EXIT_FAILURE);
}

static void window_name_set(struct window *const restrict wnd,
                            const char *const restrict in)
{
        const size_t l = strnlen(in, WINDOW_NAME_MAX);

        assertf(l > 0ul, "Window name must have at least 1 character.");
        assertf(l < WINDOW_NAME_MAX,
                "Window name exceeds maximum of %u (\"%s\" has %lu).",
                WINDOW_NAME_MAX,
                in,
                l);

        (void)memcpy(wnd->name, in, l);
        wnd->name[l] = '\0';
}

void window_init(struct window *const restrict wnd,
                 const char *const restrict name,
                 const uint32_t width,
                 const uint32_t height)
{
        assertf(wnd, "Window struct input pointer is NULL.");
        assertf(!wnd->handle, "Window already has handle <%p>.", wnd->handle);
        assertf(wnd->name[0] == '\0',
                "Window already has name \"%s\".",
                wnd->name);
        assertf(!wnd->width, "Window already has width %u.", wnd->width);
        assertf(!wnd->height, "Window already has height %u.", wnd->height);
        assertf(name, "Can't make window with no name.");
        assertf(width, "Can't make window with 0 width.");
        assertf(height, "Can't make window with 0 height.");

        glfwSetErrorCallback(glfw_err_cbfn);
        glfwInit();

        /* TODO: Add more hints and center window. */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window_name_set(wnd, name);
        wnd->handle =
                glfwCreateWindow((int)width, (int)height, name, NULL, NULL);
        wnd->width  = width;
        wnd->height = height;

        glfwMakeContextCurrent(wnd->handle);
        glfwSetKeyCallback(wnd->handle, window_key_callback);

        /* FIXME: Move this to another block of logic! */
        gladLoadGL();
        glViewport(0, 0, (int)width, (int)height);
        glDisable(GL_DEPTH);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
}

void window_running_set(struct window *const wnd, const bool t)
{
        glfwSetWindowShouldClose(wnd->handle, t);
}

bool window_running_get(const struct window *const wnd)
{
        return (!glfwWindowShouldClose(wnd->handle));
}

void window_terminate(struct window *const wnd)
{
        assertf(wnd, "Trying to terminate a NULL window.");
        assertf(wnd->name[0] != '\0', "Window never had a name.");
        assertf(wnd->handle, "Window never had a handle.");
        assertf(wnd->width, "Window never had a width.");
        assertf(wnd->height, "Window never had a height.");

        wnd->height = 0u;
        wnd->width  = 0u;
        glfwDestroyWindow(wnd->handle);
        memset(wnd->name, 0, WINDOW_NAME_MAX);
        glfwTerminate();
}

bool window_lmb_held(const struct window *const wnd)
{
        const bool now =
                glfwGetMouseButton(wnd->handle, GLFW_MOUSE_BUTTON_LEFT);
        const bool dif = now && !lmb_held_prev;

        lmb_held_prev = now;

        return (dif);
}

bool window_rmb_held(const struct window *const wnd)
{
        int mouse_button_right_now =
                glfwGetMouseButton(wnd->handle, GLFW_MOUSE_BUTTON_RIGHT);
        int ret = mouse_button_right_now && !rmb_held_prev;

        rmb_held_prev = mouse_button_right_now;

        return (ret);
}

void window_mouse_pos_get(const struct window *const restrict wnd,
                          int16_t *const restrict o)
{
        double x, y;

        glfwGetCursorPos(wnd->handle, &x, &y);

        assertf(x >= -(INT16_MAX + 1) && x <= INT16_MAX,
                "Mouse X out of range: %lf.",
                x);
        assertf(y >= -(INT16_MAX + 1) && y <= INT16_MAX,
                "Mouse Y out of range: %lf.",
                y);

        o[0] = (int16_t)x;
        o[1] = (int16_t)y;
}

void window_clear(const float r, const float g, const float b, const float a)
{
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
}

void window_display(const struct window *const wnd)
{
        glfwPollEvents();
        glfwSwapBuffers(wnd->handle);
}

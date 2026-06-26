#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "error.h"

#include "window.h"

static GLFWwindow *win_ptr       = NULL;
static uint16_t    win_width     = 0u;
static uint16_t    win_height    = 0u;
static bool        lmb_held_prev = false;
static bool        rmb_held_prev = false;

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

void window_init(const uint16_t width, const uint16_t height)
{
        assert(!win_ptr);

        glfwSetErrorCallback(glfw_err_cbfn);
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        win_width  = width;
        win_height = height;
        win_ptr = glfwCreateWindow(width, height, "Minesweeper", NULL, NULL);

        glfwMakeContextCurrent(win_ptr);
        glfwSetKeyCallback(win_ptr, window_key_callback);

        gladLoadGL();
        glViewport(0, 0, width, height);
        glDisable(GL_DEPTH);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
}

void window_running_set(const bool t)
{
        glfwSetWindowShouldClose(win_ptr, t);
}

bool window_running_get(void)
{
        return (!glfwWindowShouldClose(win_ptr));
}

void window_terminate(void)
{
        glfwDestroyWindow(win_ptr);
        glfwTerminate();
}

bool window_lmb_held(void)
{
        const bool now = glfwGetMouseButton(win_ptr, GLFW_MOUSE_BUTTON_LEFT);
        const bool dif = now && !lmb_held_prev;

        lmb_held_prev = now;

        return (dif);
}

bool window_rmb_held(void)
{
        int mouse_button_right_now =
                glfwGetMouseButton(win_ptr, GLFW_MOUSE_BUTTON_RIGHT);
        int ret = mouse_button_right_now && !rmb_held_prev;

        rmb_held_prev = mouse_button_right_now;

        return (ret);
}

void window_mouse_pos_get(int16_t *const o)
{
        double x, y;

        glfwGetCursorPos(win_ptr, &x, &y);

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

void window_display(void)
{
        glfwPollEvents();
        glfwSwapBuffers(win_ptr);
}

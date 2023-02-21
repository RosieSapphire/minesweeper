#include <stdio.h>
#include <assert.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"

static GLFWwindow *handle = NULL;
static int window_width, window_height;

static int mouse_button_left_last = 0;
static int mouse_button_right_last = 0;

static void window_key_callback(GLFWwindow *w, int k, int sc, int a, int m)
{
	sc = m = 0;
	assert(!sc && !m);

	w = handle;

	if(a != GLFW_PRESS)
		return;

	if(k == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(w, true);
		return;
	}
}

void window_init(int width, int height)
{
	assert(!handle);

	int glfw_status = glfwInit();

	if(!glfw_status) {
		printf("Failed to load GLFW.\n");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_width = width;
	window_height = height;
	handle = glfwCreateWindow(width, height, "Minesweeper", NULL, NULL);

	if(!handle) {
		printf("Failed to create window.\n");
		return;
	}

	glfwMakeContextCurrent(handle);
	glfwSetKeyCallback(handle, window_key_callback);

	gladLoadGL();
	glViewport(0, 0, width, height);
	glDisable(GL_DEPTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void window_set_is_running(bool is_running)
{
	glfwSetWindowShouldClose(handle, is_running);
}

bool window_mouse_down_left(void)
{
	int mouse_button_left_now =
		glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_LEFT);
	int ret = mouse_button_left_now && !mouse_button_left_last;

	mouse_button_left_last = mouse_button_left_now;

	return ret;
}

void window_mouse_pos_get(int *out)
{
	double x, y;

	glfwGetCursorPos(handle, &x, &y);
	out[0] = (int)x;
	out[1] = (int)y;
}

bool window_mouse_down_right(void)
{
	int mouse_button_right_now =
		glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_RIGHT);
	int ret = mouse_button_right_now && !mouse_button_right_last;

	mouse_button_right_last = mouse_button_right_now;

	return ret;
}

bool window_is_running(void)
{
	return !glfwWindowShouldClose(handle);
}

void window_clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void window_display(void)
{
	glfwPollEvents();
	glfwSwapBuffers(handle);
}

void window_terminate(void)
{
	glfwDestroyWindow(handle);
	glfwTerminate();
}

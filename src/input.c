#include <stdio.h>

#include <GLFW/glfw3.h>

#include "input.h"

struct input input_init(void)
{
        return (struct input) {
                .flags = INPUTS_NONE,
        };
}

struct input input_poll(struct input inp, const struct window *const wnd)
{
        glfwPollEvents();

        /* Shift the current bits down to previous and ONLY keep those. */
        inp.flags >>= 1u;
        inp.flags &= INPUT_MASK_PREV;

        if (glfwGetMouseButton(wnd->handle, GLFW_MOUSE_BUTTON_LEFT))
                inp.flags |= INPUT_LMB_CURR;

        if (glfwGetMouseButton(wnd->handle, GLFW_MOUSE_BUTTON_RIGHT))
                inp.flags |= INPUT_RMB_CURR;

        if (glfwGetKey(wnd->handle, GLFW_KEY_R))
                inp.flags |= INPUT_R_CURR;

        /* Check if we're pressing or releasing based on comparing input. */
        if (((inp.flags & INPUT_LMB_PREV) >> INPUT_LMB_PREV_SHIFT) ^
            ((inp.flags & INPUT_LMB_CURR) >> INPUT_LMB_CURR_SHIFT)) {
                if (inp.flags & INPUT_LMB_CURR)
                        inp.flags |= INPUT_LMB_PRESS;
                else
                        inp.flags |= INPUT_LMB_RELEASE;
        }

        if (((inp.flags & INPUT_RMB_PREV) >> INPUT_RMB_PREV_SHIFT) ^
            ((inp.flags & INPUT_RMB_CURR) >> INPUT_RMB_CURR_SHIFT)) {
                if (inp.flags & INPUT_RMB_CURR)
                        inp.flags |= INPUT_RMB_PRESS;
                else
                        inp.flags |= INPUT_RMB_RELEASE;
        }

        if (((inp.flags & INPUT_R_PREV) >> INPUT_R_PREV_SHIFT) ^
            ((inp.flags & INPUT_R_CURR) >> INPUT_R_CURR_SHIFT)) {
                if (inp.flags & INPUT_R_CURR)
                        inp.flags |= INPUT_R_PRESS;
                else
                        inp.flags |= INPUT_R_RELEASE;
        }

        return inp;
}

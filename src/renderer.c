#include <glad/glad.h>

#include "error.h"

#include "renderer.h"

void renderer_init(struct renderer *const ren)
{
        assertf(ren, "Trying to init NULL renderer.");
        assertf(!ren->flags,
                "Renderer already has flags: 0x%.8X.",
                ren->flags);

        ren->flags |= REND_FLAG_IS_INIT;
}

void renderer_terminate(struct renderer *const ren)
{
        assertf(ren, "Trying to terminate NULL renderer.");
        assertf(ren->flags & REND_FLAG_IS_INIT,
                "Render was never initialized.");

        ren->flags &= ~REND_FLAG_IS_INIT;
}

void renderer_clear(const float r,
                    const float g,
                    const float b,
                    const float a,
                    const bool  depth)
{
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | (depth * GL_DEPTH_BUFFER_BIT));
}

void renderer_display_to_window(const struct window *const restrict wnd,
                                const struct renderer *const restrict rnd)
{
        (void)rnd;
        glfwSwapBuffers(wnd->handle);
}

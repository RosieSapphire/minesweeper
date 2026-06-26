#include <glad/glad.h>

#include "error.h"

#include "renderer.h"

void renderer_init(struct renderer *const restrict ren,
                   const struct window *const restrict wnd)
{
        assertf(ren, "Trying to init NULL renderer.");
        assertf(!ren->flags,
                "Renderer already has flags: 0x%.8X.",
                ren->flags);

        assertf(wnd, "Trying to init renderer with NULL window.");
        assertf(wnd->width,
                "Trying to init renderer with window of width 0.");
        assertf(wnd->height,
                "Trying to init renderer with window of height 0.");

        gladLoadGL();
        glViewport(0, 0, (int)wnd->width, (int)wnd->height);
        glDisable(GL_DEPTH);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

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

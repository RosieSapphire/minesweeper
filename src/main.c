#include "window.h"
#include "renderer.h"
#include "input.h"
#include "tile.h"

#define WIN_WIDTH  (TILES_X * TILE_SIZE)
#define WIN_HEIGHT (TILES_Y * TILE_SIZE)

static struct window window = {
        .name   = "",
        .handle = NULL,
        .width  = 0u,
        .height = 0u,
};

static struct renderer renderer = {
        .flags  = REND_FLAGS_NONE,
        .shader = 0u,
};

static struct input input = {
        .flags = INPUTS_NONE,
};

int main(void)
{
        window_init(&window, "Minesweeper", WIN_WIDTH, WIN_HEIGHT);
        renderer_init(&renderer, &window);
        renderer_shader_load(&renderer, "res/vert.glsl", "res/frag.glsl");
        input = input_init();
        tiles_init(&renderer);

        while (window_running_get(&window)) {
                input = input_poll(input, &window);
                tiles_update(&window, input);

                renderer_clear(0.2f, 0.3f, 0.2f, 1.0f, false);
                tiles_draw(&renderer, &window);
                renderer_display_to_window(&window, &renderer);
        }

        tiles_terminate(&renderer);
        renderer_shader_unload(&renderer);
        renderer_terminate(&renderer);
        window_terminate(&window);

        return 0;
}

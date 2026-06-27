#include "window.h"
#include "renderer.h"
#include "input.h"
#include "tile.h"

static struct window window = {
        .name   = "",
        .handle = NULL,
        .width  = 0u,
        .height = 0u,
};

static struct renderer renderer = {
        .flags = REND_FLAGS_NONE,
};

static struct input input = {
        .flags = INPUTS_NONE,
};

static uint32_t shader = 0u;

int main(void)
{
        window_init(&window,
                    "Minesweeper",
                    TILES_X * TILE_SIZE,
                    TILES_Y * TILE_SIZE);
        renderer_init(&renderer, &window);
        shader = shader_load("res/vert.glsl", "res/frag.glsl");
        input  = input_init();
        tiles_init();

        while (window_running_get(&window)) {
                input = input_poll(input, &window);
                tiles_update(&window, input);

                renderer_clear(0.2f, 0.3f, 0.2f, 1.0f, false);
                tiles_draw(shader, window.width, window.height);
                renderer_display_to_window(&window, &renderer);
        }

        tiles_terminate();
        shader_unload(&shader);
        renderer_terminate(&renderer);
        window_terminate(&window);

        return 0;
}

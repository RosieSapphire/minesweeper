#include <stdio.h>

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
        .flags = REND_FLAGS_NONE,
};

static struct input input = {
        .flags = INPUTS_NONE,
};

int main(void)
{
        window_init(&window, "Minesweeper", WIN_WIDTH, WIN_HEIGHT);
        renderer_init(&renderer, &window);
        input = input_init();
        tiles_init();

        while (window_running_get(&window)) {
#if 0
                static size_t ind = 0ul;
#endif /* #if 0 */

                input = input_poll(input, &window);
                tiles_update(&window, input);

                renderer_clear(0.2f, 0.3f, 0.2f, 1.0f, false);
                tiles_draw(&window);
                renderer_display_to_window(&window, &renderer);
        }

        tiles_terminate();
        renderer_terminate(&renderer);
        window_terminate(&window);

        return 0;
}

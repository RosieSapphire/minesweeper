#include "window.h"
#include "tile.h"

#define WIN_WIDTH  (TILES_X * TILE_SIZE)
#define WIN_HEIGHT (TILES_Y * TILE_SIZE)

static struct window window = {
        .name   = "",
        .handle = NULL,
        .width  = 0u,
        .height = 0u,
};

int main(void)
{
        window_init(&window, "Minesweeper", WIN_WIDTH, WIN_HEIGHT);
        tiles_init();

        while (window_running_get(&window)) {
                window_clear(0.2f, 0.3f, 0.2f, 1.0f);

                tiles_update(&window);
                tiles_draw(&window);

                window_display(&window);
        }

        tiles_terminate();
        window_terminate(&window);

        return 0;
}

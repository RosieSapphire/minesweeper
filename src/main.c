#include "window.h"
#include "tile.h"

#define WIN_WIDTH (TILES_X * TILE_SIZE)
#define WIN_HEIGHT (TILES_Y * TILE_SIZE)

int main(void)
{
	window_init(WIN_WIDTH, WIN_HEIGHT);
	tiles_init();

	while(window_is_running()) {
		window_clear(0.2f, 0.3f, 0.2f, 1.0f);

		tiles_update(WIN_HEIGHT);
		tiles_draw(WIN_WIDTH, WIN_HEIGHT);

		window_display();
	}

	tiles_terminate();
	window_terminate();

	return 0;
}

#include <SFML/Graphics.h>
#include "minefield.h"

#define WINDOW_NAME		"Minesweeper"

int main() {
	sfVideoMode video_mode, desktop_mode;
	sfVector2i window_pos;
    sfEvent event;
    sfRenderWindow *window;
	vec2i_t mouse_pos;
    minefield_t field;
    buffer_t buff;

	field = minefield_create(vec2u_create_both(32), 32);
	buff.vertex_array = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(buff.vertex_array, sfPoints);
	sfVertexArray_resize(buff.vertex_array, BUFFER_DIMENSION);

	video_mode.bitsPerPixel = 24;
	video_mode.width = BUFFER_WIDTH;
	video_mode.height = BUFFER_HEIGHT;
	desktop_mode = sfVideoMode_getDesktopMode();
	window = sfRenderWindow_create(video_mode, WINDOW_NAME, sfClose, NULL);

    window_pos.x = (desktop_mode.width / 2) - (BUFFER_WIDTH / 2);
    window_pos.y = (desktop_mode.height / 2) - (BUFFER_HEIGHT / 2);
	sfRenderWindow_setPosition(window, window_pos);

    while (sfRenderWindow_isOpen(window)) {
		/* updating */
		while (sfRenderWindow_pollEvent(window, &event)) {
    	    switch (event.type) {
    	        case sfEvtClosed:
    	            sfRenderWindow_close(window);
    	            break;

    	        case sfEvtMouseButtonPressed: {
					tile_t *mouse_tile = NULL;
					sfVector2i mouse_pos_sf = sfMouse_getPositionRenderWindow(window);
					vec2i_t mouse_tile_pos;
					mouse_pos = vec2i_create(mouse_pos_sf.x, mouse_pos_sf.y);
					mouse_tile_pos = vec2i_create(((mouse_pos.x - field.screen_pos.x) / TILE_SIZE), ((mouse_pos.y - field.screen_pos.y) / TILE_SIZE));

    	            mouse_tile = &field.tiles[mouse_tile_pos.y * FIELD_WIDTH + mouse_tile_pos.x];
					if(mouse_tile_pos.x >= FIELD_WIDTH || mouse_tile_pos.x < 0 || mouse_tile_pos.y >= FIELD_HEIGHT || mouse_tile_pos.y < 0)
						break;

    	            if(sfMouse_isButtonPressed(sfMouseLeft)) {
    	                if(mouse_tile->st == TS_HIDDEN) {
    	                    mouse_tile->st = TS_REVEALED;
    	                }
    	            } else if(sfMouse_isButtonPressed(sfMouseRight)) {
    	                switch (mouse_tile->st) {
    	                    case TS_HIDDEN:
    	                        mouse_tile->st = TS_FLAGGED;
    	                        break;

    	                    case TS_FLAGGED:
    	                        mouse_tile->st = TS_HIDDEN;

							default:
								break;
    	                }
    	            }
    	        } break;

				default:
					break;
    	    }
    	}


		/* drawing */
		buffer_clear(&buff, sfBlack);
		minefield_draw(&field, &buff);

		sfRenderWindow_clear(window, sfBlack);
		sfRenderWindow_drawVertexArray(window, buff.vertex_array, NULL);
		sfRenderWindow_display(window);
    }

	sfRenderWindow_destroy(window);

    return 0;
}

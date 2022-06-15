#include <SFML/Graphics.hpp>
#include "minefield.h"

int main() {
    sf::Event event;
    sf::RenderWindow window;
	vec2i_t mouse_pos;
    minefield_t field;
    buffer_t buff;

	field = minefield_create({32}, 32);
	buff.vertex_array = sf::VertexArray(sf::Points, BUFFER_DIMENSION);

    window.create(sf::VideoMode(BUFFER_WIDTH, BUFFER_HEIGHT), "Minesweeper", sf::Style::Close);
    window.setPosition({1600 - BUFFER_WIDTH, 900 - BUFFER_HEIGHT});

    while (window.isOpen()) {
		// updating
		while (window.pollEvent(event)) {
    	    switch (event.type) {
    	        case sf::Event::Closed:
    	            window.close();
    	            break;

    	        case sf::Event::MouseButtonPressed: {
					sf::Vector2i mouse_pos_sf = sf::Mouse::getPosition(window);
					vec2i_t mouse_tile_pos;
					mouse_pos = {mouse_pos_sf.x, mouse_pos_sf.y};
					mouse_tile_pos = vec2i_create(((mouse_pos.x - field.screen_pos.x) / TILE_SIZE), ((mouse_pos.y - field.screen_pos.y) / TILE_SIZE));

    	            tile_t *mouse_tile = &field.tiles[mouse_tile_pos.y * FIELD_WIDTH + mouse_tile_pos.x];
					if(mouse_tile_pos.x >= FIELD_WIDTH || mouse_tile_pos.x < 0 || mouse_tile_pos.y >= FIELD_HEIGHT || mouse_tile_pos.y < 0)
						break;

    	            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    	                if (mouse_tile->st == TS_HIDDEN) {
    	                    mouse_tile->st = TS_REVEALED;
    	                }
    	            } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
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


		// drawing
		buffer_clear(&buff);
		minefield_draw(&field, &buff);

    	window.clear();
    	window.draw(buff.vertex_array);
    	window.display();
    }

    window.close();

    return 0;
}
